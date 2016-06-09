/*!
 * \file LightMgr.h
 * \date 2015/08/14 12:49
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

#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CLight;
class  CLightMgr
{
public:
	DECLARE_SINGLETON(CLightMgr)
private:
	explicit CLightMgr(void);
	~CLightMgr(void);
public:
	const D3DLIGHT9* Get_LightInfo(const _ulong& dwLightIdx);
public:
	HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);
private:
	list<CLight*>				m_LightList;
	typedef list<CLight*>		LIGHTLIST;	
public:
	void Release(void);
};

END

#endif // LightMgr_h__