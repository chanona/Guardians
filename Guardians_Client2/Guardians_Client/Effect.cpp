#include "stdafx.h"
#include "Effect.h"
#include "Export_Function.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_fAccFrame(0.f)
{

}

CEffect::~CEffect(void)
{

}

HRESULT CEffect::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Effect");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(rand() % 10, 5.f, rand() % 10);

	return S_OK;
}

HRESULT CEffect::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));


	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Effect");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		
	return S_OK;
}

_int CEffect::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	m_fAccFrame += 90.0f * fTimeDelta;

	if(m_fAccFrame >= 90.0f)
		m_fAccFrame = 0.f;

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	return 0;
}

void CEffect::Render(void)
{
	if(NULL == m_pEffect)
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

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect*		pLogoBack = new CEffect(pGraphicDev);

	if(FAILED(pLogoBack->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pLogoBack);
	}
	return pLogoBack;
}

void CEffect::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", _ulong(m_fAccFrame));	
}

_ulong CEffect::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
