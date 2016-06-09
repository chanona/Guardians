#include "stdafx.h"
#include "Light.h"

USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pDevice)
: m_pGraphicDev(pDevice)
{
	ZeroMemory(&m_LightInfo, sizeof(D3DLIGHT9));
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Init_Light(const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx)
{
	memcpy(&m_LightInfo, pLightInfo, sizeof(D3DLIGHT9));
	
	return S_OK;
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pDevice , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx)
{
	CLight*		pLight = new CLight(pDevice);
	if(FAILED(pLight->Init_Light(pLightInfo, dwLightIdx)))
		Engine::Safe_Release(pLight);

	return pLight;
}

_ulong Engine::CLight::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
