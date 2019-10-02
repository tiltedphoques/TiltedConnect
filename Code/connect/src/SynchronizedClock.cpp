#include "SynchronizedClock.hpp"

namespace TiltedPhoques
{
    SynchronizedClock::SynchronizedClock() noexcept
        : m_lastServerTick{ 0 }
        , m_simulatedTick{ 0 }
        , m_lastSimulationTime{ std::chrono::high_resolution_clock::now() }
    {}

    uint64_t SynchronizedClock::GetCurrentTick() const noexcept
    {
        return m_simulatedTick;
    }

    bool SynchronizedClock::IsSynchronized() const noexcept
    {
        return m_simulatedTick != 0;
    }

    void SynchronizedClock::Synchronize(uint64_t aServerTick, uint32_t aPing) noexcept
    {
        if (aServerTick <= m_lastServerTick)
            return;

        m_lastServerTick = aServerTick;

        const auto tripTime = aPing / 2;

        m_lastSimulationTime = std::chrono::high_resolution_clock::now();

        m_simulatedTick = m_lastServerTick + tripTime;
    }

    void SynchronizedClock::Reset() noexcept
    {
        // Rebuild
        *this = SynchronizedClock{};
    }

    void SynchronizedClock::Update() noexcept
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = now - m_lastSimulationTime;

        m_lastSimulationTime = now;

        if (m_simulatedTick == 0)
            return;

        m_simulatedTick += std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
    }
}
