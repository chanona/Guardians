/*!
 * \file StaticMesh.h
 * \date 2015/08/24 12:22
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
#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class  CStaticMesh
	: public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticMesh(void);

public:
	virtual CResources* Clone_Resource(void);
	virtual HRESULT Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Render_MeshForSDK(const D3DXMATRIX* pWorldMatrix);
	virtual void Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw = false);

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pDevice
		, const TCHAR* pPath, const TCHAR* pFileName);
private:
	LPDIRECT3DTEXTURE9*		m_ppTexture;
	D3DMATERIAL9*			m_pMtrl;
private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjancey; // 인접폴리곤에 대한 정보다.
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;



public:
	virtual _ulong Release(void);

};

END

#endif // StaticMesh_h__