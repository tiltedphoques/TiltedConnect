#pragma once

#include <cstdint>
#include <chrono>

namespace TiltedPhoques
{
	struct SynchronizedClock
	{
		SynchronizedClock() noexcept;
		[[nodiscard]] uint64_t GetCurrentTick() const noexcept;
		void Synchronize(uint64_t aServerTick, uint32_t aPing) noexcept;
		void Reset(uint64_t aServerTick, uint32_t aPing) noexcept;
		void Update() noexcept;

	private:

		uint64_t m_lastServerTick;
		uint64_t m_simulatedTick;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSimulationTime{};
	};
}