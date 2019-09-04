#pragma once

#include "Meta.h"
#include "Allocator.h"

struct Packet
{
	TP_ALLOCATOR;

	Packet(size_t aSize);
	~Packet();

	TP_NOCOPYMOVE(Packet);

	[[nodiscard]] char* GetData() const;
	[[nodiscard]] size_t GetSize() const;

private:

	friend struct Server;
	friend struct Client;

	char* m_pData;
	size_t m_size;
};
