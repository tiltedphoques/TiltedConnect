#pragma once

#include <cstdint>

namespace TiltedPhoques
{
    struct SynchronizedClock
    {
        SynchronizedClock() noexcept;
        [[nodiscard]] uint64_t GetCurrentTick() const noexcept;
        [[nodiscard]] bool IsSynchronized() const noexcept;
        // aMessageAge is how long ago in ms the message carrying aServerTick arrived
        void Synchronize(uint64_t aServerTick, uint32_t aPing, uint32_t aMessageAge = 0) noexcept;
        void Reset() noexcept;
        void Update() noexcept;

    private:

        uint64_t m_lastServerTick;
        uint64_t m_simulatedTick;
        std::chrono::nanoseconds m_previousSimulatedTick;
        std::chrono::nanoseconds m_tickDelta;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSynchronizationTime{};
    };
}
