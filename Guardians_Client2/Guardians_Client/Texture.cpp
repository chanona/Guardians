#include "stdafx.h"
#include "Texture.h"

USING(Engine)

Engine::CTexture::CTexture(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
{

}

Engine::CTexture::~CTexture(void)
{

}

Engine::CResources* Engine::CTexture::Clone_Resource(void)
{
	++(*m_pwRefCnt);

	return new CTexture(*this);
}

HRESULT Engine::CTexture::Load_Texture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	IDirect3DBaseTexture9*		pTexture = NULL;

	if(wCnt == 0)
		return E_FAIL;

	m_vecTexture.reserve(wCnt);

	TCHAR		szFullPath[MAX_PATH] = L"";

	for(size_t i = 0; i < wCnt; ++i)
	{
		wsprintf(szFullPath, pFilePath, i);

		HRESULT		hr = NULL;
		switch(eTextureType)
		{
		case TYPE_NORMAL:
			hr = D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, (LPDIRECT3DTEXTURE9*)&pTexture);
			break;

		case TYPE_CUBE:
			hr = D3DXCreateCubeTextureFromFile(m_pGraphicDev, szFullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture);
			break;
		}
		FAILED_CHECK_MSG(hr, szFullPath);

		m_vecTexture.push_back(pTexture);
	}

	m_dwContainerSize = m_vecTexture.size();
	return S_OK;
}

void Engine::CTexture::Render(const DWORD& dwStage, const DWORD& iIndex)
{
	if(iIndex >= m_dwContainerSize)
		return;

	m_pGraphicDev->SetTexture(dwStage, m_vecTexture[iIndex]);
}

Engine::CTexture* Engine::CTexture::Create(LPDIRECT3DDEVICE9 pDevice , TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt)
{
	CTexture*	pTexture = new CTexture(pDevice);
	if(FAILED(pTexture->Load_Texture(eTextureType, pFilePath, wCnt)))
	{
		MSG_BOX("pFilePath");
		Engine::Safe_Release(pTexture);
	}
	return pTexture;
}

_ulong Engine::CTexture::Release(void)
{
	_ulong dwRefCnt = Engine::CResources::Release();

	if(0 == dwRefCnt)
	{
		if( NULL == m_pwRefCnt )
		{
			DWORD		dwSize = m_vecTexture.size();
			for(DWORD i = 0; i < dwSize; ++i)
			{
				if(Engine::Safe_Release(m_vecTexture[i]))
					MSG_BOX("Texture Release Failed");
			}
			m_vecTexture.clear();		
		}	
		delete this;
	}
	return dwRefCnt;
}

