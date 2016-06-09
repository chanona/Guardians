#include "stdafx.h"
#include "Collision.h"

USING(Engine)

IMPLEMENT_SINGLETON(CCollision)

CCollision::CCollision(void)
{
	
}

CCollision::~CCollision(void)
{

}

_bool CCollision::Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
					 , const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld)
{
	_vec3			vWorldDestMin, vWorldDestMax;
	_vec3			vWorldSourMin, vWorldSourMax;


	// 월드영역으로 변화나낟.
	D3DXVec3TransformCoord(&vWorldDestMin, &vDestMin, &mDestWorld);
	D3DXVec3TransformCoord(&vWorldDestMax, &vDestMax, &mDestWorld);

	D3DXVec3TransformCoord(&vWorldSourMin, &vSourMin, &mSourWorld);
	D3DXVec3TransformCoord(&vWorldSourMax, &vSourMax, &mSourWorld);

	// 충돌하자.
	_float				fMin, fMax;

	// x축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.x, vWorldSourMin.x);
	fMax = min(vWorldDestMax.x, vWorldSourMax.x);

	if(fMin > fMax)
		return false;

	// y축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.y, vWorldSourMin.y);
	fMax = min(vWorldDestMax.y, vWorldSourMax.y);

	if(fMin > fMax)
		return false;

	// z축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.z, vWorldSourMin.z);
	fMax = min(vWorldDestMax.z, vWorldSourMax.z);

	if(fMin > fMax)
		return false;

	return true;	
}

_bool Engine::CCollision::Collision_OBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
					, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld)
{
	ZeroMemory(m_tOBB, sizeof(OBB) * 2);

	Set_Points(&m_tOBB[0], vDestMin, vDestMax);
	Set_Points(&m_tOBB[1], vSourMin, vSourMax);
	
	for (_int j = 0; j < 8; ++j)
	{
		D3DXVec3TransformCoord(&m_tOBB[0].vPoint[j], &m_tOBB[0].vPoint[j], &mDestWorld);
		D3DXVec3TransformCoord(&m_tOBB[1].vPoint[j], &m_tOBB[1].vPoint[j], &mSourWorld);
	}
	D3DXVec3TransformCoord(&m_tOBB[0].vCenter, &m_tOBB[0].vCenter, &mDestWorld);
	D3DXVec3TransformCoord(&m_tOBB[1].vCenter, &m_tOBB[1].vCenter, &mSourWorld);

	for (_int i = 0; i < 2; ++i)
		Set_Axis(&m_tOBB[i]);

	_float			fDistance[3]; 
	

	for (_int i = 0; i < 2; ++i)
	{
		for (_int j = 0; j < 3; ++j)	
		{
			fDistance[0] = fabs(D3DXVec3Dot(&m_tOBB[0].vProjAxis[0], &m_tOBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[0].vProjAxis[1], &m_tOBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[0].vProjAxis[2], &m_tOBB[i].vAxis[j]));


			fDistance[1] = fabs(D3DXVec3Dot(&m_tOBB[1].vProjAxis[0], &m_tOBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[1].vProjAxis[1], &m_tOBB[i].vAxis[j]))
				+ fabs(D3DXVec3Dot(&m_tOBB[1].vProjAxis[2], &m_tOBB[i].vAxis[j]));


			fDistance[2] = fabs(D3DXVec3Dot(&(m_tOBB[0].vCenter - m_tOBB[1].vCenter), &m_tOBB[i].vAxis[j]));

			if(fDistance[2] > fDistance[0] + fDistance[1])
				return false;
		}
	}

	return true;	
}

void Engine::CCollision::Set_Points(OBB* pOBB, const _vec3& vMin, const _vec3& vMax)
{
	pOBB->vPoint[0]	= _vec3(vMin.x, vMax.y, vMin.z);
	pOBB->vPoint[1]	= _vec3(vMax.x, vMax.y, vMin.z);
	pOBB->vPoint[2]	= _vec3(vMax.x, vMin.y, vMin.z);
	pOBB->vPoint[3]	= _vec3(vMin.x, vMin.y, vMin.z);

	pOBB->vPoint[4]	= _vec3(vMin.x, vMax.y, vMax.z);
	pOBB->vPoint[5]	= _vec3(vMax.x, vMax.y, vMax.z);
	pOBB->vPoint[6]	= _vec3(vMax.x, vMin.y, vMax.z);
	pOBB->vPoint[7]	= _vec3(vMin.x, vMin.y, vMax.z);

	pOBB->vCenter = (vMin + vMax) * 0.5f;
}

void Engine::CCollision::Set_Axis(OBB* pOBB)
{
	pOBB->vProjAxis[0] = (pOBB->vPoint[1] + pOBB->vPoint[6]) * 0.5f - pOBB->vCenter;
	pOBB->vProjAxis[1] = (pOBB->vPoint[0] + pOBB->vPoint[5]) * 0.5f - pOBB->vCenter;
	pOBB->vProjAxis[2] = (pOBB->vPoint[7] + pOBB->vPoint[5]) * 0.5f - pOBB->vCenter;

	pOBB->vAxis[0] = pOBB->vPoint[2] - pOBB->vPoint[3];
	pOBB->vAxis[1] = pOBB->vPoint[0] - pOBB->vPoint[3];
	pOBB->vAxis[2] = pOBB->vPoint[7] - pOBB->vPoint[3];

	for (_int i = 0; i < 3; ++i)
		D3DXVec3Normalize(&pOBB->vAxis[i], &pOBB->vAxis[i]);
}

_ulong Engine::CCollision::Release(void)
{
	delete this;

	return 0;
}