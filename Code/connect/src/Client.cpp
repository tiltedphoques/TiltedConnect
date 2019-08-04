#include "Client.h"
#include "SteamInterface.h"

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
    m_pInterface->CloseConnection(m_connection, 0, nullptr, false);
    m_connection = k_HSteamNetConnection_Invalid;
}

void Client::Update()
{
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

        OnConsume(pIncomingMsg->GetData(), pIncomingMsg->GetSize());

        pIncomingMsg->Release();
    }

    OnUpdate();
}

void Client::Send(const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags) const
{
    m_pInterface->SendMessageToConnection(m_connection, apData, aSize, 
        aPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable);
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
