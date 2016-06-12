#include "stdafx.h"
#include "MainApp.h"
#include "SceneSelector.h"
#include "Export_Function.h"

CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{

}

HRESULT	CMainApp::Initialize(void)
{
	// 장치를 초기화한다.
	if(FAILED(Engine::Ready_GraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINSIZEX, WINSIZEY)))
	{
		MSG_BOX("Ready_GraphicDev Failed");
		return E_FAIL;
	}

	if(FAILED(Engine::Ready_Management(Engine::Get_GraphicDev())))
	{
		MSG_BOX("Ready_Management Failed");
		return E_FAIL;
	}
	
	if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_LOGO))))
	{
		MSG_BOX("Change_Scene(SCENE_LOGO) Failed");
		return E_FAIL;
	}

	return S_OK;
}

_int CMainApp::Update(const _float& fTimeDelta)
{
	return Engine::Update_Management(fTimeDelta);
}

void CMainApp::Render(void)
{
	Engine::Render_Management();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;

	if(FAILED(pMainApp->Initialize()))
	{
		MSG_BOX("CMainApp Create Failed");
		::Safe_Release(pMainApp);
	}

	return pMainApp;
}

_ulong CMainApp::Release(void)
{
	_ulong dwRefCnt = CBase::Release();

	if(0 == dwRefCnt)
	{
		Engine::Release_Utility();
		Engine::Release_Resource();		
		Engine::Release_System();
		delete this;
	}

	return dwRefCnt;
}


