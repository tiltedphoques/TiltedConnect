#include "Server.h"
#include "SteamInterface.h"
#include <thread>
#include <algorithm>
#include <ScratchAllocator.h>
#include <StackAllocator.h>
#include <Buffer.h>
#include <cassert>

using namespace std::chrono;

Server::Server()
    : m_tickRate(10)
    , m_lastUpdateTime(0ns)
    , m_timeBetweenUpdates(100ms)
{
    SteamInterface::Acquire();

    m_pInterface = SteamNetworkingSockets();
    m_listenSock = k_HSteamListenSocket_Invalid;
}

Server::~Server()
{
    SteamInterface::Release();
}

bool Server::Host(const uint16_t aPort, uint32_t aTickRate)
{
    Close();

    SteamNetworkingIPAddr localAddress{};  // NOLINT(cppcoreguidelines-pro-type-member-init)
    localAddress.Clear();
    localAddress.m_port = aPort;
    m_listenSock = m_pInterface->CreateListenSocketIP(localAddress);

    if (m_tickRate == 0 && aTickRate == 0)
    {
        aTickRate = 10;
    }
    // If we pass 0, reuse the previously used tick rate
    else if (aTickRate == 0)
    {
        m_tickRate = aTickRate;
    }

    m_tickRate = aTickRate;
    m_timeBetweenUpdates = 1000ms / m_tickRate;

    return m_listenSock != k_HSteamListenSocket_Invalid;
}

void Server::Close()
{
    if (m_listenSock != k_HSteamListenSocket_Invalid)
        m_pInterface->CloseListenSocket(m_listenSock);

    m_listenSock = k_HSteamListenSocket_Invalid;
}

void Server::Update()
{
    m_currentTick = high_resolution_clock::now();

    if (m_listenSock != k_HSteamListenSocket_Invalid)
    {
        m_pInterface->RunCallbacks(this);

        while(true)
        {
            ISteamNetworkingMessage* pIncomingMessage = nullptr;
            const auto messageCount = m_pInterface->ReceiveMessagesOnListenSocket(m_listenSock, &pIncomingMessage, 1);
            if(messageCount <= 0 || pIncomingMessage == nullptr)
            {
                break;
                // TODO: Handle when messageCount is a negative number, it's an error
            }

            HandleMessage(pIncomingMessage->GetData(), pIncomingMessage->GetSize(), pIncomingMessage->GetConnection());

            pIncomingMessage->Release();
        }
    }

    if (m_currentTick - m_lastUpdateTime >= m_timeBetweenUpdates)
    {
        m_lastUpdateTime = m_currentTick;
        OnUpdate();
    }

    std::this_thread::sleep_for(2ms);
}

void Server::SendToAll(const void* apData, const uint32_t aSize, const EPacketFlags aPacketFlags)
{
    static thread_local ScratchAllocator s_allocator{1 << 16};

    assert(aSize < ((1 << 16) - 1));

    const auto pBuffer = static_cast<uint8_t*>(s_allocator.Allocate(size_t(aSize) + 1));
    assert(pBuffer);

    const auto pData = static_cast<const uint8_t*>(apData);

    pBuffer[0] = kPayload;
    std::copy(pData, pData + aSize, pBuffer + 1);

    for (const auto conn : m_connections)
    {
        m_pInterface->SendMessageToConnection(conn, pBuffer, aSize + 1,
            aPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable);
    }
}

void Server::Send(const ConnectionId_t aConnectionId, const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags) const
{
    static thread_local ScratchAllocator s_allocator{ 1 << 16 };

    assert(aSize < ((1 << 16) - 1));

    const auto pBuffer = static_cast<uint8_t*>(s_allocator.Allocate(size_t(aSize) + 1));
    assert(pBuffer);

    const auto pData = static_cast<const uint8_t*>(apData);

    pBuffer[0] = kPayload;
    std::copy(pData, pData + aSize, pBuffer + 1);

    m_pInterface->SendMessageToConnection(aConnectionId, pBuffer, aSize + 1,
        aPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable);
}

void Server::Kick(const ConnectionId_t aConnectionId)
{
    m_pInterface->CloseConnection(aConnectionId, 0, "Kick", true);

    Remove(aConnectionId);

    OnDisconnection(aConnectionId);
}

uint16_t Server::GetPort() const
{
    SteamNetworkingIPAddr address{};
    if(m_pInterface->GetListenSocketAddress(m_listenSock, &address))
    {
        return address.m_port;
    }

    return 0;
}

void Server::Remove(const ConnectionId_t aId)
{
    const auto it = std::find(std::begin(m_connections), std::end(m_connections), aId);
    if (it != std::end(m_connections) && !m_connections.empty())
    {
        std::iter_swap(it, std::end(m_connections) - 1);
        m_connections.pop_back();
    }
}

void Server::HandleMessage(const void* apData, const uint32_t aSize, const ConnectionId_t aConnectionId)
{
    // We handle the cases where packets target the current stack or the user stack
    if (aSize == 0)
        return;

    const auto pData = (uint8_t*)apData;
    switch(pData[0])
    {
    case kPayload:
        OnConsume(pData + 1, aSize - 1, aConnectionId);
        break;
    default:
        assert(false);
        break;
    }
}

void Server::SynchronizeClientClocks()
{
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(m_currentTick.time_since_epoch()).count();

    StackAllocator<1 << 10> allocator;
    ScopedAllocator _{ &allocator };

    const auto pBuffer = New<Buffer>(512);

    Buffer::Writer writer(pBuffer);
    writer.WriteBits(kServerTime, 8);
    writer.WriteBits(time, 64);

    for (const auto conn : m_connections)
    {
        m_pInterface->SendMessageToConnection(conn, pBuffer->GetData(), writer.GetBytePosition(), k_nSteamNetworkingSend_UnreliableNoDelay);
    }
}

void Server::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo)
{
    switch(apInfo->m_info.m_eState)
    {
    case k_ESteamNetworkingConnectionState_None:
        break;
    case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
    case k_ESteamNetworkingConnectionState_ClosedByPeer:
    {
        if (apInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
        {
            OnDisconnection(apInfo->m_hConn);

            Remove(apInfo->m_hConn);
        }

        m_pInterface->CloseConnection(apInfo->m_hConn, 0, nullptr, false);
        break;
    }
    case k_ESteamNetworkingConnectionState_Connecting:
    {
        if(m_pInterface->AcceptConnection(apInfo->m_hConn) != k_EResultOK)
        {
            m_pInterface->CloseConnection(apInfo->m_hConn, 0, nullptr, false);
            // TODO: Error handling
            break;
        }

        m_connections.push_back(apInfo->m_hConn);

        OnConnection(apInfo->m_hConn);
        break;
    }
    case k_ESteamNetworkingConnectionState_Connected:
        break;
    default:
        break;

    }
}

