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

//���� �Լ��� ���� ���α׷��� ����Ǹ� ȣ��ȴٴ� �Ϳ� �����϶�. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	//Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. 
	if (!CreateDirect3DDisplay()) return(false);

	//�������� ��ü(���� ���� ��ü)�� �����Ѵ�. 

	return(true);
}


bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	//���� ü���� �ĸ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.

	//���� ü���� ù ��° �ĸ���� �������̽��� �����´�. 
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	//���� ü���� ù ��° �ĸ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
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


	//������ ���� Ÿ�� �並 ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);


	return(true);
}

bool CGameFramework::CreateDirect3DDisplay()
{
	// ����, ��� �������� Ŭ���̾�Ʈ ������ ũ�⸦ ��� ������ �����Ѵ�
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	// D3D11CreateDeviceAndSwapChain() �Լ��� ���Ͽ� 
	// Direct3D ����̽�, ���� ü��, ����̽� ���ؽ�Ʈ�� ���ÿ� �����ϵ��� ����.
	// ����, ����̽� ������ �ʿ��� ����̽� ���� �÷���, ����̹� ����, Ư�� ����(Feature Level)�� �����Ѵ�.
	UINT dwCreateDeviceFlags = 0;
//#ifdef _DEBUG
	//dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
	
	//����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE
	}; 
	 

	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//����̽��� �����ϱ� ���Ͽ� �õ��� Ư�� ������ ������ ��Ÿ����.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	// ���� ü���� �����ϱ� ���� ���� ü�� ����ü DXGI_SWAP_CHAIN_DESC�� �����Ѵ�.
	// ���� ü���� ���� ũ��� �� �������� Ŭ���̾�Ʈ ������ ũ��� �����ϰ� ��� ������� �� ������� �����Ѵ�. 
	// ���� ü���� ��� ���� ������ ���� �����Ѵ�.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;														// ���� ü�ο� ����� �����Ǽ�
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;									// �ĸ� ������ ����ũ��
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;								// �ĸ� ������ ����ũ��
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// �ĸ� ������ �ȼ� ����
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;								// ȭ�� ���� ���� ( �и� )
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;								// ȭ�� ���� ���� ( ���� )
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// �ĸ���ۿ� ���� ǥ�� ��� ��İ� CPU ���� ���û��� ����. ����Ÿ�� ������� ��� , �ĸ���ۿ� �������� �� �� 
	dxgiSwapChainDesc.OutputWindow = m_hWnd;												// �������� �Ͼ ���������
	dxgiSwapChainDesc.SampleDesc.Count = 1;													// ���� ���ø��� ���� �ʰڴ�. ( �ȼ����� ���� ���� �� ���� )
	dxgiSwapChainDesc.SampleDesc.Quality = 0;												// ���� ���ø��� ���� �ʰڴ�. ( ���� ���ø� ǰ������ ����  )
	dxgiSwapChainDesc.Windowed = TRUE;														// ������ ���

	D3D_DRIVER_TYPE nd3dDriverType		= D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel  = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�. ������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		// [in]
		// 1. ����� ������ : NULL-����Ʈ �����  2. ����̹� Ÿ��  3. ����Ʈ���� �����Ͷ����� ���� �ڵ� / ����̹� Ÿ���� HARDWARE �̱⶧���� NULL
		// 4. ���� ���̾� ���� ����̽� ���� �÷��� �����ڿ��� D3D11_CREATE_DEVICE_DEBUG �� ����. - ����� ���� ����
		// 5. ������ ����̽��� Ư�� ���� ����. ���� ���� ������ Ư�� ���� - NULL
		// 6. ������ ���۸� �����ϴ� D3D11_BUFFER_DESC ����ü�� ������.
		// 7. SDK ���� �ݵ�� D3D11_SDK_VERSION
		// 8. �������� ����ü�� Desc ����ü 
		// [out]
		// 9. ����ü�� �������̽� ������ ��ȯ
		// 10. ����̽� �������̽� ������ ��ȯ
		// 11. ���������� ����̽��� �����ϸ� Ư������ �迭�� ���� ��ȯ
		// 12. ����̽� ���ؽ�Ʈ �������̽� �����͹�ȯ.
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels,
			D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
 	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//���� Ÿ�� �並 �����ϴ� �Լ��� ȣ���Ѵ�.
	if (!CreateRenderTargetDepthStencilView()) return(false);
	
	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		// ���콺 ĸ���� ���� ���콺 ��ġ�� ������
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);

		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		// ���콺 ĸ�� ����
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
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
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

//���� �Լ��� ���� ���α׷��� ����� �� ȣ��ȴٴ� �Ϳ� �����϶�. 
void CGameFramework::OnDestroy()
{
	//���� ��ü�� �Ҹ��Ѵ�. 
	ReleaseObjects();

	//Direct3D�� ���õ� ��ü�� �Ҹ��Ѵ�. 
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFramework::BuildObjects()
{
	// new Code 
	// CGameFramework�� Ŭ������ BuildObjects() ��� �Լ��� ������ ���� �����Ѵ�
	// CScene Ŭ���� ��ü�� �����ϰ� CScene Ŭ���� ��ü�� BuildObjects() ��� �Լ��� ȣ���Ѵ�.
	//SONImporter::Instance()->InitGameModelData();															/// MeshData Load
	
	m_pScene = new CScene();
	
	//// �÷��̾� ��ü�� �����Ѵ�.
	//m_nPlayers = 1;
	//m_ppPlayers = new CPlayer*[m_nPlayers];
	//
	//CCubePlayer* pCubePlayer = new CCubePlayer(m_pd3dDevice);
	//
	//// 1��Ī ī�޶�� ����
	//pCubePlayer->ChangeCamera(m_pd3dDevice, FIRST_PERSON_CAMERA, m_GameTimer.GetTimeElapsed());

	//// ī�޶� ��ü�� ������dy�� �� ��Ʈ�� �����Ѵ�.
	//CCamera *pCamera = pCubePlayer->GetCamera();

	////pCamera->CreateShaderVariables(m_pd3dDevice);
	//pCamera->SetViewPort(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight,
	//	0.0f, 1.0f);
	//
	//// ���� ��ȯ ����� ���� �Ѵ�.
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
	// CGameFramework�� Ŭ������ ReleaseObjects() ��� �Լ��� ������ ���� �����Ѵ�. 
	// CScene Ŭ���� ��ü�� ReleaseObjects() ��� �Լ��� ȣ���ϰ� CScene Ŭ���� ��ü�� ��ȯ�Ѵ�. 

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
				pkt.type = CS_KEYBOARD_MOVE_STOP;	// ���߿� ��ž���� �ٲ�ߵ�
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

		// Ű���� �������� ��ȯ �÷��̾ �����ʿ��� / �� �ڷ� �̵�.
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
		// ���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��°� ���
		// ���콺 ���� �Ǵ� ������ ��ư�� �������� �޽��� ó���Ҷ� ���콺 ĸ����
		// �׷��Ƿ� ���콺�� ĸ�ĵ� ���� ���콺 ��ư�� ������ �����ǹ�
		// ���콺 �¿� �Ǵ� ���Ϸ� �����̸� �÷��̾� x�� y������ ȸ��
		if (GetCapture() == m_hWnd)
		{
			// ���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ���)
			SetCursor(NULL);
			// ���� ���콺 Ŀ���� ��ġ�� ������
			GetCursorPos(&ptCursorPos);
			// ���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� �����ξ� ����
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		// �÷��̾ �̵�(dwDirection)�ϰų� ȸ���Ѵ�(cxDelta, cyDelta)
		if (/*(dwDircetion != 0) ||*/ (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				// cxDelta�� y���� ȸ���� ��Ÿ����, cyDelta�� x���� ȸ���� ��Ÿ��
				// ������ ���콺 ��ư�� ������ ��� cxDelta�� z�� ȸ���� ��Ÿ��
				if (KEY_DOWN(VK_RBUTTON) & 0xF0)	NETWORK_ENGINE->GetMyPlayer()->Rotate(cyDelta, 0.0f, -cxDelta);
				else								NETWORK_ENGINE->GetMyPlayer()->Rotate(cyDelta, cxDelta, 0.0f);
			}
			// �÷��̾ dwDirection �������� �̵� ( �����δ� �ӵ� ���� ����)
			// �̵��Ÿ��� �ð��� ����ϵ��� �Ѵ�. �̵��ӷ��� 500/�ʷ� ���� ���� �÷��̾��� �̵��ӷ��� �ִٸ�
			// �װ��� �̿��Ѵ�.
			//if (dwDircetion) NETWORK_ENGINE->GetMyPlayer()->Move(dwDircetion, 500.0f* m_GameTimer.GetTimeElapsed(), true);
		}
		if (KEY_DOWN(VK_LBUTTON) & 0xF0)
		{
			// ���⼭ ���� ���̴� ���� 
			
		}
	}
	// �÷��̾ ������ �̵��ϰ� ī�޶� �����Ѵ�. �߷°� �������� ������ �ӵ� ���Ϳ� ����

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
			cout << "�浹" << endl;
		}*/
	}


	

	//cout << i << endl;
}

void CGameFramework::AnimateObjects()
{
	// new Code
	// ��CGameFramework�� Ŭ������ AnimateObjects() ��� �Լ��� ������ ���� �����Ѵ�.
	// Ÿ�̸ӿ��� ������ ������ ���� ����� �ð��� �Ķ���ͷ� �����Ͽ� CScene Ŭ���� ��ü�� AnimateObjects() ��� �Լ��� ȣ���Ѵ�. 

	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::Update()
{
}

void CGameFramework::FrameAdvance()
{
	CPlayer *my_player = NETWORK_ENGINE->GetMyPlayer();
	// CGameFramework�� Ŭ������ FrameAdvance() ��� �Լ��� ������ ���� �����Ѵ�. 
	// Ÿ�̸��� �ð��� ���ŵǵ��� �ϰ� ������ ����Ʈ�� ����Ѵ�.
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

	//	// 3��Ī ī�޶��� �� �÷��̾ �������Ѵ�.
	//	//for (int j = 0; j < m_nPlayers; ++j)
	//	//	if (m_ppPlayers[j]) m_ppPlayers[j]->Render(m_pd3dDeviceContext);
	//}
		
	m_pDXGISwapChain->Present(0, 0);

	/*������ ������ ����Ʈ�� ���ڿ��� �����ͼ� �� �������� Ÿ��Ʋ�� ����Ѵ�. m_pszBuffer ���ڿ��� "LapProject ("���� �ʱ�ȭ�Ǿ����Ƿ� (m_pszBuffer+12)�������� ������ ����Ʈ�� ���ڿ��� ����Ͽ� �� FPS)�� ���ڿ��� ��ģ��.
	_itow_s(m_nCurrentFrameRate, (m_pszBuffer+12), 37, 10);
	wcscat_s((m_pszBuffer+12), 37, _T(" FPS)"));
	*/
	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}
