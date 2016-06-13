#include "stdafx.h"
#include "Arrow.h"
#include "Transform.h"

#include "Export_Function.h"

CArrow::CArrow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_pEffect(NULL)
	, m_pParentMatrix(NULL)
	, m_pParentWorldMatrix(NULL)
	, m_pMonster(NULL)
{

}

CArrow::~CArrow(void)
{

}

HRESULT CArrow::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(90.0f);
	
	return S_OK;
}

HRESULT CArrow::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Arrow");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	return S_OK;
}

_int CArrow::Update(const _float& fTimeDelta)
{
	if (m_pMonster == NULL)
		return -1;

	Engine::CComponent*	pMonTrans = m_pMonster->Get_Component(L"Com_Transform");

	_vec3 vDir = ((Engine::CTransform*)pMonTrans)->m_vPosition + D3DXVECTOR3(0.1f, 1.5f, 0.f) - m_pTransCom->m_vPosition;
	_float fDist = D3DXVec3Length(&vDir);

	D3DXVec3Normalize(&vDir, &vDir);

	_float fCos = D3DXVec3Dot(&vDir, &D3DXVECTOR3(0.f, 0.f, -1.f));
	_float fAngle = acosf(fCos);
	if (vDir.x > 0.f)
		fAngle = 2 * D3DX_PI - fAngle;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fAngle;

	// 몹방향으로 이동
	m_pTransCom->m_vPosition += vDir * 40.f * fTimeDelta;

	if (fDist < 1.f)
	{
		Engine::CComponent*	pMonMesh = m_pMonster->Get_Component(L"Com_Mesh");

		// 몬스터 피 깎아
		if (!(((Engine::CDynamicMesh*)pMonMesh)->Get_AnimationSet() == SALA_DIE))
			((Engine::CDynamicMesh*)pMonMesh)->Set_AnimationSet(SALA_DAMAGED);

		else
			m_pMonster = NULL;

		return -1;		// 리턴 값 -1이면 지워
	}
	/*if (NULL == m_pParentMatrix)
	{
		Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
		m_pParentMatrix = pPlayerMeshCom->Get_FrameMatrix("R_Hand");

		Engine::CTransform* pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
		m_pParentWorldMatrix = &pPlayerTransCom->m_matWorld;
	}*/

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	Engine::CGameObject::Update(fTimeDelta);

	//m_pTransCom->m_matWorld = m_pTransCom->m_matWorld * *m_pParentMatrix * *m_pParentWorldMatrix;

	return 0;
}

void CArrow::Render(void)
{
	if (NULL == m_pEffect)
		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();

	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CArrow* CArrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CArrow*		pGameObject = new CArrow(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CArrow Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CArrow::Set_Monster(CGameObject * pGameObject)
{
	m_pMonster = pGameObject;
}

void CArrow::Set_Position(const D3DXVECTOR3& vPosition)
{
	m_pTransCom->m_vPosition = vPosition;
}

void CArrow::Set_ContantTable(void)
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

_ulong CArrow::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
