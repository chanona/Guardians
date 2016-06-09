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
#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"

BEGIN(Engine)


class CHierarchyLoader;
class CAnimationCtrl;
class  CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& Instance);
	virtual ~CDynamicMesh(void);
public:
	void Set_AnimationSet(const _uint& iAniIdx);	
	void Move_Frame(const _float& fTimeDelta);
	_bool Check_EndPeriod(void);
	const _matrix* Get_FrameMatrix(const char* pFrameName);
public:
	virtual CResources* Clone_Resource(void);
	virtual HRESULT Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName);
	virtual void Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw = false);	
public:

	void Update_FrameMatrices(D3DXFRAME_DERIVED* pRootFrame, _matrix* pWorldMatrix);
	void SetUp_BoneMatrixPointer(D3DXFRAME_DERIVED* pRootFrame);
	void Render_MeshContainer(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pRootFrame);
public:
	static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pDevice
		, const TCHAR* pPath, const TCHAR* pFileName);
private:
	CHierarchyLoader*				m_pLoader;
	CAnimationCtrl*					m_pAniCtrl;
	LPD3DXFRAME						m_pRootFrame;
	
public:
	virtual _ulong Release(void);

};

END

#endif // StaticMesh_h__