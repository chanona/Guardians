#include "stdafx.h"
#include "LightMgr.h"
#include "Light.h"

USING(Engine)
IMPLEMENT_SINGLETON(Engine::CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{

}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _ulong& dwLightIdx)
{
	LIGHTLIST::iterator	iter = m_LightList.begin();	

	for (_ulong i = 0; i < dwLightIdx; ++i)
	{
		++iter;
	}

	return (*iter)->Get_LightInfo();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx)
{
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	CLight*		pLight = CLight::Create(pGraphicDev, pLightInfo, dwLightIdx);
	NULL_CHECK_RETURN(pLight, E_FAIL);

	m_LightList.push_back(pLight);

	return S_OK;
}

void Engine::CLightMgr::Release(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), CRelease_Single());
	m_LightList.clear();

	delete this;
}


