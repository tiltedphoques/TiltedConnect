#pragma once

#include "steam/steamnetworkingsockets.h"
#include <cstdint>
#include <string>
#include "SteamInterface.hpp"
#include "SynchronizedClock.hpp"

namespace TiltedPhoques
{
    struct Packet;
    struct Client : private ISteamNetworkingSocketsCallbacks
    {
        enum EDisconnectReason
        {
            kTimeout,
            kLocalProblem,
            kKicked,
            kNormal
        };

        Client() noexcept;
        virtual ~Client();

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;

        Client(Client&&) noexcept;
        Client& operator=(Client&&) noexcept;

        bool Connect(const std::string& acEndpoint) noexcept;
        void Close() noexcept;

        void Update() noexcept;

        virtual void OnConsume(const void* apData, uint32_t aSize) = 0;
        virtual void OnConnected() = 0;
        virtual void OnDisconnected(EDisconnectReason aReason) = 0;
        virtual void OnUpdate() = 0;

        void Send(Packet* apPacket, EPacketFlags aPacketFlags = kReliable) const noexcept;

        [[nodiscard]] bool IsConnected() const noexcept;
        [[nodiscard]] SteamNetworkingQuickConnectionStatus GetConnectionStatus() const noexcept;
        [[nodiscard]] const SynchronizedClock& GetClock() const noexcept;

    private:

        void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo) override;

        void HandleMessage(const void* apData, uint32_t aSize) noexcept;
        void HandleServerTime(const void* apData, uint32_t aSize) noexcept;

        HSteamNetConnection m_connection;
        ISteamNetworkingSockets* m_pInterface;
        SynchronizedClock m_clock;
    };
}
