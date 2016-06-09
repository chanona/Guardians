#include "stdafx.h"
#include "Sword.h"

#include "Export_Function.h"

CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pParentMatrix(NULL)
, m_pParentWorldMatrix(NULL)
{
	
}

CSword::~CSword(void)
{

}

HRESULT CSword::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)
		return E_FAIL;	

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(90.0f);

	return S_OK;
}

HRESULT CSword::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Sword");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

		
	return S_OK;
}

_int CSword::Update(const _float& fTimeDelta)
{
	if(NULL == m_pParentMatrix)
	{	
		Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
		m_pParentMatrix = pPlayerMeshCom->Get_FrameMatrix("R_Hand");

		Engine::CTransform* pPlayerTransCom =  (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
		m_pParentWorldMatrix = &pPlayerTransCom->m_matWorld;		
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	Engine::CGameObject::Update(fTimeDelta);

	m_pTransCom->m_matWorld = m_pTransCom->m_matWorld * *m_pParentMatrix * *m_pParentWorldMatrix;

	return 0;
}

void CSword::Render(void)
{
	if(NULL == m_pEffect)
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

CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSword*		pGameObject = new CSword(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTombStone Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CSword::Set_ContantTable(void)
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

_ulong CSword::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}

	return dwRefCnt;
}
