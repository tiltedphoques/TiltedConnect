#pragma once

#include "steam/steamnetworkingsockets.h"
#include <cstdint>
#include <chrono>
#include <vector>

struct Server : private ISteamNetworkingSocketsCallbacks
{
    Server();
    virtual ~Server();

    bool Host(uint16_t aPort, uint32_t aTickRate);
    void Close();

    void Update();

    virtual void OnUpdate() = 0;
    virtual void OnConsume(const void* apData, const uint32_t aSize, void* aCookie) = 0;
    virtual void* OnConnection(uint32_t aHandle) = 0;
    virtual void OnDisconnection(void* aCookie) = 0;

    void SendToAll(const void* apData, const uint32_t aSize);

private:

    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* apInfo) override;

    HSteamListenSocket m_listenSock;
    ISteamNetworkingSockets* m_pInterface;

    std::vector<HSteamListenSocket> m_connections;

    uint32_t m_tickRate;
    std::chrono::time_point<std::chrono::steady_clock> m_lastUpdateTime;
    std::chrono::milliseconds m_timeBetweenUpdates;
};
