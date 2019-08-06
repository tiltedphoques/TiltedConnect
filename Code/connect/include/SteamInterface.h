#pragma once
#include <atomic>
#include "steam/steamnetworkingsockets.h"

enum EPacketFlags
{
    kReliable,
    kUnreliable
};

enum EConnectOpcode : uint8_t
{
    kPayload = 0,
    kServerTime = 1
};

struct SteamInterface
{
    static void Acquire();
    static void Release();

private:

    static std::atomic<std::size_t> s_initCounter;
};

using ConnectionId_t = HSteamNetConnection;