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

		// Get the start of the write buffer
		[[nodiscard]] char* GetData() const noexcept;

		// Get the writable data size, note that the actual packet size may differ from that
		[[nodiscard]] size_t GetSize() const noexcept;

		// Get the writable data size + protocol data size
		[[nodiscard]] size_t GetTotalSize() const noexcept;

		// Returns true if the packet has an associated buffer, usually used to check if the underlying allocator had enough space
		[[nodiscard]] bool IsValid() const noexcept;

	private:

		friend struct Server;
		friend struct Client;

		char* m_pData;
		size_t m_size;
	};
}