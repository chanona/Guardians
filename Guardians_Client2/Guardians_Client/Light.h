/*!
 * \file Light.h
 * \date 2015/08/14 12:41
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

#ifndef Light_h__
#define Light_h__

#include "Base.h"

BEGIN(Engine)

class  CLight : public Engine::CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);
public:
	const D3DLIGHT9* Get_LightInfo(void) {
		return &m_LightInfo;}
public:
	HRESULT Init_Light(const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_LightInfo;
public:
	virtual _ulong Release(void);
};

END

#endif // Light_h__