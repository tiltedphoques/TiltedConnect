#include "Packet.h"
#include "SteamInterface.h"

Packet::Packet(const size_t aSize)
	: m_pData(nullptr)
	, m_size(aSize + 1)
{
	m_pData = static_cast<char*>(GetAllocator()->Allocate(m_size));
	m_pData[0] = kPayload;
}

Packet::~Packet()
{
	GetAllocator()->Free(m_pData);
}

char* Packet::GetData() const
{
	return m_pData + 1;
}

size_t Packet::GetSize() const
{
	return m_size - 1;
}
