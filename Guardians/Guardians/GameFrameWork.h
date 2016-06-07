#pragma once

//-----------------------------------------------------------------------------
// File: GameFramework.h
//-----------------------------------------------------------------------------

#define FRAMEWORK	CGameFramework::GetInstance()

#ifndef _GAME_APPLICATION_FRAMEWORK_H_
#define _GAME_APPLICATION_FRAMEWORK_H_

#include "stdafx.h"
#include "Scene.h"
#include "Player.h"
#include "Monster.h"
#include "GameTimer.h"
#include "Bow.h"
#include "SkyBox.h"

#define FULLSCREEN_WIDTH	640
#define FULLSCREEN_HEIGHT	480

class CScene;
class CGameFramework
{
public:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
		
	//스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다.
	IDXGISwapChain *m_pDXGISwapChain;
	//렌더 타겟 뷰 인터페이스에 대한 포인터이다. 
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	// 깊이 버퍼 객체 인터페이스와 뷰에 대한 포인터이다.
	ID3D11Texture2D* m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView* m_pd3dDepthStencilView;

	
	//다음은 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열이다.
	_TCHAR m_pszBuffer[50];

	POINT m_ptOldCursorPos;



public:
	CGameFramework();
	~CGameFramework();

	static CGameFramework* GetInstance()
	{
		static CGameFramework instance;
		return &instance;
	}

	// 플레이어는 보통 하나지만 둘이상일수도 있으므로 리스트로 선언.
	//int m_nPlayers;
	//CPlayer** m_ppPlayers;
	CMonster* m_pMonster;
	vector<CBow*> m_vecBow;
	CSkyBox* m_pSkyBox;

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	//디바이스, 스왑 체인, 디바이스 컨텍스트, 디바이스와 관련된 뷰를 생성하는 함수이다. 
	
	bool CreateRenderTargetDepthStencilView();
	bool CreateDirect3DDisplay();
	

	//렌더링할 메쉬, 객체를 생성하고 소멸하는 함수이다. 
	void BuildObjects();
	void ReleaseObjects();

	//프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수이다. 
	void ProcessInput();
	void AnimateObjects();
	void Update();
	void FrameAdvance();

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	//디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요하다.
	ID3D11DeviceContext *m_pd3dDeviceContext;

	//디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다.
	ID3D11Device *m_pd3dDevice;

	//다음은 게임 프레임워크에서 사용할 타이머이다.
	CGameTimer m_GameTimer;

	//다음은 게임의 장면(Scene)을 관리하는 객체에 대한 포인터를 나타낸다.
	CScene *m_pScene;

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND GetHwnd() const { return m_hWnd; }
	CGameTimer* GetTimer() { return &m_GameTimer; }
};

#endif 

