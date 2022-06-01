#pragma once

#include "steam/steamnetworkingsockets.h"

namespace TiltedPhoques
{
    enum EPacketFlags
    {
        kReliable,
        kUnreliable
    };

    enum EConnectOpcode : uint8_t
    {
        kPayload = 0,
        kServerTime = 1,
        kCompressedPayload = 2
    };

    struct SteamInterface
    {
        static void Acquire();
        static void Release();
    };

    using ConnectionId_t = HSteamNetConnection;
}
