#include "stdafx.h"
#include "DynamicCamera.h"
#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_bMouseFix(true)
, m_bClick(false)
{

}

CDynamicCamera::~CDynamicCamera(void)
{
	
}

HRESULT CDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINSIZEX) / WINSIZEY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();
	return S_OK;
}

void CDynamicCamera::KeyCheck(const _float& fTimeDelta)
{
	float	fTime = fTimeDelta;

	if(Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye -= vRight * m_fCamSpeed * fTime;
		m_vAt -= vRight * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		D3DXVECTOR3		vRight;
		D3DXMATRIX		matCamState;

		D3DXMatrixInverse(&matCamState, NULL, &m_matView);
		memcpy(&vRight, &matCamState.m[0][0], sizeof(D3DXVECTOR3));
		D3DXVec3Normalize(&vRight, &vRight);

		m_vEye += vRight * m_fCamSpeed * fTime;
		m_vAt += vRight * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_W) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed * fTime;
		m_vAt += vLook * m_fCamSpeed * fTime;
	}

	if(Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		D3DXVECTOR3		vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed * fTime;
		m_vAt -= vLook * m_fCamSpeed * fTime;
	}

	int		iDistance = 0;

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXMATRIX		matViewInv;
		D3DXVECTOR3		vRight;
		D3DXMatrixInverse(&matViewInv, NULL, &m_matView);
		memcpy(&vRight, &matViewInv.m[0][0], sizeof(D3DXVECTOR3));

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance / 10.f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vAt = m_vEye + vDir;
	}
}

_int CDynamicCamera::Update(const _float& fTimeDelta)
{
	KeyCheck(fTimeDelta);
	FixMouse();

	Engine::CCamera::Update(fTimeDelta);

	return 0;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice , const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pDevice);
	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
	POINT		ptMouse = {WINSIZEX >> 1, WINSIZEY >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_ulong CDynamicCamera::Release()
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

