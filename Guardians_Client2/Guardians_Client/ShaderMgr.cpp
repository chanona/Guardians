#include "stdafx.h"
#include "ShaderMgr.h"
#include "Shader.h"

USING(Engine)

IMPLEMENT_SINGLETON(CShaderMgr)

Engine::CShaderMgr::CShaderMgr(void)
{

}

Engine::CShaderMgr::~CShaderMgr(void)
{

}

LPD3DXEFFECT Engine::CShaderMgr::Get_EffectHandle(const TCHAR* pShaderTag)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if(NULL == pShader)
		return NULL;

	return pShader->Get_EffectHandle();
}

HRESULT Engine::CShaderMgr::Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	CShader*		pShader = Find_Shader(pShaderTag);

	if(NULL != pShader)
	{
		MSG_BOX("Already ShaderFile Failed");
		return E_FAIL;
	}

	pShader = CShader::Create(pGraphicDev, pFilePath);

	if(NULL == pShader)
		return E_FAIL;

	m_mapShaders.insert(MAPSHADERS::value_type(pShaderTag, pShader));
	
	return S_OK;
}

CShader* Engine::CShaderMgr::Find_Shader(const TCHAR* pShaderTag)
{
	MAPSHADERS::iterator	iter = find_if(m_mapShaders.begin(), m_mapShaders.end(), Engine::CTagFinder(pShaderTag));

	if(iter == m_mapShaders.end())
		return NULL;

	return iter->second;
}

void Engine::CShaderMgr::Release(void)
{
	for_each(m_mapShaders.begin(), m_mapShaders.end(), CRelease_Pair());
	m_mapShaders.clear();

	delete this;
}
