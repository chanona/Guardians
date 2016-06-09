/*!
 * \file RcTex.h
 * \date 2015/04/04 23:00
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
#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class  CRcTex
	: public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CRcTex(void);
public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void);
public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);	
public:
	virtual _ulong Release(void);
};

END

#endif // RcTex_h__