//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "SONImporter.h"
#include "GameFrameWork.h"
#include "ClientNetEngine.h"
#include "PlayerManager.h"
#include "ObjectManager.h"
#include "MouseCol.h"
#include "HeightMapTerrain.h"

CGameFramework::CGameFramework()
{
	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;
	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	// new Code
	m_pScene = NULL;
	_tcscpy_s(m_pszBuffer, _T("LapProject ("));

	m_pSkyBox = nullptr;

	//m_nPlayers = 0;
	//m_ppPlayers = NULL;
}

CGameFramework::~CGameFramework()
{
}

//다음 함수는 응용 프로그램이 실행되면 호출된다는 것에 유의하라. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인 등을 생성하는 함수를 호출한다. 
	if (!CreateDirect3DDisplay()) return(false);

	//렌더링할 객체(게임 월드 객체)를 생성한다. 

	return(true);
}


bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	//스왑 체인의 후면버퍼에 대한 렌더 타겟 뷰를 생성한다.

	//스왑 체인의 첫 번째 후면버퍼 인터페이스를 가져온다. 
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	//스왑 체인의 첫 번째 후면버퍼에 대한 렌더 타겟 뷰를 생성한다.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create Depth Stencil Texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL,
		&m_pd3dDepthStencilBuffer))) return (false);
	
	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer,
		&d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return false;


	//생성된 렌더 타겟 뷰를 디바이스 컨텍스트에 연결한다.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);


	return(true);
}

bool CGameFramework::CreateDirect3DDisplay()
{
	// 먼저, 출력 윈도우의 클라이언트 영역의 크기를 멤버 변수에 저장한다
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	// D3D11CreateDeviceAndSwapChain() 함수를 통하여 
	// Direct3D 디바이스, 스왑 체인, 디바이스 컨텍스트를 동시에 생성하도록 하자.
	// 먼저, 디바이스 생성에 필요한 디바이스 생성 플래그, 드라이버 유형, 특성 레벨(Feature Level)를 설정한다.
	UINT dwCreateDeviceFlags = 0;
//#ifdef _DEBUG
	//dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
	
	//디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE
	}; 
	 

	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서를 나타낸다.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	// 스왑 체인을 생성하기 위한 스왑 체인 구조체 DXGI_SWAP_CHAIN_DESC를 설정한다.
	// 스왑 체인의 버퍼 크기는 주 윈도우의 클라이언트 영역의 크기로 설정하고 출력 윈도우는 주 윈도우로 설정한다. 
	// 스왑 체인의 출력 모드는 윈도우 모드로 설정한다.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;														// 스왑 체인에 사용할 버퍼의수
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;									// 후면 버퍼의 가로크기
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;								// 후면 버퍼의 세로크기
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// 후면 버퍼의 픽셀 형식
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;								// 화면 갱신 비율 ( 분모 )
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;								// 화면 갱신 비율 ( 분자 )
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// 후면버퍼에 대한 표면 사용 방식과 CPU 접근 선택사항 지정. 렌더타깃 출력으로 사용 , 후면버퍼에 렌더링을 할 것 
	dxgiSwapChainDesc.OutputWindow = m_hWnd;												// 렌더링이 일어날 출력윈도우
	dxgiSwapChainDesc.SampleDesc.Count = 1;													// 다중 샘플링을 하지 않겠다. ( 픽셀마다 다중 샘플 수 지정 )
	dxgiSwapChainDesc.SampleDesc.Quality = 0;												// 다중 샘플링을 하지 않겠다. ( 다중 샘플링 품질레벨 지정  )
	dxgiSwapChainDesc.Windowed = TRUE;														// 윈도우 모드

	D3D_DRIVER_TYPE nd3dDriverType		= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel  = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성한다. 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성한다.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		// [in]
		// 1. 어댑터 포인터 : NULL-디폴트 어댑터  2. 드라이버 타입  3. 소프트웨어 래스터라이저 구현 핸들 / 드라이버 타입이 HARDWARE 이기때문에 NULL
		// 4. 실행 레이어 설정 디바이스 생성 플래그 개발자에겐 D3D11_CREATE_DEVICE_DEBUG 가 좋다. - 디버그 정보 제공
		// 5. 생성할 디바이스의 특성 레벨 순서. 가장 높은 수준의 특성 레벨 - NULL
		// 6. 생성할 버퍼를 서술하는 D3D11_BUFFER_DESC 구조체의 포인터.
		// 7. SDK 버전 반드시 D3D11_SDK_VERSION
		// 8. 종합적인 스왑체인 Desc 구조체 
		// [out]
		// 9. 스왑체인 인터페이스 포인터 반환
		// 10. 디바이스 인터페이스 포인터 반환
		// 11. 성공적으로 디바이스를 생성하면 특성레벨 배열의 원소 반환
		// 12. 디바이스 컨텍스트 인터페이스 포인터반환.
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels,
			D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
 	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//렌더 타겟 뷰를 생성하는 함수를 호출한다.
	if (!CreateRenderTargetDepthStencilView()) return(false);
	
	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		// 마우스 캡쳐후 현재 마우스 위치를 가져옴
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);

		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		// 마우스 캡쳐 해제
		ReleaseCapture();

		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{

		case VK_F1:
			NETWORK_ENGINE->GetMyPlayer()->ChangeCamera(m_pd3dDevice, FIRST_PERSON_CAMERA
				, m_GameTimer.GetTimeElapsed());
			break;
		case VK_F2:
			NETWORK_ENGINE->GetMyPlayer()->ChangeCamera(m_pd3dDevice, THIRD_PERSON_CAMERA
				, m_GameTimer.GetTimeElapsed());
			break;

		case VK_F3:
			
			break;


		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;


		default:
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		/*윈도우의 크기가 변경될 때(현재는 “Alt+Enter“ 전체 화면 모드와 윈도우 모드로 전환될 때) 스왑 체인의 후면버퍼 크기를 조정하고 후면버퍼에 대한 렌더 타겟 뷰를 다시 생성한다. */
	/*case WM_SIZE:
	{
		m_nWndClientWidth  = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
		m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		if (NETWORK_ENGINE->GetMyPlayer())
		{
			CCamera *pCamera = NETWORK_ENGINE->GetMyPlayer()->GetCamera();
			pCamera->SetViewPort(m_pd3dDeviceContext, 0, 0,m_nWndClientWidth,
				m_nWndClientHeight, 0.0f, 1.0f);

		}
		break;
	}*/
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

//다음 함수는 응용 프로그램이 종료될 때 호출된다는 것에 유의하라. 
void CGameFramework::OnDestroy()
{
	//게임 객체를 소멸한다. 
	ReleaseObjects();

	//Direct3D와 관련된 객체를 소멸한다. 
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFramework::BuildObjects()
{
	// new Code 
	// CGameFramework“ 클래스의 BuildObjects() 멤버 함수를 다음과 같이 변경한다
	// CScene 클래스 객체를 생성하고 CScene 클래스 객체의 BuildObjects() 멤버 함수를 호출한다.
	//SONImporter::Instance()->InitGameModelData();															/// MeshData Load
	
	m_pScene = new CScene();
	
	//// 플레이어 객체를 생성한다.
	//m_nPlayers = 1;
	//m_ppPlayers = new CPlayer*[m_nPlayers];
	//
	//CCubePlayer* pCubePlayer = new CCubePlayer(m_pd3dDevice);
	//
	//// 1인칭 카메라로 변경
	//pCubePlayer->ChangeCamera(m_pd3dDevice, FIRST_PERSON_CAMERA, m_GameTimer.GetTimeElapsed());

	//// 카메라 객체를 생성하dy고 뷰 포트를 설정한다.
	//CCamera *pCamera = pCubePlayer->GetCamera();

	////pCamera->CreateShaderVariables(m_pd3dDevice);
	//pCamera->SetViewPort(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight,
	//	0.0f, 1.0f);
	//
	//// 투영 변환 행렬을 생성 한다.
	//pCamera->GenerateProjectionMatrix(1.0f, 5000.0f, float(m_nWndClientWidth) / float(m_nWndClientHeight), 60.0f);
	//pCamera->GenerateViewMatrix();
	////pCamera->CreateShaderVariables(m_pd3dDevice);

	//pCubePlayer->SetPosition(D3DXVECTOR3(0.0f, 10.0f, -500.0f));

	//m_ppPlayers[0] = pCubePlayer;

	if (m_pScene) 
		m_pScene->BuildObjects(m_pd3dDevice);
}

void CGameFramework::ReleaseObjects()
{
	// new Code
	// CGameFramework“ 클래스의 ReleaseObjects() 멤버 함수를 다음과 같이 변경한다. 
	// CScene 클래스 객체의 ReleaseObjects() 멤버 함수를 호출하고 CScene 클래스 객체를 반환한다. 

	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	//if (m_ppPlayers)
	//{
	//	for (int j = 0; j < m_nPlayers; j++) delete m_ppPlayers[j];
	//	delete[] m_ppPlayers;
	//}

	for (auto bow : m_vecBow)
	{
		delete bow;
	}

	delete m_pMonster;
}

void CGameFramework::ProcessInput()
{
	static DWORD preTime = GetTickCount();
	BYTE preDir = NETWORK_ENGINE->GetMyPlayer()->GetDirection();
	BYTE newDir = 0;

	bool bProcessedByScene = false;
	if (m_pScene) bProcessedByScene = m_pScene->ProcessInput();

	if (GetActiveWindow() == m_hWnd && !bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDircetion = 0;
		if (KEY_DOWN('A')) newDir |= Direction::LEFT;
		if (KEY_DOWN('D')) newDir |= Direction::RIGHT;
		if (KEY_DOWN('W')) newDir |= Direction::FORWARD;
		if (KEY_DOWN('S')) newDir |= Direction::BACKWARD;
		//if (KEY_DOWN(VK_LBUTTON)) 

		if (GetAsyncKeyState('A') == 0 || GetAsyncKeyState('A') == 1)   newDir &= ~Direction::LEFT;
		if (GetAsyncKeyState('D') == 0 || GetAsyncKeyState('D') == 1)   newDir &= ~Direction::RIGHT;
		if (GetAsyncKeyState('W') == 0 || GetAsyncKeyState('W') == 1)   newDir &= ~Direction::UP;
		if (GetAsyncKeyState('S') == 0 || GetAsyncKeyState('S') == 1)   newDir &= ~Direction::DOWN;
		
		if (preDir != newDir || (GetTickCount() - preTime >= 333))
		{
			if (newDir == Direction::STOP)
			{
				cs_packet_player_keyboard_move_stop pkt;
				pkt.size = sizeof(pkt);
				pkt.type = CS_KEYBOARD_MOVE_STOP;	// 나중에 스탑으로 바꿔야됨
				pkt.direction = newDir;
				pkt.x = NETWORK_ENGINE->GetMyPlayer()->GetPositionX();
				pkt.y = NETWORK_ENGINE->GetMyPlayer()->GetPositionY();
				pkt.z = NETWORK_ENGINE->GetMyPlayer()->GetPositionZ();
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Idle";

				NETWORK_ENGINE->SendPacket((char *)&pkt);
				//NETWORK_ENGINE->GetMyPlayer()->SetDirection(newDir);
			}
			else
			{
				cs_packet_player_keyboard_move_start pkt;
				pkt.size = sizeof(pkt);
				pkt.type = CS_KEYBOARD_MOVE_START;
				pkt.direction = newDir;
				pkt.x = NETWORK_ENGINE->GetMyPlayer()->GetPositionX();
				pkt.y = NETWORK_ENGINE->GetMyPlayer()->GetPositionY();
				pkt.z = NETWORK_ENGINE->GetMyPlayer()->GetPositionZ();
				pkt.speed = int(NETWORK_ENGINE->GetMyPlayer()->GetSpeed());
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Walk";
				pkt.time_stamp = GetTickCount();

				NETWORK_ENGINE->SendPacket((char *)&pkt);
			}
			NETWORK_ENGINE->GetMyPlayer()->SetDirection(newDir);
			preTime = GetTickCount();
		}

		//cout << NETWORK_ENGINE->GetMyPlayer()->GetPosition().x << ", " << NETWORK_ENGINE->GetMyPlayer()->GetPosition().y << " , " << NETWORK_ENGINE->GetMyPlayer()->GetPosition().z << endl;

		// 키보드 상태정보 반환 플레이어를 오른쪽왼쪽 / 앞 뒤로 이동.
		if (GetKeyboardState(pKeyBuffer))
		{
			if (pKeyBuffer['W'] & 0xF0) dwDircetion |= Direction::FORWARD;		
			if (pKeyBuffer['S'] & 0xF0) dwDircetion |= Direction::BACKWARD;		
			if (pKeyBuffer['A'] & 0xF0)	dwDircetion |= Direction::LEFT;
			if (pKeyBuffer['D'] & 0xF0) dwDircetion |= Direction::RIGHT;
			
			if (pKeyBuffer['1'] & 0xF0)
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Idle";
			if (pKeyBuffer['2'] & 0xF0)
			{
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Attack";
				
				m_pScene->BuildBow(m_pd3dDevice);
				/*CBow* pBow = new CBow;
				pBow->SetSkinned(skinnedModel);

				pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pBow->m_pSkinnedModel->m_vVertices[0], pBow->m_pSkinnedModel->m_vVertices.size());
				pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pBow->m_pSkinnedModel->m_vIndices[0], pBow->m_pSkinnedModel->m_vIndices.size());

				pBow->SetMesh(pTestMesh);
				pBow->SetTexture(ppTextures[0]);
				pBow->SetMaterial(ppMaterials[0]);*/

				//FRAMEWORK->m_vecBow.push_back(pBow);
			}
				
			if (pKeyBuffer['3'] & 0xF0)
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Walk";
			if (pKeyBuffer['4'] & 0xF0)
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Hit";
			if (pKeyBuffer['5'] & 0xF0)
				NETWORK_ENGINE->GetMyPlayer()->clipName = "Die";

			//if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDircetion |= DIR_UP;
			//if (pKeyBuffer[VK_NEXT] & 0xF0) dwDircetion |= DIR_DOWN;
		}

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;
		// 마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는가 계산
		// 마우스 왼쪽 또는 오른쪽 버튼이 눌러질때 메시지 처리할때 마우스 캡쳐함
		// 그러므로 마우스가 캡쳐된 것은 마우스 버튼이 눌러진 상태의미
		// 마우스 좌우 또는 상하로 움직이면 플레이어 x축 y축으로 회전
		if (GetCapture() == m_hWnd)
		{
			// 마우스 커서를 화면에서 없앤다(보이지 않게함)
			SetCursor(NULL);
			// 현재 마우스 커서의 위치를 가져옴
			GetCursorPos(&ptCursorPos);
			// 마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인양 구함
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		// 플레이어를 이동(dwDirection)하거나 회전한다(cxDelta, cyDelta)
		if (/*(dwDircetion != 0) ||*/ (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				// cxDelta는 y축의 회전을 나타내고, cyDelta는 x축의 회전을 나타냄
				// 오른쪽 마우스 버튼이 눌려진 경우 cxDelta는 z축 회전을 나타냄
				if (KEY_DOWN(VK_RBUTTON) & 0xF0)	NETWORK_ENGINE->GetMyPlayer()->Rotate(cyDelta, 0.0f, -cxDelta);
				else								NETWORK_ENGINE->GetMyPlayer()->Rotate(cyDelta, cxDelta, 0.0f);
			}
			// 플레이어를 dwDirection 방향으로 이동 ( 실제로는 속도 벡터 변경)
			// 이동거리는 시간에 비례하도록 한다. 이동속력은 500/초로 가정 만약 플레이어의 이동속력이 있다면
			// 그값을 이용한다.
			//if (dwDircetion) NETWORK_ENGINE->GetMyPlayer()->Move(dwDircetion, 500.0f* m_GameTimer.GetTimeElapsed(), true);
		}
		if (KEY_DOWN(VK_LBUTTON) & 0xF0)
		{
			// 여기서 보우 쉐이더 생성 
			
		}
	}
	// 플레이어를 실제로 이동하고 카메라를 갱신한다. 중력과 마찰력의 영향을 속도 벡터에 적용

	for (auto& player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (!player.second->IsConnected()) continue;

		if (player.second->GetID() == NETWORK_ENGINE->GetID())
		{
			player.second->Move(newDir);
			player.second->Update(m_GameTimer.GetTimeElapsed());
		}
		else
		{
			player.second->OtherMove();
		}
	}	
	int i = 0;

	for (auto& monster : OBJECT_MANAGER->GetMonsterMap())
	{
		if (!monster.second->GetAlive()) continue;;

		monster.second->Move();

		//i++;
	}

	for (auto& bow : m_vecBow)
	{
		//cout << bow->GetPosition().x << "," << bow->GetPosition().y << "," << bow->GetPosition().z << "," << endl;
		bow->Move(D3DXVECTOR3(0.f, 0.f, 0.f));

		/*if (D3DXVec3Length(&vDir) < 10.f)
		{
			cout << "충돌" << endl;
		}*/
	}


	

	//cout << i << endl;
}

void CGameFramework::AnimateObjects()
{
	// new Code
	// ”CGameFramework“ 클래스의 AnimateObjects() 멤버 함수를 다음과 같이 변경한다.
	// 타이머에서 마지막 프레임 이후 경과된 시간을 파라메터로 전달하여 CScene 클래스 객체의 AnimateObjects() 멤버 함수를 호출한다. 

	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::Update()
{
}

void CGameFramework::FrameAdvance()
{
	CPlayer *my_player = NETWORK_ENGINE->GetMyPlayer();
	// CGameFramework“ 클래스의 FrameAdvance() 멤버 함수를 다음과 같이 변경한다. 
	// 타이머의 시간이 갱신되도록 하고 프레임 레이트를 계산한다.
	m_GameTimer.Tick();

	ProcessInput();

	AnimateObjects();

	float fClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);

	CCamera *pCamera = NULL;
	
	if (my_player && my_player->IsConnected())
	{
		pCamera = my_player->GetCamera();
		my_player->UpdateShaderVariables(m_pd3dDeviceContext);
		//cout << my_player->GetPosition().x << " , " << my_player->GetPosition().y << " " << my_player->GetPosition().z << endl;
	}

	m_pScene->Render(m_pd3dDeviceContext, pCamera);

	//for (int i = 0; i < m_nPlayers; ++i){
	//	if (m_ppPlayers[i]){
	//		m_ppPlayers[i]->UpdateShaderVariables(m_pd3dDeviceContext);
	//		pCamera = m_ppPlayers[i]->GetCamera();
	//	}
	//	if (m_pScene) m_pScene->Render(m_pd3dDeviceContext, pCamera);

	//	// 3인칭 카메라일 때 플레이어를 렌더링한다.
	//	//for (int j = 0; j < m_nPlayers; ++j)
	//	//	if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);
	//}
		
	m_pDXGISwapChain->Present(0, 0);

	/*현재의 프레임 레이트를 문자열로 가져와서 주 윈도우의 타이틀로 출력한다. m_pszBuffer 문자열이 "LapProject ("으로 초기화되었으므로 (m_pszBuffer+12)에서부터 프레임 레이트를 문자열로 출력하여 “ FPS)” 문자열과 합친다.
	_itow_s(m_nCurrentFrameRate, (m_pszBuffer+12), 37, 10);
	wcscat_s((m_pszBuffer+12), 37, _T(" FPS)"));
	*/
	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}
