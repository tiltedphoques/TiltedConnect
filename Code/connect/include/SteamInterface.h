#pragma once
#include <atomic>

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
