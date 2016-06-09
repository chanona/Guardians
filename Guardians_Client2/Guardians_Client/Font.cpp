#include "stdafx.h"
#include "Font.h"

USING(Engine)

Engine::CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

Engine::CFont::~CFont(void)
{
	
}

HRESULT Engine::CFont::Init_Font(const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName)
{
	D3DXFONT_DESC		Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.Width = m_iWidth = iWidth;
	Font_Desc.Height = m_iHeight = iHeight;
	Font_Desc.Weight = iWeight;
	lstrcpy(Font_Desc.FaceName, pFaceName);
	Font_Desc.CharSet = HANGUL_CHARSET;

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont));
	return S_OK;
}

void Engine::CFont::Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos)
{
	RECT	rc = {(long)vPos.x, (long)vPos.y, (long)vPos.x + m_iWidth * lstrlen(pString), (long)vPos.y + m_iHeight};

	m_pFont->DrawText(NULL, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pDevice , const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName)
{
	CFont*		pFont = new CFont(pDevice);
	if(FAILED(pFont->Init_Font(iWidth, iHeight, iWeight, pFaceName)))
		Engine::Safe_Release(pFont);

	return pFont;
}

_ulong Engine::CFont::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		Engine::Safe_Release(m_pFont);
		delete this;
	}
	return dwRefCnt;
}

