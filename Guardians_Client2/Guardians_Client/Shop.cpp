#include "stdafx.h"
#include "Shop.h"
#include "Export_Function.h"

CShop::CShop(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandObject(pGraphicDev)
	, m_pEffect(NULL)
	, m_bColl(false)
{

}

CShop::~CShop(void)
{

}

HRESULT CShop::Initialize(void)
{
	if (FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(318.f, 0.f, 332.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(-90.f);

	m_pTransCom->m_vScale /= 270.f;

	return S_OK;
}

HRESULT CShop::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Shop");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	return S_OK;
}

_int CShop::Update(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	CLandObject::Update(fTimeDelta);

	/*_vec3			vDestMin, vDestMax;
	m_pMeshCom->Get_MinMax(&vDestMin, &vDestMax);
	_matrix			mDestWorld, mDestTrans, mDestScale;
	D3DXMatrixScaling(&mDestScale, m_pTransCom->m_vScale.x, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z);
	D3DXMatrixTranslation(&mDestTrans, m_pTransCom->m_vPosition.x, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z);

	Engine::CMesh* pPlayerMeshCom = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
	_vec3			vSourMin, vSourMax;
	pPlayerMeshCom->Get_MinMax(&vSourMin, &vSourMax);
	Engine::CTransform*		pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
	_matrix			mSourWorld, mSourScale, mSourTrans;
	D3DXMatrixScaling(&mSourScale, pPlayerTransCom->m_vScale.x, pPlayerTransCom->m_vScale.y, pPlayerTransCom->m_vScale.z);
	D3DXMatrixTranslation(&mSourTrans, pPlayerTransCom->m_vPosition.x, pPlayerTransCom->m_vPosition.y, pPlayerTransCom->m_vPosition.z);
	mSourWorld = mSourScale * mSourTrans;
	mDestWorld = mDestScale * mDestTrans;

	m_bColl = Engine::Collision_AABB(vDestMin, vDestMax, mDestWorld
		, vSourMin, vSourMax, mSourWorld);*/

	//Engine::CMesh* pSwordMeshCom = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Sword");	
	//pSwordMeshCom->Get_MinMax(&vSourMin, &vSourMax);

	//Engine::CTransform*		pSwordTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Sword");

	//m_bColl = Engine::Collision_OBB(vDestMin, vDestMax, m_pTransCom->m_matWorld
	//	, vSourMin, vSourMax, pSwordTransCom->m_matWorld);

	return 0;
}

void CShop::Render(void)
{
	if (NULL == m_pEffect)
		return;

	if (true == m_bColl)
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

CShop* CShop::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShop*		pGameObject = new CShop(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CShop Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CShop::Set_ContantTable(void)
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

_ulong CShop::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
