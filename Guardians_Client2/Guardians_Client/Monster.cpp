#include "stdafx.h"
#include "Monster.h"
#include "Export_Function.h"
#include "protocol.h"
#include "ClientNetEngine.h"
CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandObject(pGraphicDev)
	, m_pEffect(NULL)
	, m_fTimeDelta(0.f)
	, m_iAniIdx(SALA_CRY)
	, m_bMove(false)
{

}

CMonster::~CMonster(void)
{

}

HRESULT CMonster::Initialize(void)
{
	if (FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale = _vec3(3.f, 3.f, 3.f);
		
	m_pTransCom->m_vPosition = _vec3((float)(rand() % 10), 0.f, (float)(rand() % 10));

	m_iHP = 100;
	m_iAtt = 10;

	return S_OK;
}

HRESULT CMonster::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Monster");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	return S_OK;
}

_int CMonster::Update(const _float& fTimeDelta)
{
	if (m_bAlive == false)
		return 0;
	
	m_fTimeDelta = fTimeDelta;

	if (m_bMove)
		Move(fTimeDelta);

	if ((m_pMeshCom->Check_EndPeriod()) && !m_bMove)
	{
		if (m_pMeshCom->Get_AnimationSet() == SALA_DAMAGED)
			m_iHP -= 50;

		if (m_pMeshCom->Get_AnimationSet() == SALA_DIE)
		{
			cs_packet_remove_monster pkt;
			pkt.size = sizeof(pkt);
			pkt.type = CSPacketType::CS_REMOVE_MONSTER;
			pkt.monster_id = m_id;

			NETWORK_ENGINE->SendPacket((char *)&pkt);
			return -1;
		}
			

		if (m_iHP <= 0)
			m_pMeshCom->Set_AnimationSet(SALA_DIE);
		else
			m_pMeshCom->Set_AnimationSet(SALA_CRY);
	}
	
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	CLandObject::Update(fTimeDelta);

	return 0;
}

void CMonster::Render(void)
{
	if (m_bAlive == false) return;

	if (NULL == m_pEffect)
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

void CMonster::Clear()
{
	m_iAniIdx = SALA_CRY;
	m_bMove = false;
	m_pTransCom->m_vPosition = _vec3(25.f, 0.f, 25.0f);
	m_iHP = 100;
	m_iAtt = 10;
}

void CMonster::Move(const _float& fTimeDelta)
{
	D3DXVECTOR3		vDir = m_vDestPos - m_pTransCom->m_vPosition;

	float		fDistance = D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransCom->m_vPosition += vDir * 3.f * fTimeDelta;

	if (fDistance < 1.f)
	{
		m_pMeshCom->Set_AnimationSet(0);
		m_bMove = false;
	}
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster*		pGameObject = new CMonster(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CMonster Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CMonster::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if (NULL == pLightInfo)
		return;

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));
}

_ulong CMonster::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

void CMonster::Reset()
{
	m_fTimeDelta = 0.f;
	m_iAniIdx = SALA_CRY;
	m_bMove = false;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale = _vec3(3.f, 3.f, 3.f);

	m_pTransCom->m_vPosition = _vec3(0.f, 0.f, 0.0f);

	m_iHP = 100;
	m_iAtt = 10;
}
