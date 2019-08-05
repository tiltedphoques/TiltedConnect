#pragma once
#include <atomic>
#include "steam/steamnetworkingsockets.h"

enum EPacketFlags
{
    kReliable,
    kUnreliable
};

struct SteamInterface
{
    static void Acquire();
    static void Release();

private:

    static std::atomic<std::size_t> s_initCounter;
};

using ConnectionId_t = HSteamNetConnection;