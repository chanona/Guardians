#include "stdafx.h"
#include "MouseCol.h"
#include "Export_Function.h"
#include "Defines.h"

CMouseCol::CMouseCol(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_vPivotPos(0.f, 0.f, 0.f)
, m_vRayDir(0.f, 0.f, 0.f)
{

}

CMouseCol::~CMouseCol(void)
{
	Release();
}

POINT CMouseCol::GetMousePos(void)
{
	POINT	pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return pt;
}

CMouseCol* CMouseCol::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMouseCol* pCollision = new CMouseCol(pDevice);
	if (FAILED(pCollision->Initialize()))
		Engine::Safe_Delete(pCollision);

	return pCollision;
}

void CMouseCol::PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx)
{
	Translation_ViewSpace();

	D3DXMATRIX		matWorld;
	D3DXMatrixIdentity(&matWorld);
	Translation_Local(&matWorld);

	const Engine::VTXTEX*	pVertex = pTerrainVtx;

	float		fU, fV, fDist;

	for (int z = 0; z < VTXCNTZ - 1; ++z)
	{
		for (int x = 0; x < VTXCNTX - 1; ++x)
		{
			int		iIndex = z * VTXCNTX + x;

			if (D3DXIntersectTri(&pVertex[iIndex + VTXCNTX + 1].vPos
				, &pVertex[iIndex + VTXCNTX].vPos
				, &pVertex[iIndex + 1].vPos
				, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = pVertex[iIndex + VTXCNTX + 1].vPos
					+ (pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fU
					+ (pVertex[iIndex + 1].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fV;
				return;
			}
			if (D3DXIntersectTri(&pVertex[iIndex].vPos
				, &pVertex[iIndex + 1].vPos
				, &pVertex[iIndex + VTXCNTX].vPos
				, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = pVertex[iIndex].vPos
					+ (pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fU
					+ (pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex].vPos) * fV;
				return;
			}
		}
	}
}

void CMouseCol::PickObject(D3DXVECTOR3* pOut, const Engine::VTXTEX* pVertex, const D3DXMATRIX* pmatWorld)
{

}

HRESULT CMouseCol::Initialize(void)
{
	return S_OK;
}

void CMouseCol::Translation_ViewSpace(void)
{
	POINT		ptMouse = GetMousePos();

	//	0 -> -1		, 800 -> 1
	//  0 ->1		, 600 - > -1

	D3DXMATRIX matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3		vTemp;
	vTemp.x = ((float(ptMouse.x) / (WINSIZEX >> 1)) - 1.f) / matProj._11;
	vTemp.y = ((float(-ptMouse.y) / (WINSIZEY >> 1)) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	/*D3DXMATRIX		matView;
	D3DXMatrixInverse(&matView, NULL, pmatProj);
	D3DXVec3TransformCoord(&vTemp, &vTemp, &matView);*/

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX matView;
	D3DXMATRIX matViewInv;
	
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matViewInv, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matViewInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matViewInv);

	D3DXMATRIX		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInv);
}

_ulong CMouseCol::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

