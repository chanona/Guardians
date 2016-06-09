#include "stdafx.h"
#include "Shader.h"	

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Init_Shader(const TCHAR* pFilePath)
{	
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, &m_pErrBuffer)))
	{
		if(NULL == m_pErrBuffer)
		{
			MSG_BOX("ShaderFile Path Failed");
			return E_FAIL;
		}
		MessageBoxA(NULL, (char*)m_pErrBuffer->GetBufferPointer(), "Shader Compiled Error", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}


CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath)
{
	CShader*		pShader = new CShader(pGraphicDev);

	if(FAILED(pShader->Init_Shader(pFilePath)))
	{
		MSG_BOX("CShader Create Failed");
		Engine::Safe_Release(pShader);		
	}
	return pShader;
}

_ulong Engine::CShader::Release(void)
{
	_ulong		dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		if(Engine::Safe_Release(m_pErrBuffer))
			MSG_BOX("m_pErrBuffer Release Failed");
		if(Engine::Safe_Release(m_pEffect))
			MSG_BOX("m_pEffect Release Failed");
		delete this;
	}

	return dwRefCnt;

}
