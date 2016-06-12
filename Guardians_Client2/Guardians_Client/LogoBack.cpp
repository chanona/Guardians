#include "stdafx.h"
#include "LogoBack.h"
#include "Export_Function.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

CLogoBack::~CLogoBack(void)
{

}

HRESULT CLogoBack::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogoBack::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Material Component
	D3DMATERIAL9			MtrlInfo;
	MtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	MtrlInfo.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(MtrlInfo);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_LOGO, L"Texture_Logo");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));
		
	return S_OK;
}

_int CLogoBack::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	m_pTransCom->m_vScale = D3DXVECTOR3(2.f, 2.f, 2.f);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CLogoBack::Render(void)
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

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoBack*		pLogoBack = new CLogoBack(pGraphicDev);

	if(FAILED(pLogoBack->Initialize()))
	{
		MSG_BOX("pLogoBack Create Faild");
		::Safe_Release(pLogoBack);
	}
	return pLogoBack;
}

void CLogoBack::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
}

_ulong CLogoBack::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}
