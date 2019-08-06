#include "Client.h"
#include "SteamInterface.h"
#include <ScratchAllocator.h>
#include <cassert>
#include "Buffer.h"

Client::Client()
{
    SteamInterface::Acquire();

    m_connection = k_HSteamNetConnection_Invalid;
    m_pInterface = SteamNetworkingSockets();
}

Client::~Client()
{
    SteamInterface::Release();
}

Client::Client(Client&& aRhs) noexcept
    : m_connection(k_HSteamNetConnection_Invalid)
    , m_pInterface(nullptr)
{
    SteamInterface::Acquire();

    this->operator=(std::move(aRhs));
}

Client& Client::operator=(Client&& aRhs) noexcept
{
    std::swap(m_connection, aRhs.m_connection);
    std::swap(m_pInterface, aRhs.m_pInterface);

    return *this;
}

bool Client::Connect(const std::string& acEndpoint)
{
    SteamNetworkingIPAddr remoteAddress{};
    remoteAddress.ParseString(acEndpoint.c_str());

    m_connection = m_pInterface->ConnectByIPAddress(remoteAddress);

    return m_connection != k_HSteamNetConnection_Invalid;
}

void Client::Close()
{
    if (m_connection != k_HSteamNetConnection_Invalid)
    {
        m_pInterface->CloseConnection(m_connection, 0, nullptr, true);
        m_connection = k_HSteamNetConnection_Invalid;

        OnDisconnected(kNormal);
    }
}

void Client::Update()
{
    m_clock.Update();

    m_pInterface->RunCallbacks(this);

    while(true)
    {
        ISteamNetworkingMessage* pIncomingMsg = nullptr;
        const auto messageCount = m_pInterface->ReceiveMessagesOnConnection(m_connection, &pIncomingMsg, 1);
        if(messageCount <= 0 || pIncomingMsg == nullptr)
        {
            // TODO: Handle error when messageCount < 0
            break;
        }

        HandleMessage(pIncomingMsg->GetData(), pIncomingMsg->GetSize());

        pIncomingMsg->Release();
    }

    OnUpdate();
}

void Client::Send(const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags) const
{
    static thread_local ScratchAllocator s_allocator{ 1 << 16 };

    assert(aSize < ((1 << 16) - 1));

    const auto pBuffer = static_cast<uint8_t*>(s_allocator.Allocate(size_t(aSize) + 1));
    assert(pBuffer);

    const auto pData = static_cast<const uint8_t*>(apData);

    pBuffer[0] = kPayload;
    std::copy(pData, pData + aSize, pBuffer + 1);

    m_pInterface->SendMessageToConnection(m_connection, pBuffer, aSize + 1, 
        aPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable);
}

bool Client::IsConnected() const
{
    if(m_connection != k_HSteamNetConnection_Invalid)
    {
        SteamNetConnectionInfo_t info{};
        if(m_pInterface->GetConnectionInfo(m_connection, &info))
        {
            return info.m_eState == k_ESteamNetworkingConnectionState_Connected;
        }
    }

    return false;
}

SteamNetworkingQuickConnectionStatus Client::GetConnectionStatus() const
{
    SteamNetworkingQuickConnectionStatus status{};

    if (m_connection != k_HSteamNetConnection_Invalid)
    {
        m_pInterface->GetQuickConnectionStatus(m_connection, &status);
    }

    return status;
}

const SynchronizedClock& Client::GetClock() const
{
    return m_clock;
}

void Client::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo)
{
    switch(apInfo->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_None:
        break;
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    {
        if (apInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
        {
            OnDisconnected(kTimeout);
        }
        else if (apInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
        {
            OnDisconnected(kLocalProblem);
        }
        else
        {
            OnDisconnected(kKicked);
        }

        m_pInterface->CloseConnection(m_connection, 0, nullptr, false);
        m_connection = k_HSteamNetConnection_Invalid;

        break;
    }
    case k_ESteamNetworkingConnectionState_Connecting:
        break;
    case k_ESteamNetworkingConnectionState_Connected:
        OnConnected();
        break;
    default:
        break;
    }
}

void Client::HandleMessage(const void* apData, uint32_t aSize)
{
    // We handle the cases where packets target the current stack or the user stack
    if (aSize == 0)
        return;

    auto pData = static_cast<const uint8_t*>(apData);

    const auto opcode = pData[0];
    
    pData += 1;
    aSize -= 1;

    switch (opcode)
    {
    case kPayload:
        OnConsume(pData, aSize);
        break;
    case kServerTime:
        HandleServerTime(pData, aSize);
        break;
    default:
        assert(false);
        break;
    }
}

void Client::HandleServerTime(const void* apData, uint32_t aSize)
{
    if (aSize < 8)
        return;

    const auto pData = static_cast<const uint8_t*>(apData);
    const auto connectionStatus = GetConnectionStatus();

    uint64_t serverTime;
    std::copy(pData, pData + 8, &serverTime);

    m_clock.Synchronize(serverTime, connectionStatus.m_nPing);
}
