#include "stdafx.h"
#include "Tree.h"
#include "Export_Function.h"

CTree::CTree(LPDIRECT3DDEVICE9 pGraphicDev)
	: CLandObject(pGraphicDev)
	, m_pEffect(NULL)
{

}

CTree::~CTree(void)
{

}

HRESULT CTree::Initialize(void)
{
	if (FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Effect");
	if (NULL == m_pEffect)
		return E_FAIL;

	if (FAILED(Add_Component()))
		return E_FAIL;

	//m_pTransCom->m_vPosition = _vec3((float)(rand() % 100), 0.f, (float)(rand() % 100));

	m_pTransCom->m_vPosition = _vec3(rand() % 10, 5.f, rand() % 10);

	m_pTransCom->m_vScale = D3DXVECTOR3(10.f, 10.f, 10.);

	return S_OK;
}

HRESULT CTree::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Tree");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Tree1", pComponent));
	
	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_Tree");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Tree", pComponent));


	return S_OK;
}

_int CTree::Update(const _float& fTimeDelta)
{
	//CLandObject::Update(fTimeDelta);
	
	Engine::CGameObject::Update(fTimeDelta);

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	
	return 0;
}

void CTree::Render(void)
{
	if (NULL == m_pEffect)
		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();

	m_pEffect->End();

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CTree* CTree::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTree*		pGameObject = new CTree(pGraphicDev);

	if (FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTombStone Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CTree::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");
}

_ulong CTree::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if (0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
