#include "stdafx.h"
#include "Npc.h"
#include "Export_Function.h"

CNpc::CNpc(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandObject(pGraphicDev)
	, m_pEffect(NULL)
	, m_fTimeDelta(0.f)
	, m_iAniIdx(0)
	, m_bMove(false)
{

}

CNpc::~CNpc(void)
{

}

HRESULT CNpc::Initialize(void)
{
	if (FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale /= 55.f;

	m_pTransCom->m_vPosition = _vec3(320.f, 0.f, 328.0f);

	return S_OK;
}

HRESULT CNpc::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Npc");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));
	
	return S_OK;
}

_int CNpc::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	CLandObject::Update(fTimeDelta);

	return 0;
}

void CNpc::Render(void)
{
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

void CNpc::Move(const _float& fTimeDelta)
{
	
}

CNpc* CNpc::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNpc*		pGameObject = new CNpc(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CMonster Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CNpc::Set_ContantTable(void)
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

_ulong CNpc::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
