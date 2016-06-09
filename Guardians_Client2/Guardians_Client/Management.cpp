#include "stdafx.h"
#include "Management.h"
#include "Renderer.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pGraphicDev(NULL)
{

}

Engine::CManagement::~CManagement(void)
{

}

Engine::CComponent* Engine::CManagement::Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt)
{
	if(NULL == m_pScene)
		return NULL;

	return m_pScene->Get_Component(pComponentTag, pLayerTag, pObjectTag, iCnt);
}

HRESULT Engine::CManagement::Ready_Management(LPDIRECT3DDEVICE9 pDevice)
{
	m_pGraphicDev = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);

	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");

	return S_OK;
}

_int Engine::CManagement::Update(const _float& fTimeDelta)
{
	_int		iResult = 0;

	if(m_pScene != NULL)
		iResult = m_pScene->Update(fTimeDelta);

	return iResult;
}

void Engine::CManagement::Render(void)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render();
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Release(m_pRenderer);  
	Engine::Safe_Release(m_pScene);

	delete this;
}

