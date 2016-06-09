/*!
 * \file VIBuffer.h
 * \date 2015/04/04 22:28
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
#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class  CVIBuffer
	: public CResources
{
public:
	enum BUFFERTYPE {TYPE_RCTEX, TYPE_TERRAIN, TYPE_CUBE, TYPE_END};	
protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVIBuffer(void);
public:
	void Get_VtxInfo(void* pVtxInfo);
public:
	void Set_VtxInfo(void* pVtxInfo);
	void Set_IdxInfo(void* pInxInfo, const _ulong* pTriCnt);

public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void)PURE;
public:
	void Render(const D3DXMATRIX* pmatWorld);


protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	_ulong						m_dwVtxSize;
	_ulong						m_dwVtxCnt;
	_ulong						m_dwVtxFVF;

	LPDIRECT3DINDEXBUFFER9		m_pIB;
	_ulong						m_dwIdxSize;
	_ulong						m_dwTriCnt;
	D3DFORMAT					m_IdxFmt;
public:
	virtual _ulong Release(void);

};

END

#endif // VIBuffer_h__