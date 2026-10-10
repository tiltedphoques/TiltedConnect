#pragma once

#include "steam/steamnetworkingsockets.h"

namespace TiltedPhoques
{
    enum EPacketFlags
    {
        kReliable,
        kUnreliable,
        // Skips Nagle's algorithm: sent right away, flushing anything queued before it
        kUnreliableNoNagle
    };

    [[nodiscard]] inline int ToSteamSendFlags(const EPacketFlags acPacketFlags) noexcept
    {
        switch (acPacketFlags)
        {
        case kUnreliable: return k_nSteamNetworkingSend_Unreliable;
        case kUnreliableNoNagle: return k_nSteamNetworkingSend_UnreliableNoNagle;
        default: return k_nSteamNetworkingSend_Reliable;
        }
    }

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
