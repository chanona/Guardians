/*!
 * \file TerrainTex.h
 * \date 2015/08/17 20:49
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

#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class  CTerrainTex
	: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrainTex(void);
public:
	virtual CResources* Clone_Resource(void);

public:
	virtual HRESULT Create_Buffer(const _ushort& wCntX, const _ushort& wCntZ, const _ushort& wItv);
public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const _ushort& wCntX, const _ushort& wCntZ, const _ushort& wItv);
public:
	virtual _ulong Release(void);


};

END

#endif // TerrainTex_h__