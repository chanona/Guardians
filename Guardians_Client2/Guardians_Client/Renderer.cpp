#include "stdafx.h"
#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "ShaderMgr.h"

USING(Engine)



Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pGraphicDev(pDevice)
{

}

Engine::CRenderer::~CRenderer(void)
{

}

void Engine::CRenderer::SetCurrentScene(CScene* pScene) {m_pScene = pScene;}

HRESULT Engine::CRenderer::Ready_Renderer(void)
{
	if(FAILED(CShaderMgr::GetInstance()->Ready_ShaderFromFiles(m_pGraphicDev, L"Shader_Default", L"Shader_Default.hpp")))
		return E_FAIL;

	if(FAILED(CShaderMgr::GetInstance()->Ready_ShaderFromFiles(m_pGraphicDev, L"Shader_Terrain", L"Shader_Terrain.hpp")))
		return E_FAIL;

	if(FAILED(CShaderMgr::GetInstance()->Ready_ShaderFromFiles(m_pGraphicDev, L"Shader_Meshes", L"Shader_Meshes.hpp")))
		return E_FAIL;

	if(FAILED(CShaderMgr::GetInstance()->Ready_ShaderFromFiles(m_pGraphicDev, L"Shader_SkyBox", L"Shader_SkyBox.hpp")))
		return E_FAIL;

	if(FAILED(CShaderMgr::GetInstance()->Ready_ShaderFromFiles(m_pGraphicDev, L"Shader_Effect", L"Shader_Effect.hpp")))
		return E_FAIL;

	return S_OK;
}

void Engine::CRenderer::Render(void)
{
	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	m_pGraphicDev->BeginScene();

	Render_Priority();
	Render_ZSort();
	Render_Alpha();
	Render_UI();

	if(m_pScene != NULL)
		m_pScene->Render();

	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, NULL, NULL);

	Clear_RenderGroup();	
}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer* pRenderer = new CRenderer(pDevice);
	if(FAILED(pRenderer->Ready_Renderer()))
	{
		Engine::Safe_Release(pRenderer);
	}
	return pRenderer;
}

void Engine::CRenderer::Render_Priority(void)
{


	RENDERLIST::iterator	iter = m_RenderGroup[RENDER_PRIORITY].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[RENDER_PRIORITY].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

}
void Engine::CRenderer::Render_ZSort(void)
{
	RENDERLIST::iterator	iter = m_RenderGroup[RENDER_ZSORT].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[RENDER_ZSORT].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}
void Engine::CRenderer::Render_Alpha(void)
{	
	MAPALPHA::iterator	iter = m_mapAlpha.begin();
	MAPALPHA::iterator	iter_end = m_mapAlpha.end();

	for (; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}
void Engine::CRenderer::Render_UI(void)
{
	RENDERLIST::iterator	iter = m_RenderGroup[RENDER_UI].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[RENDER_UI].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_int i = 0; i < RENDER_END; ++i)
	{	
		m_RenderGroup[i].clear();
	}

	m_mapAlpha.clear();
}

_ulong Engine::CRenderer::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}