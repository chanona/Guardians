#include "stdafx.h"
#include "MouseCol.h"
#include "GameFrameWork.h"
#include "ClientNetEngine.h"

CMouseCol::CMouseCol()
: m_vPivotPos(0.f, 0.f, 0.f)
, m_vRayDir(0.f, 0.f, 0.f)
{

}

CMouseCol::~CMouseCol()
{

}

POINT CMouseCol::GetMousePos(void)
{
	POINT	pt;

	GetCursorPos(&pt);
	ScreenToClient(FRAMEWORK->GetHwnd(), &pt);

	return pt;
}

void CMouseCol::Translation_ViewSpace(void)
{
	POINT		ptMouse = GetMousePos();
	D3DXMATRIX	matProj;

	matProj = NETWORK_ENGINE->GetMyPlayer()->GetCamera()->GetProjectionMatrix();

	D3DXVECTOR3		vTemp;
	vTemp.x = ((float(ptMouse.x) / (FRAMEWORK->m_nWndClientWidth >> 1)) - 1.f) / matProj._11;
	vTemp.y = ((float(-ptMouse.y) / (FRAMEWORK->m_nWndClientHeight >> 1)) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX		matView;

	matView = NETWORK_ENGINE->GetMyPlayer()->GetCamera()->GetViewMatrix();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInv);
}

int CMouseCol::PickTerrain(CDiffusedVertex* pTerrainVtx, WORD wCntX, WORD wCntZ)
{
	Translation_ViewSpace();

	D3DXMATRIX		matWorld;
	D3DXMatrixIdentity(&matWorld);
	Translation_Local(&matWorld);

	float fU, fV, fDist;

	CDiffusedVertex* pVertex = pTerrainVtx;

	for (int z = 0; z < wCntZ - 1; ++z)
	{
		for (int x = 0; x < wCntX - 1; ++x)
		{
			int iIndex = z * wCntX + x;

			if (D3DXIntersectTri(&pVertex[iIndex + wCntX + 1].m_d3dxvPosition
				, &pVertex[iIndex + wCntX].m_d3dxvPosition
				, &pVertex[iIndex + 1].m_d3dxvPosition
				, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				NETWORK_ENGINE->GetMyPlayer()->SetPosition(pVertex[iIndex].m_d3dxvPosition);
				return iIndex;
			}
			if (D3DXIntersectTri(&pVertex[iIndex].m_d3dxvPosition
				, &pVertex[iIndex + 1].m_d3dxvPosition
				, &pVertex[iIndex + wCntX].m_d3dxvPosition
				, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				NETWORK_ENGINE->GetMyPlayer()->SetPosition(pVertex[iIndex].m_d3dxvPosition);
				return iIndex;	
			}
		}
	}
	return -1;
}

void CMouseCol::Release()
{

}

