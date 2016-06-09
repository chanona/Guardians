#include "stdafx.h"
#include "Camera.h"

Engine::CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
{

}

Engine::CCamera::~CCamera(void)
{

}

_int Engine::CCamera::Update(const _float& fTimeDelta)
{
	Invalidate_ViewMatrix();

	return 0;
}

void Engine::CCamera::Invalidate_ViewMatrix(void)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void Engine::CCamera::Invalidate_ProjMatrix(void)
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFovY, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

_ulong Engine::CCamera::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{

	}

	return dwRefCnt;

}

