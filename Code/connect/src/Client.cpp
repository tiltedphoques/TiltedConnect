#include "Client.hpp"
#include "SteamInterface.hpp"
#include <cassert>
#include "Buffer.hpp"
#include "Packet.hpp"
#include <google/protobuf/stubs/port.h>

namespace TiltedPhoques
{
    Client::Client() noexcept
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

    bool Client::Connect(const std::string& acEndpoint) noexcept
    {
        SteamNetworkingIPAddr remoteAddress{};
        remoteAddress.ParseString(acEndpoint.c_str());

        m_connection = m_pInterface->ConnectByIPAddress(remoteAddress, 0, nullptr);

        return m_connection != k_HSteamNetConnection_Invalid;
    }

    void Client::Close() noexcept
    {
        if (m_connection != k_HSteamNetConnection_Invalid)
        {
            m_pInterface->CloseConnection(m_connection, 0, nullptr, true);
            m_connection = k_HSteamNetConnection_Invalid;

            OnDisconnected(kNormal);
        }
    }

    void Client::Update() noexcept
    {
        m_clock.Update();

        m_pInterface->RunCallbacks(this);

        while (true)
        {
            ISteamNetworkingMessage* pIncomingMsg = nullptr;
            const auto cMessageCount = m_pInterface->ReceiveMessagesOnConnection(m_connection, &pIncomingMsg, 1);
            if (cMessageCount <= 0 || pIncomingMsg == nullptr)
            {
                // TODO: Handle error when messageCount < 0
                break;
            }

            HandleMessage(pIncomingMsg->GetData(), pIncomingMsg->GetSize());

            pIncomingMsg->Release();
        }

        OnUpdate();
    }

    void Client::Send(Packet* apPacket, const EPacketFlags acPacketFlags) const noexcept
    {
        m_pInterface->SendMessageToConnection(m_connection, apPacket->m_pData, apPacket->m_size,
            acPacketFlags == kReliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, nullptr);
    }

    bool Client::IsConnected() const noexcept
    {
        if (m_connection != k_HSteamNetConnection_Invalid)
        {
            SteamNetConnectionInfo_t info{};
            if (m_pInterface->GetConnectionInfo(m_connection, &info))
            {
                if(info.m_eState == k_ESteamNetworkingConnectionState_Connected)
                {
                    return GetClock().IsSynchronized();
                }
            }
        }

        return false;
    }

    SteamNetworkingQuickConnectionStatus Client::GetConnectionStatus() const noexcept
    {
        SteamNetworkingQuickConnectionStatus status{};

        if (m_connection != k_HSteamNetConnection_Invalid)
        {
            m_pInterface->GetQuickConnectionStatus(m_connection, &status);
        }

        return status;
    }

    const SynchronizedClock& Client::GetClock() const noexcept
    {
        return m_clock;
    }

    void Client::OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo)
    {
        switch (apInfo->m_info.m_eState)
        {
        case k_ESteamNetworkingConnectionState_None:
            break;
        case k_ESteamNetworkingConnectionState_ClosedByPeer:
        case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
        {
            m_pInterface->CloseConnection(m_connection, 0, nullptr, false);
            m_connection = k_HSteamNetConnection_Invalid;

            m_clock.Reset();

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

            break;
        }
        case k_ESteamNetworkingConnectionState_Connecting:
            break;
        case k_ESteamNetworkingConnectionState_Connected:
            // We don't notify here, wait for clock sync
            break;
        default:
            break;
        }
    }

    void Client::HandleMessage(const void* apData, uint32_t aSize) noexcept
    {
        // We handle the cases where packets target the current stack or the user stack
        if (aSize == 0)
            return;

        auto pData = static_cast<const uint8_t*>(apData);

        const auto cOpcode = pData[0];

        pData += 1;
        aSize -= 1;

        switch (cOpcode)
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

    void Client::HandleServerTime(const void* apData, uint32_t aSize) noexcept
    {
        if (aSize < 8)
            return;

        const auto cConnectionStatus = GetConnectionStatus();

        const auto cServerTime = google::protobuf::BigEndian::Load64(apData);
        const auto cWasSynchronized = GetClock().IsSynchronized();

        m_clock.Synchronize(cServerTime, cConnectionStatus.m_nPing);

        if (!cWasSynchronized)
            OnConnected();
    }
}
