#include "stdafx.h"
#include "Player.h"
#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
: CLandObject(pGraphicDev)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_bPush(false)
, m_iAniIdx(0)
, m_pMouseCol(nullptr)
, m_bMove(false)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	if(FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)
		return E_FAIL;	

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale = _vec3(0.01f, 0.01f, 0.01f);

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);

	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

		
	return S_OK;
}

_int CPlayer::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Check_KeyState(fTimeDelta);

	if (m_bMove)
		Move(fTimeDelta);

	if(true == m_pMeshCom->Check_EndPeriod())
		m_pMeshCom->Set_AnimationSet(0);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	CLandObject::Update(fTimeDelta);

	return 0;
}

void CPlayer::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ContantTable();

	m_pMeshCom->Move_Frame(m_fTimeDelta);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
#ifdef _DEBUG
	_matrix			matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	_matrix			matTrans;
	D3DXMatrixIdentity(&matTrans);
	memcpy(&matTrans.m[3][0], &m_pTransCom->m_matWorld.m[3][0], sizeof(_vec3));

	_matrix			matWorld;
	matWorld = matScale * matTrans;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
#endif	

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);
}

void CPlayer::Move(const _float& fTimeDelta)
{
	D3DXVECTOR3		vDir = m_vDestPos - m_pTransCom->m_vPosition;

	float		fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransCom->m_vPosition += vDir * 3.f * fTimeDelta;

	if (fDistance < 1.f)
		m_bMove = false;
}

void CPlayer::SetPush(int iIndex)
{
	if (true == m_bPush)
		return;

	m_bPush = true;

	m_pMeshCom->Set_AnimationSet(iIndex);

	if (m_iAniIdx > 2)
		m_iAniIdx = 0;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pGameObject = new CPlayer(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CPlayer Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CPlayer::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
}

void CPlayer::Check_KeyState(const _float& fTimeDelta)
{
	if (Engine::GetDIKeyState(DIK_W) & 0x80)
	{
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &m_pTransCom->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);
		m_pTransCom->m_vPosition += vDirection * -1.f * 3.0f * fTimeDelta;

		SetPush(1);
	}
	else m_bPush = false;

	if (Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		_vec3			vDirection(0.f, 0.f, 0.f);

		memcpy(&vDirection, &m_pTransCom->m_matWorld.m[2][0], sizeof(_vec3));
		D3DXVec3Normalize(&vDirection, &vDirection);
		m_pTransCom->m_vPosition -= vDirection * -1.f * 3.0f * fTimeDelta;

		SetPush(1);
	}

	if (Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(90.0f) * fTimeDelta;
	}

	if (Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(90.0f) * fTimeDelta;
	}

	if(Engine::GetDIKeyState(DIK_SPACE) & 0x80)
		SetPush(2);
	else m_bPush = false;

	if (Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON))
	{
		m_bMove = true;
		m_pMouseCol->PickTerrain(&m_vDestPos, m_pVertex);
		list<Engine::CGameObject*>* pTest = Engine::Find_ObjectList(L"GameLogic", L"TombStone");
		auto iter = pTest->begin();
		auto iter_end = pTest->end();

		for (; iter != iter_end; ++iter)
		{
			Engine::CComponent* pTransCom = (*iter)->Get_Component(L"Com_Transform");
			D3DXVECTOR3 vPos = ((Engine::CTransform*)pTransCom)->m_vPosition;
			D3DXVECTOR3 vDir = m_vDestPos - vPos;
			if (D3DXVec3Length(&vDir) < 1.f)
			{
				int i = 0;
			}
		}
	}
}

void CPlayer::Set_MouseCol(CMouseCol* pMouse)
{
	m_pMouseCol = pMouse;
}

_ulong CPlayer::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}

	return dwRefCnt;
}
