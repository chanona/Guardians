/*!
 * \file Mesh.h
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
#ifndef Mesh_h__
#define Mesh_h__

#include "Resources.h"

BEGIN(Engine)

class  CMesh
	: public CResources
{
public:
	enum MESHTYPE {TYPE_STATIC, TYPE_DYNAMIC, TYPE_END};
protected:
	explicit CMesh(LPDIRECT3DDEVICE9 pDevice);
	explicit CMesh(const CMesh& Instance);
	virtual ~CMesh(void);
public:
	void Get_MinMax(_vec3* pMin, _vec3* pMax) {
		*pMin = m_vMin; *pMax = m_vMax;}
public:
	virtual CResources* Clone_Resource(void)PURE;

public:
	virtual HRESULT Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName) PURE;
	virtual void Render_MeshForSDK(const D3DXMATRIX* pWorldMatrix) {}
protected:
	_vec3							m_vMin, m_vMax;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
public:
	virtual _ulong Release(void);
};

END

#endif // Mesh_h__