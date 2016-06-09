#include "stdafx.h"
#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent(void)
{

}

Engine::CComponent::~CComponent(void)
{

}

_ulong Engine::CComponent::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		
	}

	return dwRefCnt;
}