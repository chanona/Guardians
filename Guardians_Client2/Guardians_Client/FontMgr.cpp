#include "stdafx.h"
#include "FontMgr.h"
#include "Font.h"

USING(Engine)
IMPLEMENT_SINGLETON(Engine::CFontMgr)

Engine::CFontMgr::CFontMgr(void)
{

}

Engine::CFontMgr::~CFontMgr(void)
{

}

HRESULT Engine::CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey , const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));
	if(iter != m_mapFont.end())
		return E_FAIL;

	CFont*		pFont = CFont::Create(pDevice, iWidth, iHeight, iWeight, pFontKey);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.insert(MAPFONT::value_type(pFontKey, pFont));
	return S_OK;
}

void Engine::CFontMgr::Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color)
{
	MAPFONT::iterator	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTagFinder(pFontKey));
	if(iter == m_mapFont.end())
		return;

	iter->second->Render(pString, Color, vPos);
}

void Engine::CFontMgr::Release(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CRelease_Pair());
	m_mapFont.clear();

	delete this;
}

