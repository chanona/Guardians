/*!
 * \file FontMgr.h
 * \date 2015/08/14 13:50
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFont;
class  CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)
private:
	explicit CFontMgr(void);
	~CFontMgr(void);
public:
	HRESULT Ready_Font(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFontKey
		, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);
private:
	typedef map<const TCHAR*, CFont*>		MAPFONT;
	MAPFONT		m_mapFont;
public:
	void Release(void);
};

END

#endif // FontMgr_h__