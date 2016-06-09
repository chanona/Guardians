#include "stdafx.h"
#include "Resources.h"

USING(Engine)

Engine::CResources::CResources(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pwRefCnt(new _ulong(0))
, m_pGraphicDev(pGraphicDev)
{

}

Engine::CResources::CResources(const CResources& Instance)
: m_pGraphicDev(Instance.m_pGraphicDev)
, m_pwRefCnt(Instance.m_pwRefCnt)
{
	
}

Engine::CResources::~CResources(void)
{

}

_ulong Engine::CResources::Release(void)
{
	_ulong	dwRefCnt = Engine::CComponent::Release();

	if(0 == dwRefCnt) // ��ü�� �ּҸ� �����ϰ��ִ� ���������� ����
	{
		if(0 == (*m_pwRefCnt)) // ������ü�� ����� Ƚ��
		{
			Engine::Safe_Delete(m_pwRefCnt);	
		}		
		else
			--(*m_pwRefCnt);	
	}

	return dwRefCnt;
}