#include "SynchronizedClock.hpp"
#include <Math.hpp>

using namespace std::chrono_literals;

namespace TiltedPhoques
{
    SynchronizedClock::SynchronizedClock() noexcept
        : m_lastServerTick{ 0 }
        , m_simulatedTick{ 0 }
        , m_latestSimulatedTick{ 0 }
        , m_deltaTick{ 0 }
        , m_lastSimulationTime{ std::chrono::high_resolution_clock::now() }
        , m_lastSynchronizationTime{ std::chrono::high_resolution_clock::now() }
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

        m_lastSynchronizationTime = std::chrono::high_resolution_clock::now();

        if(IsSynchronized())
        {
            m_previousSimulatedTick = m_simulatedTick;
            m_latestSimulatedTick = m_lastServerTick + tripTime;
        }
        else
        {
            m_previousSimulatedTick = m_latestSimulatedTick = m_simulatedTick = m_lastServerTick + tripTime;
        }
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
        const std::chrono::duration<float> syncDelta = now - m_lastSynchronizationTime;

        m_lastSimulationTime = now;

        if (!IsSynchronized())
            return;

        const auto deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
        const auto syncDeltaSeconds = syncDelta.count();

        m_previousSimulatedTick += deltaMs;
        m_latestSimulatedTick += deltaMs;

        m_simulatedTick = Lerp(m_previousSimulatedTick, m_latestSimulatedTick, Min(1.0f, syncDeltaSeconds));
    }
}
