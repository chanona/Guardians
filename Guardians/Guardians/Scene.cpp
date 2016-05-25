#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
	m_ppShaders = NULL;
	m_nShaders = 0;

	m_pLights = NULL;
	m_pd3dcbLights = NULL;
}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	//이 쉐이더 객체에 대한 포인터들의 배열을 정의한다.
	m_nShaders = 2;
	m_ppShaders = new CShader*[SHADER_END];
	
	m_ppShaders[1] = new CSceneShader();
	m_ppShaders[1]->CreateShader(pd3dDevice);
	m_ppShaders[1]->BuildObjects(pd3dDevice);

	m_ppShaders[0] = new CAnimationShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	m_ppShaders[2] = new CBowShader();
	m_ppShaders[2]->CreateShader(pd3dDevice);
	//m_ppShaders[2]->BuildObjects(pd3dDevice);

	BuildLights(pd3dDevice);
}

void CScene::BuildBow(ID3D11Device *pd3dDevice)
{
	
}

void CScene::BuildLights(ID3D11Device* pd3dDevice)
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));

	// 게임 월드 전체를 비추는 주변조명 설정
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	// 3개의 조명(점 광원, 스팟 광원, 방향성 광원) 설정
	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 40.0f;
	m_pLights->m_pLights[0].m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvPosition = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.001f, 0.0001f);

	m_pLights->m_pLights[1].m_bEnable = 1.0f;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 100.0f;
	m_pLights->m_pLights[1].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	
	m_pLights->m_pLights[2].m_bEnable = 1.0f;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);

}
void CScene::UpdateLights(ID3D11DeviceContext* pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);	// 매핑 시킬 리소스 주소, 서브 리소스 인덱스 번호 , 액세스를 어떻게 할것인가, 0, 맵핑된 포인터를 받을 포인터 
	LIGHTS* pcbLight = (LIGHTS*)d3dMappedResource.pData;
	memcpy(pcbLight, m_pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::ReleaseObjects()
{



}
void CScene::ReleaseLights()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::ProcessInput()
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int j = 0; j < m_nShaders; j++) m_ppShaders[j]->AnimateObjects(fTimeElapsed);
}

void CScene::Render(ID3D11DeviceContext* pd3dDeviceContext,CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights)
	{
		// 두번쨰 조명은 플레이어가 가지고 있는 손전등이다. 그러므로 카메라의 위치가 바뀌면 조명의 위치와
		// 방향을 카메라의 위치와 방향으로 변경한다.
		D3DXVECTOR3 d3dxvCameraPosition = pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);

		m_pLights->m_pLights[1].m_d3dxvPosition = d3dxvCameraPosition;
		m_pLights->m_pLights[1].m_d3dxvDirection = pCamera->GetLookVector();

		m_pLights->m_pLights[2].m_d3dxvDirection = D3DXVECTOR3(-0.0f, -1.0f, -1.0f);
		UpdateLights(pd3dDeviceContext);
	}
	for (int i = 0; i < SHADER_END; ++i)
	{
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}
}