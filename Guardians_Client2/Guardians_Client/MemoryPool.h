#pragma once
#include "stdafx.h"
#include "Memory.h"

template <class T, int ALLOC_BLOCK_COUNT = 100>
class CMemoryPool
{
public:
	CMemoryPool()
	{

	}
	~CMemoryPool()
	{
	}

	static void* operator new(size_t allocLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR *));

	if (!m_pFreeList)
		AllocBlock();

	UCHAR *pReturn = m_pFreeList;

	m_pFreeList = *reinterpret_cast<UCHAR **>(pReturn);

	return pReturn;
	}

		static void operator delete(void *pDel)
	{

		*reinterpret_cast<UCHAR **>(pDel) = m_pFreeList;

		m_pFreeList = static_cast<UCHAR *>(pDel);
	}

	static void AllocBlock()
	{
		m_pFreeList = new UCHAR[sizeof(T) * ALLOC_BLOCK_COUNT];

		UCHAR **ppCurrent = reinterpret_cast<UCHAR **>(m_pFreeList);
		UCHAR *pNext = m_pFreeList;

		for (int i = 0; i < ALLOC_BLOCK_COUNT - 1; ++i)
		{
			pNext += sizeof(T);
			*ppCurrent = pNext;
			ppCurrent = reinterpret_cast<UCHAR **>(pNext);
		}

		*ppCurrent = nullptr;
	}
private:
	static UCHAR *m_pFreeList;
};

template <class T, int ALLOC_BLOCK_COUNT>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_COUNT>::m_pFreeList;
