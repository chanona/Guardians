#include "stdafx.h"
#include "DynamicMesh.h"
#include "Loader.h"
#include "AnimationCtrl.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_pLoader(NULL)
, m_pRootFrame(NULL)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& Instance)
: CMesh(Instance)
, m_pLoader(Instance.m_pLoader)
, m_pRootFrame(Instance.m_pRootFrame)
{
	m_dwRefCnt = Instance.m_dwRefCnt;
	m_pAniCtrl = CAnimationCtrl::Clone(*(Instance.m_pAniCtrl));
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void Engine::CDynamicMesh::Set_AnimationSet(const _uint& iAniIdx)
{
	if(NULL == m_pAniCtrl)
		return;

	m_pAniCtrl->Set_AnimationSet(iAniIdx);
}

const _uint & CDynamicMesh::Get_AnimationSet(void)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_pAniCtrl->Get_AnimationSet();
}

void Engine::CDynamicMesh::Move_Frame(const _float& fTimeDelta)
{
	if(NULL == m_pAniCtrl)
		return;

	m_pAniCtrl->Move_Frame(fTimeDelta);	

	_matrix				matTmp;
	D3DXMatrixIdentity(&matTmp);
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTmp);
}

_bool Engine::CDynamicMesh::Check_EndPeriod(void)
{
	if(NULL == m_pAniCtrl)
		return false;

	return m_pAniCtrl->Check_EndPeriod();	
}

const _matrix* Engine::CDynamicMesh::Get_FrameMatrix(const char* pFrameName)
{
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);

	return &pFrame->CombinedTransformationMatrix;
}

Engine::CResources* Engine::CDynamicMesh::Clone_Resource(void)
{
	++(*m_pwRefCnt);

	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT		hr = NULL;
	TCHAR		szFullPath[128] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = new CHierarchyLoader(m_pGraphicDev, pPath);

	LPD3DXANIMATIONCONTROLLER			pAniCtrl = NULL;
	
	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, m_pLoader, NULL, &m_pRootFrame, &pAniCtrl)))
	{
		MSG_BOX("실패");
		return E_FAIL;	
	}

	m_pAniCtrl = CAnimationCtrl::Create(pAniCtrl);

	if(NULL == m_pAniCtrl)
		return E_FAIL;

	_matrix			matTmp;
	D3DXMatrixIdentity(&matTmp);

	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, &matTmp);

	SetUp_BoneMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	m_vMin = _vec3(-50.f, 0.f, -50.f);
	m_vMax = _vec3(50.f, 200.f, 50.f);

#ifdef _DEBUG
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8, 0, VTXFVF_CUBE, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	VTXCUBE*			pVtxCube = NULL; 
	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

	pVtxCube[1].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVtxCube[1].vTexUV = pVtxCube[1].vPos;	

	pVtxCube[2].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

	pVtxCube[3].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

	pVtxCube[4].vPos = D3DXVECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVtxCube[4].vTexUV = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

	pVtxCube[6].vPos = D3DXVECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

	pVtxCube[7].vPos = D3DXVECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVtxCube[7].vTexUV = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX16*			pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//+x
	pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;
	//-x
	pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;
	//+y
	pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;
	//-y
	pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;
	//+z
	pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;
	//-z
	pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

	m_pIB->Unlock();
#endif

	return S_OK;
}

// 뼈대들이 가지고 있는 CombineTransformationMatrix에 값을 채워준다.
void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, _matrix* pParentMatrix)
{
	if(NULL == pFrame)
		return;

	pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;

	if(NULL != pFrame->pFrameSibling)
	{
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);
	}

	if(NULL != pFrame->pFrameFirstChild)
	{
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

void Engine::CDynamicMesh::SetUp_BoneMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if(NULL == pFrame)
		return;

	if(NULL != pFrame->pMeshContainer
		&& NULL != pFrame->pMeshContainer->pSkinInfo)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;		

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			D3DXFRAME_DERIVED*		pBone = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pMeshContainer->pSkinInfo->GetBoneName(i));

			pMeshContainer->ppCombinedTransformationMatrices[i] = &pBone->CombinedTransformationMatrix;
		}
	}

	if(NULL != pFrame->pFrameSibling)
	{
		SetUp_BoneMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
	}

	if(NULL != pFrame->pFrameFirstChild)
	{
		SetUp_BoneMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
	}
}

void Engine::CDynamicMesh::Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw)
{
	Render_MeshContainer(pEffect, (D3DXFRAME_DERIVED*)m_pRootFrame);	

#ifdef _DEBUG
	if(true)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
		m_pGraphicDev->SetFVF(VTXFVF_CUBE);
		m_pGraphicDev->SetIndices(m_pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif
}

void Engine::CDynamicMesh::Render_MeshContainer(LPD3DXEFFECT pEffect, D3DXFRAME_DERIVED* pFrame)
{
	if(NULL != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;


		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrices[i] = pMeshContainer->pOffsetFrameMatrices[i] * *pMeshContainer->ppCombinedTransformationMatrices[i];
		}

		void			*pDest = NULL, *pSour = NULL;
		
		pMeshContainer->pOriMesh->LockVertexBuffer(0, (void**)&pDest);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (void**)&pSour);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrices, NULL, pDest, pSour);

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		pEffect->Begin(NULL, 0);
 		pEffect->BeginPass(0);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTextures[i]);

			pEffect->CommitChanges();
						
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}		

 		pEffect->EndPass();
		pEffect->End();

		pEffect->SetTexture("g_BaseTexture", NULL);
	}

	if(NULL != pFrame->pFrameSibling)
	{
		Render_MeshContainer(pEffect, (D3DXFRAME_DERIVED*)pFrame->pFrameSibling);
	}

	if(NULL != pFrame->pFrameFirstChild)
	{
		Render_MeshContainer(pEffect, (D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
	}
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice , const TCHAR* pPath, const TCHAR* pFileName)
{
	CDynamicMesh*		pMesh = new CDynamicMesh(pDevice);
	if(FAILED(pMesh->Load_MeshFromFile(pPath, pFileName)))
		Engine::Safe_Release(pMesh);

	return pMesh;
}

_ulong Engine::CDynamicMesh::Release(void)
{
	_ulong	dwRefCnt = Engine::CMesh::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{
			m_pLoader->DestroyFrame(m_pRootFrame);
			Engine::Safe_Delete(m_pLoader);			
			if(Engine::Safe_Release(m_pVB))
				MSG_BOX("m_pVB Release Failed");
			if(Engine::Safe_Release(m_pIB))
				MSG_BOX("m_pIB Release Failed");
		}

		Engine::Safe_Release(m_pAniCtrl);
		delete this;
	}	
	return dwRefCnt;
}

