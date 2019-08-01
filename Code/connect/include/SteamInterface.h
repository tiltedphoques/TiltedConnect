#pragma once
#include <atomic>

struct SteamInterface
{
    static void Acquire();
    static void Release();

private:

    static std::atomic<std::size_t> s_initCounter;
};
