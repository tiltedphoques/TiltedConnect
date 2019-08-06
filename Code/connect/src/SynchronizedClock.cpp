#include "SynchronizedClock.h"

SynchronizedClock::SynchronizedClock()
    : m_lastServerTick{ 0 }
    , m_simulatedTick{ 0 }
    , m_lastSimulationTime{ std::chrono::high_resolution_clock::now() }
{}

uint64_t SynchronizedClock::GetCurrentTick() const
{
    return m_simulatedTick;
}

void SynchronizedClock::Synchronize(uint64_t aServerTick, uint32_t aPing)
{
    if (aServerTick <= m_lastServerTick)
        return;
        
    m_lastServerTick = aServerTick;

    const auto tripTime = aPing / 2;

    m_lastSimulationTime = std::chrono::high_resolution_clock::now();

    m_simulatedTick = m_lastServerTick + tripTime;
}

void SynchronizedClock::Reset(uint64_t aServerTick, uint32_t aPing)
{
    // Rebuild
    *this = SynchronizedClock{};
    Synchronize(aServerTick, aPing);
}

void SynchronizedClock::Update()
{
    const auto now = std::chrono::high_resolution_clock::now();
    const auto delta = now - m_lastSimulationTime;

    m_lastSimulationTime = now;

    m_simulatedTick += std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
}
