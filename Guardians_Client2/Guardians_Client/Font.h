/*!
 * \file Font.h
 * \date 2015/08/14 13:35
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

#ifndef Font_h__
#define Font_h__

#include "Base.h"

BEGIN(Engine)

class  CFont : public Engine::CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CFont(void);
public:
	HRESULT Init_Font(const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName);
	void Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pDevice
		, const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName);
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXFONT				m_pFont;
	UINT					m_iHeight;
	UINT					m_iWidth;
public:
	virtual _ulong Release(void);
};

END

#endif // Font_h__