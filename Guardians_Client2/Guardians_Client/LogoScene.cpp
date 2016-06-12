#include "stdafx.h"
#include "LogoScene.h"
#include "LogoBack.h"
#include "Loading.h"
#include "SceneSelector.h"
#include "Export_Function.h"

CLogoScene::CLogoScene(LPDIRECT3DDEVICE9 pGrpahicDev)
: CScene(pGrpahicDev)
{

}

CLogoScene::~CLogoScene(void)
{

}

HRESULT CLogoScene::Init_Scene(void)
{
	if(FAILED(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", 10, 20, FW_HEAVY)))
	{
		MSG_BOX("Ready_Font Failed");
		return E_FAIL;
	}

	if(FAILED(Ready_Lighting()))
	{
		MSG_BOX("Ready_Lighting Failed");
		return E_FAIL;
	}

	if(FAILED(Ready_Resources()))
	{
		MSG_BOX("Ready_Resources Failed");
		return E_FAIL;
	}

	if(FAILED(Ready_Environment()))
	{
		MSG_BOX("Ready_Environment Failed");
		return E_FAIL;
	}

	return S_OK;
}

_int CLogoScene::Update(const _float& fTimeDelta)
{
	CScene::Update(fTimeDelta);	

	if(true == m_pLoading->GetComplete())
	{
		if(GetAsyncKeyState(VK_RETURN) & 0x8000)		
		{
			if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_STAGE))))
			{
				MSG_BOX("Change_Scene(SCENE_STAGE) Failed");
				return E_FAIL;
			}
			return 0;
		}
	}

	return  0;
}

void CLogoScene::Render(void)
{
	Engine::Render_Font(L"Font_Default", _vec3(650.f, 550.f, 0.f), m_pLoading->GetLoadingMessage()
		, D3DXCOLOR(0.2f, 0.5f, 0.5f, 1.f));
}

HRESULT CLogoScene::Ready_Lighting(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(0.f, 0.f, 0.f);

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLogoScene::Ready_Resources(void)
{
	Engine::Reserve_ContainerSize(RESOURCE_END);

	m_pLoading = CLoading::Create(CLoading::LOADING_STAGE);
	if(NULL == m_pLoading)
	{
		MSG_BOX("CLoading Create Failed");
		return E_FAIL;
	}

	// For.Buffers ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_RcTex")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return E_FAIL;
	}

	// For.Texture ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, L"Texture_Logo", Engine::CTexture::TYPE_NORMAL, L"../Resource/Texture/LogoScene/Logo%d.jpg", 1)))
	{
		MSG_BOX("Ready_Buffer Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLogoScene::Ready_Environment(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	// For.CLogoBack Instance
	pGameObject = CLogoBack::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"LogoBack", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Environment", pLayer));

	return S_OK;
}

CLogoScene* CLogoScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoScene*	pScene = new CLogoScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("pScene Initialized Failed");
		::Safe_Release(pScene);
	}

	return pScene;
}

_ulong CLogoScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Release(m_pLoading);
		Engine::Release_Light();
		Engine::Reset_Resource(RESOURCE_LOGO);
		delete this;
	}

	return dwRefCnt;
}

