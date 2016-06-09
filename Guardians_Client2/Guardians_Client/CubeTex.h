/*!
 * \file CubeTex.h
 * \date 2015/05/18 20:45
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
#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class  CCubeTex
	: public CVIBuffer
{
private:
	CCubeTex(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CCubeTex(void);
public:
	virtual CResources* Clone_Resource(void);

public:
	virtual HRESULT Create_Buffer(void);

public:
	static CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual _ulong Release(void);
};

END

#endif // CubeTex_h__