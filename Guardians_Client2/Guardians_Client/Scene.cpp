#include "stdafx.h"
#include "Scene.h"
#include "Layer.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pGraphicDev(pDevice)
{

}

Engine::CScene::~CScene(void)
{
	
}

Engine::CComponent* Engine::CScene::Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt)
{
	CLayer*	pLayer = Find_Layer(pLayerTag);

	if(NULL == pLayer)
		return NULL;

	return pLayer->Get_Component(pComponentTag, pObjectTag, iCnt);	
}

list<Engine::CGameObject*>* CScene::Find_ObjectList(const TCHAR * pLayerTag, const TCHAR * pObjectTag)
{
	CLayer*	pLayer = Find_Layer(pLayerTag);

	if (NULL == pLayer)
		return NULL;

	return pLayer->Find_ObjectList(pObjectTag);
}

void CScene::Add_Object(const TCHAR * pLayerTag, const TCHAR * pObjectTag, CGameObject * pGameObject)
{
	CLayer*	pLayer = Find_Layer(pLayerTag);

	if (NULL == pLayer)
		return;

	pLayer->Ready_Object(pObjectTag, pGameObject);
}

_int Engine::CScene::Update(const _float& fTimeDelta)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	_int		iResult = 0;

	for (; iter != iter_end; ++iter)
	{
		iResult = iter->second->Update(fTimeDelta);

		if(iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void Engine::CScene::Render(void)
{
	MAPLAYER::iterator	iter = m_mapLayer.begin();
	MAPLAYER::iterator	iter_end = m_mapLayer.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}

Engine::CLayer* Engine::CScene::Find_Layer(const TCHAR* pLayerTag)
{
	MAPLAYER::iterator	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTagFinder(pLayerTag));

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second;
}

_ulong Engine::CScene::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		for_each(m_mapLayer.begin(), m_mapLayer.end(), CRelease_Pair());
		m_mapLayer.clear();		
	}

	return dwRefCnt;
}

	

