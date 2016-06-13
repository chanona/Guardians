#include "stdafx.h"
#include "StaticCamera.h"
#include "Export_Function.h"


CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice)
	: Engine::CCamera(pDevice)
	, m_bMouseFix(true)
	, m_bClick(false)
	, m_fHeelUp(10.f)
	, m_fHeelDown(10.f)
	, m_fAngle(0.f)
	, m_bStart(true)
{

}

CStaticCamera::~CStaticCamera(void)
{

}

HRESULT CStaticCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINSIZEX) / WINSIZEY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	m_fHeight = -2.f;
	return S_OK;
}

_int CStaticCamera::Update(const _float& fTimeDelta)
{
	//FixMouse();

	Engine::CCamera::Update(fTimeDelta);
	SetCameraPos(fTimeDelta);

	return 0;
}

void CStaticCamera::SetCameraPos(const _float& fTimeDelta)
{
	if (GetActiveWindow() != g_hWnd) return;

	int		iDistance = 0;

	m_vAt = ((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_vPosition + D3DXVECTOR3(0.f, 3.f, 0.f);

	float fAngleY = ((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_fAngle[Engine::CTransform::ANGLE_Y] + D3DX_PI / 2.f;

	m_vEye = m_vAt - D3DXVECTOR3(m_fHeelUp * cosf(-fAngleY), m_fHeight, m_fHeelUp * sinf(-fAngleY));
	
	if (true == m_bStart)
	{
		m_bStart = false;
	}

	if (Engine::GetDIKeyState(DIK_A))
	{
		((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(90.0f) * fTimeDelta;
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);
		// 여기다가 이제 카메라 이동을 만들어 준다 .
	}

	if (Engine::GetDIKeyState(DIK_D))
	{
		((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(90.0f) * fTimeDelta;
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &((Engine::CTransform*)(Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player")))->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);
		// 여기다가 이제 카메라 이동을 만들어 준다 .
	}

	if (Engine::GetDIMouseMove(Engine::CInput::DIM_Z) > 0.f)
	{
		m_fHeight += 0.05f;
		m_fHeelUp -= 0.5f;
	}

	if (Engine::GetDIMouseMove(Engine::CInput::DIM_Z) < 0.f)
	{
		m_fHeight -= 0.05f;
		m_fHeelUp += 0.5f;
	}
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CStaticCamera*		pCamera = new CStaticCamera(pDevice);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CStaticCamera::FixMouse(void)
{
	POINT		ptMouse = { WINSIZEX >> 1, WINSIZEY >> 1 };
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_ulong CStaticCamera::Release()
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

