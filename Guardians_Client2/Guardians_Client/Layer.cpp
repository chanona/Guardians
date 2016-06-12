#include "stdafx.h"
#include "Layer.h"
#include "GameObject.h"

USING(Engine)

Engine::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pGraphicDev(pDevice)
{

}

Engine::CLayer::~CLayer(void)
{

}

Engine::CComponent* Engine::CLayer::Get_Component(const TCHAR* pComponentTag, const TCHAR* pObjectTag, const _uint& iCnt)
{
	list<CGameObject*>*	pObjectList = Find_ObjectList(pObjectTag);

	if(NULL == pObjectList) // 없었다.
		return NULL;

	list<CGameObject*>::iterator	iter = pObjectList->begin();

	for (_uint i = 0; i < iCnt; ++i)
	{
		++iter;
	}

	return (*iter)->Get_Component(pComponentTag);	
}

HRESULT Engine::CLayer::Ready_Object(const TCHAR* pObjectTag, CGameObject* pGameObject)
{
	list<CGameObject*>*	pObjectList = Find_ObjectList(pObjectTag);

	if(NULL == pObjectList) // 없었다.
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_mapObjlist.insert(map<const TCHAR*, list<CGameObject*>>::value_type(pObjectTag, NewObjectList));
	}

	else	// 있었다.
	{
		pObjectList->push_back(pGameObject);
	}

	
	return S_OK;
}

_int Engine::CLayer::Update(const _float& fTimeDelta)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	_int			iResult = 0;

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end;)		
		{
			iResult = (*iterList)->Update(fTimeDelta);

			if (iResult & 0x80000000)
			{
				::Safe_Release(*iterList);
				iterList = iter->second.erase(iterList);
			}

			else
				++iterList;
		}		
	}	

	return iResult;
}

void Engine::CLayer::Render(void)
{
	MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
	MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

	for (; iter != iter_end; ++iter)
	{
		OBJECTLIST::iterator	iterList = iter->second.begin();
		OBJECTLIST::iterator	iterList_end = iter->second.end();

		for (; iterList != iterList_end; ++iterList)		
		{
			(*iterList)->Render();
		}		
	}	
}

Engine::CLayer* Engine::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLayer*		pLayer = new CLayer(pDevice);

	return pLayer;
}

list<CGameObject*>* Engine::CLayer::Find_ObjectList(const TCHAR* pObjectTag)
{
	map<const TCHAR*, list<CGameObject*>>::iterator	iter = find_if(m_mapObjlist.begin(), m_mapObjlist.end(), Engine::CTagFinder(pObjectTag));

	if(iter == m_mapObjlist.end())
		return NULL;

	return &iter->second;
}

_ulong Engine::CLayer::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		MAPOBJLIST::iterator	iter = m_mapObjlist.begin();
		MAPOBJLIST::iterator	iter_end = m_mapObjlist.end();

		for (; iter != iter_end; ++iter)
		{
			OBJECTLIST::iterator	iterList = iter->second.begin();
			OBJECTLIST::iterator	iterList_end = iter->second.end();

			for (; iterList != iterList_end; ++iterList)		
				Engine::Safe_Release((*iterList));

			iter->second.clear();
		}
		m_mapObjlist.clear();	

		delete this;
	}

	return dwRefCnt;	
}