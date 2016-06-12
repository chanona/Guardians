#include "stdafx.h"
#include "Terrain.h"
#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
{

}

CTerrain::~CTerrain(void)
{

}

HRESULT CTerrain::Initialize(void)
{


	m_pEffect = Engine::Get_EffectHandle(L"Shader_Terrain");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	// 직접 이미지를 생성한다.
	/*if(FAILED(D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pGaraTexture)))
		return E_FAIL;

	D3DLOCKED_RECT			rcRect;
	ZeroMemory(&rcRect, sizeof(D3DLOCKED_RECT));

	m_pGaraTexture->LockRect(0, &rcRect, NULL, 0);

	for (_int i = 0; i < 129; ++i)
	{
		_byte* pByte = ((_byte*)rcRect.pBits) + rcRect.Pitch * i;

		for (_int j = 0; j < 129; ++j)
		{

			

			_ulong* pColor = ((_ulong*)pByte + j);

			if(j > 65)
				*pColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
			else
				*pColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);


		}
	}

	m_pGaraTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../GaraTexture.jpg", D3DXIFF_JPG, m_pGaraTexture, NULL);*/

	D3DXCreateTextureFromFile(m_pGraphicDev, L"../Resource/Texture/StageScene/Terrain/a.tga", &m_pGaraTexture);


	return S_OK;
}

HRESULT CTerrain::Add_Component(void)
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
	MtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	MtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	pComponent = m_pMtrlCom = Engine::CMaterial::Create(MtrlInfo);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Material", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	/*pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));*/

	/*pComponent = m_pFloorTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Floor");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_FloorTexture", pComponent));*/

	pComponent = m_pTileTextureCom[0] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Tile");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TileTexture", pComponent));

	pComponent = m_pTileTextureCom[1] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Tile2");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TileTexture2", pComponent));

	pComponent = m_pTileTextureCom[2] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Tile3");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TileTexture3", pComponent));

	pComponent = m_pTileTextureCom[3] = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Tile4");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TileTexture4", pComponent));

	pComponent = m_pTileNormalCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_TileNormal");
	if (NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TileNormal", pComponent));


	pComponent = m_pColorHeightTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_ColorHeight");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_ColorHeightTexture", pComponent));

		
	return S_OK;
}

_int CTerrain::Update(const _float& fTimeDelta)
{
	Engine::CGameObject::Update(fTimeDelta);

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);

	return 0;
}

void CTerrain::Render(void)
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

	//m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_TileTexture", NULL);
	m_pEffect->SetTexture("g_TileTexture2", NULL);
	m_pEffect->SetTexture("g_TileTexture3", NULL);
	m_pEffect->SetTexture("g_TileTexture4", NULL);
	m_pEffect->SetTexture("g_ColorHeightTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*		pGameObject = new CTerrain(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTerrain Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CTerrain::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	//m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture");	
	m_pTileTextureCom[0]->Set_ConstantTable(m_pEffect, "g_TileTexture");

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));

	const D3DMATERIAL9* pMtrlInfo = m_pMtrlCom->Get_MtrlInfo();
	m_pEffect->SetVector("g_vTerrainDiffuse", &_vec4((_float*)&pMtrlInfo->Diffuse));
	m_pEffect->SetVector("g_vTerrainAmbient", &_vec4((_float*)&pMtrlInfo->Ambient));

	//m_pFloorTextureCom->Set_ConstantTable(m_pEffect, "g_FloorTexture");
	m_pTileTextureCom[1]->Set_ConstantTable(m_pEffect, "g_TileTexture2");
	m_pTileTextureCom[2]->Set_ConstantTable(m_pEffect, "g_TileTexture3");
	m_pTileTextureCom[3]->Set_ConstantTable(m_pEffect, "g_TileTexture4");
	m_pTileNormalCom->Set_ConstantTable(m_pEffect, "g_NormalTexture");
	m_pColorHeightTextureCom->Set_ConstantTable(m_pEffect, "g_ColorHeightTexture");
	m_pEffect->SetTexture("g_ColorHeightTexture", m_pGaraTexture);

	m_pEffect->SetVector("g_vTargetPos", &_vec4(10.0f, 0.0f, 10.0f, 1.0f));
	m_pEffect->SetFloat("g_fTargetRange", 10.0f);
	
}


_ulong CTerrain::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Release(m_pGaraTexture);
		delete this;
	}

	return dwRefCnt;
}
