#include "stdafx.h"
#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice)
, m_dwSubsetCnt(0)
, m_pSubset(NULL)
, m_ppTexture(NULL)
, m_pMtrl(NULL)
, m_pMesh(NULL)
{

}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

Engine::CResources* Engine::CStaticMesh::Clone_Resource(void)
{
	++(*m_pwRefCnt);

	return new CStaticMesh(*this);
}

HRESULT Engine::CStaticMesh::Load_MeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT		hr = NULL;
	TCHAR		szFullPath[128] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	if(FAILED(D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pGraphicDev, &m_pAdjancey, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pMesh)))
		return E_FAIL;

	LPD3DXMESH			pMesh = m_pMesh;

	// �޽��� ������ ��ֿ� ���� ������ ���� ���.
	_ulong dwFVF = pMesh->GetFVF(); // �޽��� ������ ������ �ִ� fvf������ ����.

	if(false == (dwFVF & D3DFVF_NORMAL))
	{
		// �޽ø� �����Ѵ�.(������ ������ �����ؼ�)
		if(FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh)))
			return E_FAIL;

		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjancey->GetBufferPointer());
	}

	// �޽��� min, max������ ������ �˾ƺ���
	
	// ������ �̷���ִ� FVF�ϳ��� ����
	D3DVERTEXELEMENT9				Element[MAX_FVF_DECL_SIZE];
	ZeroMemory(Element, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE); // ���ʱ�ȭ�ϰ�

	// ������ �������̿��������� �� ���� �迭�� �־�
	m_pMesh->GetDeclaration(Element);

	_ushort			wPositionOffset = 0;

	for (_int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{		
		// �� �������̿��������߿� ��ġ�� ã��
		if(Element[i].Usage == D3DDECLUSAGE_POSITION)
		{
			// �� ��ġ�� �ش��ϴ� �������̿����� �޸� ���� byte�� ����
			wPositionOffset = Element[i].Offset;
			break;
		}
	}

	// ù��° ������ ��ġ������ ������ ����
	_vec3*			pFirstVertexPosition;

	// �޽ø� �̷���ִ� ������(�迭)�� ���� ���ּҸ� ����
	_byte*			pVertex = NULL;

	// �޽ø� ��!! ��? ������ ������ ���÷���
	m_pMesh->LockVertexBuffer(0, (void**)&pVertex);

	// pVertex + wPositionOffset : �޽ø� �̷���ִ� ������(�迭)�� ���� ���ּҿ��ٰ� wPositionOffset�� ���ϸ� ��ġ������ ����ִ� ������ �ּ�
	pFirstVertexPosition = ((_vec3*)(pVertex + wPositionOffset));	

	// �޽ø� �̷���ִ� ������ ��ü ������ ���´�.
	_ulong dwNumVertices = m_pMesh->GetNumVertices();

	// �����´� + fvf�� �������ִ� + ������ ������
	_uint iStride = D3DXGetFVFVertexSize(dwFVF);

	D3DXComputeBoundingBox(pFirstVertexPosition, dwNumVertices, iStride, &m_vMin, &m_vMax);
	
	m_pMesh->UnlockVertexBuffer();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	LPD3DXMATERIAL	pSubSetInfo = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_pMtrl = new D3DMATERIAL9[m_dwSubsetCnt];

	for(_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		// �� ����¿� �ش��ϴ� ������ ������ �����ߴ�.
		m_pMtrl[i] = pSubSetInfo[i].MatD3D;

		TCHAR			szFullPath[128] = L"";
		TCHAR			szFileName[128] = L"";

		lstrcpy(szFullPath, pPath);
		
		MultiByteToWideChar(CP_ACP, 0, pSubSetInfo[i].pTextureFilename, strlen(pSubSetInfo[i].pTextureFilename)
			, szFileName, 128);

		lstrcat(szFullPath, szFileName);
		
		if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i])))
			return E_FAIL;		
	}	

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

void Engine::CStaticMesh::Render_MeshForSDK(const D3DXMATRIX* pWorldMatrix)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pWorldMatrix);

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pGraphicDev->SetMaterial(&m_pMtrl[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_MeshForShader(LPD3DXEFFECT pEffect, _bool bColliderDraw)
{
	// ����� ������ �׷������ڴ�.
	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);

		// ������ ������ ���̴��� ����
		pEffect->SetVector("g_vMeshDiffuse", &_vec4((_float*)&m_pMtrl[i].Diffuse));	
		pEffect->SetVector("g_vMeshAmbient", &_vec4((_float*)&m_pMtrl[i].Ambient));	

		pEffect->CommitChanges();

		m_pMesh->DrawSubset(i);
	}

#ifdef _DEBUG
	if(true == bColliderDraw)
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

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice , const TCHAR* pPath, const TCHAR* pFileName)
{
	CStaticMesh*		pMesh = new CStaticMesh(pDevice);
	if(FAILED(pMesh->Load_MeshFromFile(pPath, pFileName)))
		Engine::Safe_Release(pMesh);

	return pMesh;
}

_ulong Engine::CStaticMesh::Release(void)
{
	_ulong	dwRefCnt = Engine::CMesh::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{
			Engine::Safe_Delete_Array(m_pMtrl);

			for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
			{
				if(Engine::Safe_Release(m_ppTexture[i]))
					MSG_BOX("Static Mesh`s Texture Release Failed");
			}
			Engine::Safe_Delete_Array(m_ppTexture);
			if(Engine::Safe_Release(m_pMesh))
				MSG_BOX("Static Mesh Release Failed");
			Engine::Safe_Release(m_pSubset);
#ifdef _DEBUG
			Engine::Safe_Release(m_pVB);
			Engine::Safe_Release(m_pIB);
#endif
		}
		delete this;
	}	
	return dwRefCnt;
}

