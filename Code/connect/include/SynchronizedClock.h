#pragma once

#include <cstdint>
#include <chrono>

struct SynchronizedClock
{
    SynchronizedClock();
    [[nodiscard]] uint64_t GetCurrentTick() const;
    void Synchronize(uint64_t aServerTick, uint32_t aPing);
    void Reset(uint64_t aServerTick, uint32_t aPing);
    void Update();

private:

    uint64_t m_lastServerTick;
    uint64_t m_simulatedTick;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSimulationTime{};
};
