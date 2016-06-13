#include "stdafx.h"
#include "StageScene.h"
#include "Terrain.h"
#include "Player.h"
#include "Monster.h"
#include "TombStone.h"
#include "SkyBox.h"
#include "MouseCol.h"
#include "StaticCamera.h"
#include "Effect.h"
#include "UI.h"
#include "Export_Function.h"
#include "ClientNetEngine.h"
CStageScene::CStageScene(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CScene(pGraphicDev)
{

}

CStageScene::~CStageScene(void)
{

}

HRESULT CStageScene::Init_Scene(void)
{
/*
	m_pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);

	_float		fNear = 50.0f;
	_float		fFar = 200.0f;

	m_pGraphicDev->SetRenderState(D3DRS_FOGSTART, *((_ulong*)&fNear));
	m_pGraphicDev->SetRenderState(D3DRS_FOGEND, *((_ulong*)&fFar));
	m_pGraphicDev->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	m_pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));*/


	if(FAILED(Ready_Lighting()))
		return E_FAIL;

	if(FAILED(Ready_Environment()))
		return E_FAIL;

	if(FAILED(Ready_GameLogic()))
		return E_FAIL;

	// Network 
	if (!NETWORK_ENGINE->Start(g_hWnd))
	{
		cout << "Network Engine Start Failed" << endl;
		NETWORK_ENGINE->ShutDown();
		return FALSE;
	}


	return S_OK;
}

_int CStageScene::Update(const _float& fTimeDelta)
{
	if (NETWORK_ENGINE->GetID() == 0) return 0;

	Engine::CScene::Update(fTimeDelta);

	return 0;
}

void CStageScene::Render(void)
{
	
}

CStageScene* CStageScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageScene*	pScene = new CStageScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("pScene Initialized Failed");
		::Safe_Release(pScene);
	}
	return pScene;
}

HRESULT CStageScene::Ready_Lighting(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(0.f, -0.2f, 1.f); // In WorldSpace

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CStageScene::Ready_Environment(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Sky", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Environment", pLayer));	

	return S_OK;
}

HRESULT CStageScene::Ready_GameLogic(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	Engine::CGameObject*		pMouse = NULL;

	// For.StaticCamera-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//pGameObject = CStaticCamera::Create(m_pGraphicDev, &_vec3(0.f, 5.f, -5.f), &_vec3(0.f, 0.f, 0.f));
	//if(NULL == pGameObject)
	//	return E_FAIL;
	//pLayer->Ready_Object(L"Camera", pGameObject);
	//
	// MouseCol
	pMouse = pGameObject = CMouseCol::Create(m_pGraphicDev);
	if (NULL == pGameObject)
	{
		if (NULL == pGameObject)
			return E_FAIL;
		pLayer->Ready_Object(L"MouseCol", pGameObject);
	}

	// For.Terrain-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	pGameObject = CTerrain::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"Terrain", pGameObject);

	// For.Player-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//pGameObject = CPlayer::Create(m_pGraphicDev);
	//((CPlayer*)pGameObject)->Set_MouseCol((CMouseCol*)pMouse);
	//if (NULL == pGameObject)
	//	return E_FAIL;

	//pLayer->Ready_Object(L"Player", pGameObject);

	// For.Monster-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//pGameObject = CMonster::Create(m_pGraphicDev);
	//if (NULL == pGameObject)
	//	return E_FAIL;
	//pLayer->Ready_Object(L"Monster", pGameObject);

	pGameObject = CUI::Create(m_pGraphicDev);
	if (NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"UI", pGameObject);

	// For.Sword-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//pGameObject = CSword::Create(m_pGraphicDev);
	//if(NULL == pGameObject)
	//	return E_FAIL;
	//pLayer->Ready_Object(L"Sword", pGameObject);
	//ifstream fin("stonePos.txt");

	// For.TombStone-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	for (_int i = 0; i < 20; ++i)
	{
		pGameObject = CTombStone::Create(m_pGraphicDev);
		if(NULL == pGameObject)
			return E_FAIL;
		pLayer->Ready_Object(L"TombStone", pGameObject);
	}
		
	/*for (_int i = 0; i < 20; ++i)
	{
		pGameObject = CEffect::Create(m_pGraphicDev);
		if(NULL == pGameObject)
			return E_FAIL;
		pLayer->Ready_Object(L"Effect", pGameObject);
	}*/

	m_mapLayer.insert(MAPLAYER::value_type(L"GameLogic", pLayer));

	return S_OK;
}

_ulong CStageScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;	
}
