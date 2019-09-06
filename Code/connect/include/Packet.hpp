#pragma once

#include "Meta.hpp"
#include "Allocator.hpp"

namespace TiltedPhoques
{
	struct Packet
	{
		TP_ALLOCATOR;

		Packet(size_t aSize) noexcept;
		~Packet() noexcept;

		TP_NOCOPYMOVE(Packet);

		[[nodiscard]] char* GetData() const noexcept;
		[[nodiscard]] size_t GetSize() const noexcept;

	private:

		friend struct Server;
		friend struct Client;

		char* m_pData;
		size_t m_size;
	};
}