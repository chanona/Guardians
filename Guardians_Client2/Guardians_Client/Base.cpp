#include "stdafx.h"
#include "Base.h"

Engine::CBase::CBase(void)
: m_dwRefCnt(0)
{

}

Engine::CBase::~CBase(void)
{

}

void Engine::CBase::Add_Ref(void)
{
	++m_dwRefCnt;
}

_ulong Engine::CBase::Release(void)
{
	if(0 == m_dwRefCnt)
		return m_dwRefCnt;

	return m_dwRefCnt--;	
}
