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
		
	//���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	IDXGISwapChain *m_pDXGISwapChain;
	//���� Ÿ�� �� �������̽��� ���� �������̴�. 
	ID3D11RenderTargetView *m_pd3dRenderTargetView;
	// ���� ���� ��ü �������̽��� �信 ���� �������̴�.
	ID3D11Texture2D* m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView* m_pd3dDepthStencilView;

	
	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�.
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

	// �÷��̾�� ���� �ϳ����� ���̻��ϼ��� �����Ƿ� ����Ʈ�� ����.
	//int m_nPlayers;
	//CPlayer** m_ppPlayers;
	CMonster* m_pMonster;
	vector<CBow*> m_vecBow;
	CSkyBox* m_pSkyBox;

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();
	//����̽�, ���� ü��, ����̽� ���ؽ�Ʈ, ����̽��� ���õ� �並 �����ϴ� �Լ��̴�. 
	
	bool CreateRenderTargetDepthStencilView();
	bool CreateDirect3DDisplay();
	

	//�������� �޽�, ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�. 
	void BuildObjects();
	void ReleaseObjects();

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�. 
	void ProcessInput();
	void AnimateObjects();
	void Update();
	void FrameAdvance();

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	//����̽� ���ؽ�Ʈ�� ���� �������̴�. �ַ� ���������� ������ �ϱ� ���Ͽ� �ʿ��ϴ�.
	ID3D11DeviceContext *m_pd3dDeviceContext;

	//����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	ID3D11Device *m_pd3dDevice;

	//������ ���� �����ӿ�ũ���� ����� Ÿ�̸��̴�.
	CGameTimer m_GameTimer;

	//������ ������ ���(Scene)�� �����ϴ� ��ü�� ���� �����͸� ��Ÿ����.
	CScene *m_pScene;

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND GetHwnd() const { return m_hWnd; }
	CGameTimer* GetTimer() { return &m_GameTimer; }
};

#endif 

