#include "Server.h"
#include "SteamInterface.h"
#include <thread>
#include <algorithm>

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
    const auto now = steady_clock::now();

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

            const uintptr_t ptr = pIncomingMessage->GetConnectionUserData();

            OnConsume(pIncomingMessage->GetData(), pIncomingMessage->GetSize(), pIncomingMessage->GetConnection());

            pIncomingMessage->Release();
        }
    }

    if (now - m_lastUpdateTime >= m_timeBetweenUpdates)
    {
        m_lastUpdateTime = now;
        OnUpdate();
    }

    std::this_thread::sleep_for(2ms);
}

void Server::SendToAll(const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags)
{
    for (const auto conn : m_connections)
        Send(conn, apData, aSize, aPacketFlags);
}

void Server::Send(ConnectionId_t aConnectionId, const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags) const
{
    m_pInterface->SendMessageToConnection(aConnectionId, apData, aSize,
        aPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable);
}

void Server::Kick(ConnectionId_t aConnectionId)
{
    m_pInterface->CloseConnection(aConnectionId, 0, "Kick", true);
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
            m_connections.erase(std::find(std::begin(m_connections), std::end(m_connections), apInfo->m_hConn));
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

