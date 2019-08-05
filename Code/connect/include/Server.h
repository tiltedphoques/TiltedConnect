#pragma once

#include "steam/steamnetworkingsockets.h"
#include <cstdint>
#include <chrono>
#include <vector>
#include "SteamInterface.h"

struct Server : private ISteamNetworkingSocketsCallbacks
{
    Server();
    virtual ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(Server&&) = delete;

    bool Host(uint16_t aPort, uint32_t aTickRate);
    void Close();

    void Update();

    virtual void OnUpdate() = 0;
    virtual void OnConsume(const void* apData, const uint32_t aSize, ConnectionId_t aConnectionId) = 0;
    virtual void OnConnection(ConnectionId_t aHandle) = 0;
    virtual void OnDisconnection(ConnectionId_t aConnectionId) = 0;

    void SendToAll(const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags = kReliable);
    void Send(ConnectionId_t aConnectionId, const void* apData, const uint32_t aSize, EPacketFlags aPacketFlags = kReliable) const;

    [[nodiscard]] uint16_t GetPort() const;

private:

    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo) override;

    HSteamListenSocket m_listenSock;
    ISteamNetworkingSockets* m_pInterface;

    std::vector<HSteamListenSocket> m_connections;

    uint32_t m_tickRate;
    std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;
    std::chrono::milliseconds m_timeBetweenUpdates;
};
