#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}

Engine::CGameObject::~CGameObject(void)
{

}

Engine::CComponent*	Engine::CGameObject::Get_Component(const TCHAR* pComponentTag)
{
	Engine::CComponent*	pComponent = Find_Component(pComponentTag);

	if(NULL == pComponent)
		return NULL;

	return pComponent;	
}

_int Engine::CGameObject::Update(const _float& fTimeDelta)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.begin();
	MAPCOMPONENT::iterator	iter_end = m_mapComponent.end();

	_int		iResult = 0;

	for(; iter != iter_end ; ++iter)
	{
		iResult = iter->second->Update(fTimeDelta);

		if (iResult & 0x80000000)
		{
			return iResult;
		}
	}

	return iResult;
}

CComponent* Engine::CGameObject::Find_Component(const TCHAR* pComTag)
{
	MAPCOMPONENT::iterator	iter = find_if(m_mapComponent.begin(), m_mapComponent.end(), CTagFinder(pComTag));

	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

_ulong Engine::CGameObject::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		for_each(m_mapComponent.begin(), m_mapComponent.end(), CRelease_Pair());
		m_mapComponent.clear();
	}
	return dwRefCnt;	
}

