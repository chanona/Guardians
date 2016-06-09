/*!
 * \file Resources.h
 * \date 2015/04/03 18:18
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
#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(Engine)

class  CResources
	: public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);
	explicit CResources(const CResources& Instance);
	virtual ~CResources(void);
public:
	virtual CResources* Clone_Resource(void) PURE;
protected:
	_ulong*		m_pwRefCnt;
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
public:
	virtual _ulong Release(void);
};

END

#endif // Resources_h__