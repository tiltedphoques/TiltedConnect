#pragma once
#include <atomic>

struct SteamInterface
{
    static void Acquire();
    static void Release();

private:

    static std::atomic<size_t> s_initCounter;
};
