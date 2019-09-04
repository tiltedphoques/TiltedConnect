#pragma once

#include "steam/steamnetworkingsockets.h"
#include <cstdint>
#include <string>
#include "SteamInterface.h"
#include "SynchronizedClock.h"

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

    Client();
    virtual ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    Client(Client&&) noexcept;
    Client& operator=(Client&&) noexcept;

    bool Connect(const std::string& acEndpoint);
    void Close();

    void Update();

    virtual void OnConsume(const void* apData, uint32_t aSize) = 0;
    virtual void OnConnected() = 0;
    virtual void OnDisconnected(EDisconnectReason aReason) = 0;
    virtual void OnUpdate() = 0;

    void Send(Packet* apPacket, EPacketFlags aPacketFlags = kReliable) const;

    [[nodiscard]] bool IsConnected() const;
    [[nodiscard]] SteamNetworkingQuickConnectionStatus GetConnectionStatus() const;
    [[nodiscard]] const SynchronizedClock& GetClock() const;

private:

    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo) override;

    void HandleMessage(const void* apData, uint32_t aSize);
    void HandleServerTime(const void* apData, uint32_t aSize);

    HSteamNetConnection m_connection;
    ISteamNetworkingSockets* m_pInterface;
    SynchronizedClock m_clock;
};
