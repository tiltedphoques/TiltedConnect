#pragma once

#include "steam/steamnetworkingsockets.h"
#include <cstdint>
#include <chrono>
#include <string>
#include "SteamInterface.h"

struct Client : private ISteamNetworkingSocketsCallbacks
{
    enum EDisconnectReason
    {
        kTimeout,
        kLocalProblem,
        kKicked
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

    virtual void OnConsume(const void* apData, const uint32_t aSize) = 0;
    virtual void OnConnected() = 0;
    virtual void OnDisconnected(EDisconnectReason aReason) = 0;

    void Send(const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags = kReliable) const;

private:

    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo) override;

    HSteamNetConnection m_connection;
    ISteamNetworkingSockets* m_pInterface;
};
