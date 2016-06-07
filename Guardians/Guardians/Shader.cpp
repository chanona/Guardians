#include "stdafx.h"
#include "Shader.h"
#include "SONImporter.h"
#include <atlbase.h>
#include "GameFrameWork.h"
#include "PlayerManager.h"
#include "ObjectManager.h"
#include "Bow.h"
#include "SkyBox.h"

CShader::CShader()
{
	m_pd3dcbWorldMatrix = NULL;
	m_pd3dcbMaterial = NULL;
	m_pd3dcbBoneTransforms = NULL;

	//m_ppObjects = NULL;
	//m_nObjects = 0;

	m_pd3dVertexShader = NULL;
	m_pd3dPixelShader = NULL;
	m_pd3dVertexLayout = NULL;
}

CShader::~CShader()
{
	if (m_pd3dVertexShader) m_pd3dVertexShader->Release();
	if (m_pd3dPixelShader) m_pd3dPixelShader->Release();
	if (m_pd3dGeometryShader) m_pd3dGeometryShader->Release();
	if (m_pd3dVertexLayout) m_pd3dVertexLayout->Release();
}

void CShader::BuildObjects(ID3D11Device* pd3dDevice)
{

}
void CShader::ReleaseObjects()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	if (m_pd3dcbMaterial) m_pd3dcbMaterial->Release();
	if (m_pd3dcbBoneTransforms) m_pd3dcbBoneTransforms->Release();

	//if (FRAMEWORK->GetInstance()->m_ppPlayers)
	//{
	//	for (int j = 0; j < FRAMEWORK->GetInstance()->m_nPlayers; j++) delete FRAMEWORK->GetInstance()->m_ppPlayers[j];
	//	delete[] FRAMEWORK->GetInstance()->m_ppPlayers;
	//}
}
void CShader::AnimateObjects(float fTimeElapsed)
{
	for (auto& player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (!player.second->IsConnected()) continue;
		player.second->Animate(fTimeElapsed, TEXTYPE_DYNAMIC);
	}

	for(auto& monster : OBJECT_MANAGER->GetMonsterMap())
	{
		if(!monster.second->GetAlive()) continue;
		monster.second->Animate(fTimeElapsed, TEXTYPE_DYNAMIC);
	}

	/*for (auto& bow : FRAMEWORK->m_vecBow)
	{
		bow->Animate(fTimeElapsed, TEXTYPE_STATIC);
	}*/

	/*for (int j = 0; j < FRAMEWORK->GetInstance()->m_nPlayers; j++)
	{
		FRAMEWORK->GetInstance()->m_ppPlayers[j]->Animate(fTimeElapsed);
	}*/
	//FRAMEWORK->GetInstance()->m_pMonster->Animate(fTimeElapsed); 
}

// CShader 클래스의 CreateVertexShaderFromFile() 멤버 함수를 다음과 같이 정의한다.
// 이 함수는 쉐이더 파일에서 정점 - 쉐이더와 입력 - 레이아웃를 생성한다.
void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	// 구식문법을 엄밀히 체크해서 금지한다(디폴트는 체크하지 않음)
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	// 컴파일된 바이트코드는 임의의 길이를 가지는 데이터를 리턴하기 위해 사용 하는 인터페이스포인터
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된
	쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 정점-쉐이더를 생성한다. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//컴파일된 쉐이더 코드의 메모리 주소와 입력 레이아웃에서 정점 레이아웃을 생성한다. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

// CShader 클래스의 CreatePixelShaderFromFile() 멤버 함수를 다음과 같이 정의한다
// 이 함수는 쉐이더 파일에서 픽셀 - 쉐이더를 생성한다.
void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*파일(pszFileName)에서 쉐이더 함수(pszShaderName)를 컴파일하여 컴파일된 쉐이더 코드의 메모리 주소(pd3dShaderBlob)를 반환한다.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//컴파일된 쉐이더 코드의 메모리 주소에서 픽셀-쉐이더를 생성한다. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

// CShader 클래스의 CreateShader() 멤버 함수와 Render() 멤버 함수를 다음과 같이 정의한다.
void CShader::CreateShader(ID3D11Device *pd3dDevice)
{
}
void CShader::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);

	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext,D3DXMATRIX* pd3dxmtxWorld)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_WORLD_MATRIX* pcbWorldMatrix = (VS_CB_WORLD_MATRIX*)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
}

void CShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{

}
void CShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, CTexture* pTexture)
{
	/// 텍스쳐관련 업데이트
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_TEXTURE, pTexture->m_nTextures, pTexture->m_ppd3dsrvTextures);
	pd3dDeviceContext->PSSetSamplers(PS_SLOT_SAMPLER_STATE, pTexture->m_nTextures, pTexture->m_ppd3dSamplerStates);
}

// 쉐이더 렌더링 함수를 정의한다.
void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera)
{
	//객체 렌더링
	//for (auto& bow : FRAMEWORK->m_vecBow)
	//{
	//	UpdateShaderVariables(pd3dDeviceContext, &bow->m_d3dxmtxWorld);
	//	UpdateShaderVariables(pd3dDeviceContext, &bow->m_pMaterial->m_Material);
	//	UpdateShaderVariables(pd3dDeviceContext, bow->m_pTexture);
	//	//정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	//}

	//if (!(FRAMEWORK->m_vecBow.empty()))
	//{
	//	////정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	//	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	//	for (auto& bow : FRAMEWORK->m_vecBow)
	//	{
	//		CShader::UpdateShaderVariables(pd3dDeviceContext, &bow->m_d3dxmtxWorld);
	//		UpdateShaderVariables(pd3dDeviceContext, &bow->m_d3dxmtxWorld);
	//		UpdateShaderVariables(pd3dDeviceContext, &bow->m_pMaterial->m_Material);
	//		CShader::UpdateShaderVariables(pd3dDeviceContext, bow->m_pTexture);
	//		bow->Render(pd3dDeviceContext);
	//	}
	//}

	for (auto& player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (player.second->IsConnected())
		{
			UpdateShaderVariables(pd3dDeviceContext, &player.second->m_d3dxmtxWorld);
			UpdateShaderVariables(pd3dDeviceContext, &player.second->m_pMaterial->m_Material);
			UpdateShaderVariables(pd3dDeviceContext, player.second->m_pTexture);
			//정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
			if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
			//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
			if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
			//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
			if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
		}
	}

	//for (int j = 0; j < FRAMEWORK->GetInstance()->m_nPlayers; j++)
	//{
	//	if (FRAMEWORK->GetInstance()->m_ppPlayers[j])
	//	{
	//		// 객체의 재질(Material) 정보를 쉐이더 프로그램으로 전달한다.
	//		UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_d3dxmtxWorld);
	//		if (FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pMaterial->m_Material);
	//		if (FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pTexture) UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pTexture);
	//		//m_ppObjects[j]->Render(pd3dDeviceContext);
	//	}
	//}

	//if (FRAMEWORK->GetInstance()->m_pMonster->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_pMonster->m_pMaterial->m_Material);
	//if (FRAMEWORK->GetInstance()->m_pMonster->m_pTexture) UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_pMonster->m_pTexture);
}
/****************************************************************************************/
/*********************************** CIlluminatedShader ********************************/
/****************************************************************************************/
CIlluminatedShader::CIlluminatedShader()
{
}
CIlluminatedShader::~CIlluminatedShader()
{
}

void CIlluminatedShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);
}

void CIlluminatedShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}

void CIlluminatedShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL* pcbMaterial = (MATERIAL*)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CIlluminatedShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera)
{
	CShader::Render(pd3dDeviceContext, pCamera);
}
void CIlluminatedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}
void CIlluminatedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}
void CIlluminatedShader::CreateShader(ID3D11Device* pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSLightingColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSLightingColor", "ps_4_0", &m_pd3dPixelShader);
}
void CIlluminatedShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	// 객체들의 물질의 색상은 빨강색 / 녹색 / 파란색 
	CMaterial** ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ppMaterials[1] = new CMaterial();
	ppMaterials[1]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[1]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ppMaterials[2] = new CMaterial();
	ppMaterials[2]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 0.0f, 1.0f, 10.0f);
	ppMaterials[2]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	CCubeMeshIlluminated* pCubeMeshIlluminated = new CCubeMeshIlluminated(pd3dDevice, 12.0f, 12.0f, 12.0f);
	CSphereMeshIlluminated* pSphereMeshIlluminated = new CSphereMeshIlluminated(pd3dDevice, 12.0f, 40, 40);

	int xObjects = 5, yObjects = 5, zObjects = 5, i = 0;
	//m_nObjects = (xObjects + 1) * (yObjects + 1) *(zObjects + 1);
	//m_ppObjects = new CGameObject*[m_nObjects];
	
	float fxPitch = 12.0f *1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;

	CRotatingObject* pRotatingObject = NULL;
	for (int x = -xObjects; x <= xObjects; x += 2)
	{
		for (int y = -yObjects; y <= yObjects; y += 2)
		{
			for (int z = -zObjects; z <= zObjects; z += 2)
			{
				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh((i % 2) ? (CMesh*)pCubeMeshIlluminated : (CMesh*)pSphereMeshIlluminated);
				pRotatingObject->SetMaterial(ppMaterials[i % 3]);
				pRotatingObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
				//m_ppObjects[i++] = pRotatingObject;

			}
		}
	}
	CreateShaderVariables(pd3dDevice);
	
	delete[] ppMaterials;
}

/****************************************************************************************/
/*********************************** CDiffsedShader ************************************/
/****************************************************************************************/

CDiffusedShader::CDiffusedShader()
{

}
CDiffusedShader::~CDiffusedShader()
{

}

void CDiffusedShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	// 인자 Error
	CCubeMesh* pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f, D3DCOLOR_XRGB(0, 0, 128));

	int xObjects = 6, yObjects = 6, zObjects = 6, i = 0;
	//m_nObjects = (xObjects + ((xObjects % 2) ? 0 : 1)) * (yObjects + ((yObjects % 2) ? 0 : 1)) * (zObjects + ((zObjects % 2) ? 0 : 1));
	//m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;
	
	CRotatingObject* pRotatingObject = NULL;
	for (int x = -xObjects; x <= xObjects; x += 2)
	{
		for (int y = -yObjects; y <= yObjects; y += 2)
		{
			for (int z = -zObjects; z <= zObjects; z += 2)
			{
				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pCubeMesh);
				pRotatingObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
				pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
				//m_ppObjects[i++] = pRotatingObject;
			}
		}
	}
	CreateShaderVariables(pd3dDevice);
}
void CDiffusedShader::ReleaseObjects()
{
	CShader::ReleaseObjects();
}
void CDiffusedShader::AnimateObjects(float fTimeElapsed)
{
	CShader::AnimateObjects(fTimeElapsed);
}


void CDiffusedShader::CreateShader(ID3D11Device* pd3dDevice)
{
	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSDiffusedColor",
		"vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSDiffusedColor",
		"ps_4_0", &m_pd3dPixelShader);
}
void CDiffusedShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);
}
void CDiffusedShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext,pd3dxmtxWorld);
}
void CDiffusedShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera)
{
	CShader::Render(pd3dDeviceContext,pCamera);
}

/***************************************************************************************/
/************************************ Scene 쉐이더 *************************************/
/***************************************************************************************/

// CObjectShader 클래스의 생성자와 소멸자를 다음과 같이 정의한다.
CSceneShader::CSceneShader()
{
	m_pd3dcbWorldMatrix = NULL;
	//m_ppObjects   = NULL;
	//m_nObjects    = 0;
}

CSceneShader::~CSceneShader()
{

}

void CSceneShader::CreateShader(ID3D11Device *pd3dDevice)
{
	CDiffusedShader::CreateShader(pd3dDevice);
}

void CSceneShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CDiffusedShader::CreateShaderVariables(pd3dDevice);
	
	// 월드 변환 행렬을 위한 상수 버퍼를 생성한다.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_WORLD_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbWorldMatrix);
}

void CSceneShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext,
	D3DXMATRIX* pd3dxmtxWorld)
{
	CDiffusedShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
	
	// 월드 변환 행렬을 상수버퍼에 복사한다.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&d3dMappedResource);
	VS_CB_WORLD_MATRIX* pcbWorldMatrix = (VS_CB_WORLD_MATRIX*)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	
	// 상수 버퍼를 디바이스의 슬롯(VS_SLOT_WORLD_MATRIX)에 연결한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
	
}

void CSceneShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	//// 가로 세로 높이가 12x12x12인 정육면체 메쉬를 생성함.
	//CCubeMesh* pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f);

	//int xObjects = 20, yObjects = 0, zObjects = 20, i = 0;
	//// x축 y축 z축으로 23개씩 총 23x23x23 = 12167개의 정육면체 배치.
	//
	//m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1);
	//m_ppObjects = new CGameObject*[m_nObjects];
	//
	//float fxPitch = 12.0f;
	//float fyPitch = 12.0f;
	//float fzPitch = 12.0f;
	//
	////정육면체 객체는 자전한다.

	////CRotatingObject* pRotatingObject = NULL;
	//CGameObject* pObject = new CGameObject();


	//for (int x = -xObjects; x <= xObjects; ++x){
	//	for (int y = -yObjects; y <= yObjects; ++y){
	//		for (int z = -zObjects; z <= zObjects; ++z){

	//			pObject = new CRotatingObject();
	//			pObject->SetMesh(pCubeMesh);
	//			// 각 정육면체 객체의 위치를 설정한다
	//			pObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
	//			//// 각 정육면체 객체의 회전축을 y축으로 설정
	//			//pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//			//pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
	//			m_ppObjects[i++] = pObject;
	//		}
	//	}
	//}

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 8배, y-축은 2배 확대한다.
	D3DXVECTOR3 d3dxvScale(100.f, 5.0f, 100.f);
	D3DXCOLOR d3dxColor(0.0f, 0.2f, 0.0f, 0.0f);

	//지형을 높이 맵 이미지 파일을 사용하여 생성한다. 
	//지형을 구성하는 격자 메쉬의 개수는 총 256(16x16)개가 된다.
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("..\\Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);

	CreateShaderVariables(pd3dDevice);
}

void CSceneShader::ReleaseObjects()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
	
	/*if (m_ppObjects)
	{
		for (int j = 0; j < m_nObjects; ++j) delete m_ppObjects[j];
		
		delete[] m_ppObjects;
	}*/
	if (m_pTerrain) delete m_pTerrain;
}
void CSceneShader::AnimateObjects(float fTimeElapsed)
{
	//for (int j = 0; j < m_nObjects; ++j)
	//	m_ppObjects[j]->Animate(fTimeElapsed);
}

// CObjectShader 클래스의 Render() 멤버 함수를 다음과 같이 정의한다.
// 쉐이더 객체에 연결된 객체들을 렌더링하는 함수를 정의한다.
void CSceneShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CDiffusedShader::Render(pd3dDeviceContext);

	// m_ppObjects 즉 오브젝트의 렌더 함수를 호출 하는 것이다. 
	//for (int j = 0; j < m_nObjects; j++){
	//	if (m_ppObjects[j]){
	//		
	//		UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
	//		m_ppObjects[j]->Render(pd3dDeviceContext,pCamera);
//
	//	}
	//}

	//지형을 렌더링한다.
	if (m_pTerrain)
	{
		UpdateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, &m_pTerrain->m_d3dxmtxWorld);
		m_pTerrain->Render(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, pCamera);
	}
}


/***************************************************************************************/
/************************************ Player 쉐이더 *************************************/
/***************************************************************************************/

CPlayerShader::CPlayerShader()
{
	m_pd3dcbWorldMatrix = NULL;
}
CPlayerShader::~CPlayerShader()
{
	if (m_pd3dcbWorldMatrix) m_pd3dcbWorldMatrix->Release();
}
void CPlayerShader::ReleaseObjects()
{
	CDiffusedShader::ReleaseObjects();
}
void CPlayerShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CDiffusedShader::CreateShader(pd3dDevice);
}
void CPlayerShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CDiffusedShader::CreateShaderVariables(pd3dDevice);
}
void CPlayerShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld)
{
	CDiffusedShader::UpdateShaderVariables(pd3dDeviceContext, pd3dxmtxWorld);
}
void CPlayerShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera)
{
	CDiffusedShader::Render(pd3dDeviceContext, pCamera);
}

CTexturedShader::CTexturedShader()
{

}
CTexturedShader::~CTexturedShader()
{

}

void CTexturedShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
 	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSTexturedColor", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSTexturedColor", "ps_4_0", &m_pd3dPixelShader);

}			

void CTexturedShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	// 텍스쳐 맵핑에 사용할 샘플러 상태 객체를 생성한다.
	ID3D11SamplerState* pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;

	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	// 직육면체에 맵핑할 텍스쳐 객체를 생성한다. 이미지 파일은 어떤 이미지라도 상관 없으므로
	// 적당한 파일의 이름을 사용하라.
	ID3D11ShaderResourceView* pd3dTexture = NULL;
	CTexture** ppTextures = new CTexture*[3];
	
	ppTextures[0] = new CTexture(1);
	HRESULT hResult;
	if (SUCCEEDED(hResult = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\wood.jpg"), NULL, NULL, &pd3dTexture, NULL)))
		ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[1] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\wood.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[1]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[2] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\wood.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[2]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CMesh* pMeshTextured = new CTexturedCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f);

	// 텍스쳐 맵핑된 직육면체와 조명과 텍스쳐 맵핑을 사용한 직육면체를 교대로 배치할 것이다.
	int xObjects = 3, yObjects = 3, zObjects = 3, i = 0, nObjectTypes = 2;
	//m_nObjects = ((xObjects * 2) + 1)* ((yObjects * 2) + 1) * ((zObjects * 2) + 1);
	//m_ppObjects = new CGameObject*[m_nObjects];

	float fxPitch = 12.0f * 1.7f;
	float fyPitch = 12.0f * 1.7f;
	float fzPitch = 12.0f * 1.7f;

	CRotatingObject* pRotatingObject = NULL;

	for (int x = -xObjects; x <= xObjects; ++x)
	{
		for (int y = -yObjects; y <= yObjects; ++y)
		{
			for (int z = -zObjects; z <= zObjects; ++z)
			{
				pRotatingObject = new CRotatingObject();
				pRotatingObject->SetMesh(pMeshTextured);
				pRotatingObject->SetTexture(ppTextures[i % 3]);
				pRotatingObject->SetPosition((x*(fxPitch*nObjectTypes) + 0 * fxPitch),
					(y*(fyPitch*nObjectTypes) + 0 * fyPitch), 
					(z*(fzPitch*nObjectTypes) + 0 * fzPitch));
				pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
				//m_ppObjects[i++] = pRotatingObject;

			}
		}
	}

	CreateShaderVariables(pd3dDevice);

	delete[] ppTextures;



}

CIlluminatedTexturedShader::CIlluminatedTexturedShader()
{
}
CIlluminatedTexturedShader::~CIlluminatedTexturedShader()
{
}

void CIlluminatedTexturedShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL* pcbMaterial = (MATERIAL*)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}

void CIlluminatedTexturedShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);

}
void CIlluminatedTexturedShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSTexturedLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSTexturedLighting", "ps_4_0", &m_pd3dPixelShader);
}


void CIlluminatedTexturedShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 5.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[1] = new CMaterial();
	ppMaterials[1]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.5f, 0.0f, 1.0f);
	ppMaterials[1]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 10.0f);
	ppMaterials[1]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	ppMaterials[2] = new CMaterial();
	ppMaterials[2]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f);
	ppMaterials[2]->m_Material.m_d3dxcSpecular = D3DXCOLOR(0.5f, 0.0f, 0.5f, 10.0f);
	ppMaterials[2]->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;
	CTexture **ppTextures = new CTexture*[3];
	ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Stone03.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);
	ppTextures[1] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Brick02.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[1]->SetTexture(0, pd3dTexture, pd3dSamplerState);
	ppTextures[2] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Wood03.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[2]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	CSBXTestMesh* pCubeModel = new CSBXTestMesh(pd3dDevice);

	CreateShaderVariables(pd3dDevice);

	delete[] ppTextures;
	delete[] ppMaterials;
}

CAnimationShader::CAnimationShader()
{


}
CAnimationShader::~CAnimationShader()
{
}

void CAnimationShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL* pcbMaterial = (MATERIAL*)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CAnimationShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_BONE_TRANSFORM);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbBoneTransforms);

}
void CAnimationShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSSkinned", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSSkinned", "ps_4_0", &m_pd3dPixelShader);
}
void CAnimationShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;
	CTexture **ppTextures = new CTexture*[3];
	//
	ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Player.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[1] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Brick02.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[1]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[2] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Wood03.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[2]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	//int Temp = (FRAMEWORK->GetInstance()->m_nPlayers);
	//FRAMEWORK->GetInstance()->m_ppPlayers = new CPlayer*[Temp];

	SkinnedModel* skinnedModel = new SkinnedModel(FRAMEWORK->GetInstance()->m_pd3dDevice, "..\\Data\\Player_0.SBX", TEXTYPE_DYNAMIC);
	CSBXTestMesh* pTestMesh = new CSBXTestMesh(pd3dDevice);
	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 1.0f, 0);

	auto& player_map = PLAYER_MANAGER->GetPlayerMap();
	
	for (auto player : player_map)
	{
		CPlayer *pPlayer = player.second;
		pPlayer->SetSkinned(skinnedModel);
		pPlayer->clipName = "Idle";

		pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pPlayer->m_pSkinnedModel->m_vVertices[0], pPlayer->m_pSkinnedModel->m_vVertices.size());
		pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pPlayer->m_pSkinnedModel->m_vIndices[0], pPlayer->m_pSkinnedModel->m_vIndices.size());
		
		pPlayer->SetMesh(pTestMesh);
		pPlayer->SetTexture(ppTextures[0]);
		pPlayer->SetMaterial(ppMaterials[0]);
	}
	//CPlayer* pPlayer = new CPlayer();

	delete[] ppTextures;

	//////////////////////// 몬스터 생성

	ppTextures = new CTexture*[3];

	ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Monster_0.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[1] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\bow_0.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[1]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	ppTextures[2] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Wood03.jpg"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[2]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	// 여기도 릭남음
	skinnedModel = new SkinnedModel(FRAMEWORK->GetInstance()->m_pd3dDevice, "..\\Data\\Monster_0.SBX", TEXTYPE_DYNAMIC);
	pTestMesh = new CSBXTestMesh(pd3dDevice);

	/*ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Bow_0.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	SkinnedModel* skinnedModel = new SkinnedModel(FRAMEWORK->GetInstance()->m_pd3dDevice, "..\\Data\\Bow_0.SBX", TEXTYPE_STATIC);
	CSBXTestMesh* pTestMesh = new CSBXTestMesh(pd3dDevice);
	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 1.0f, 0);*/
	
	for (auto& monster : OBJECT_MANAGER->GetMonsterMap())
	{
		CMonster *pMonster = monster.second;

		pMonster->SetSkinned(skinnedModel);
		pMonster->clipName = "Walk";

		// 여기 릭남음
		pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vVertices[0], pMonster->m_pSkinnedModel->m_vVertices.size());
		pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vIndices[0], pMonster->m_pSkinnedModel->m_vIndices.size());

		pMonster->SetMesh(pTestMesh);
		pMonster->SetTexture(ppTextures[0]);
		pMonster->SetMaterial(ppMaterials[0]);
	}



	//CMonster* pMonster = new CMonster();

	//pMonster->SetSkinned(skinnedModel);
	//pMonster->clipName = "Take_001";

	//// 여기 릭남음
	//
	//pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vVertices[0], pMonster->m_pSkinnedModel->m_vVertices.size());
	//pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vIndices[0], pMonster->m_pSkinnedModel->m_vIndices.size());

	//pMonster->SetMesh(pTestMesh);
	//pMonster->SetTexture(ppTextures[0]);
	//pMonster->SetMaterial(ppMaterials[0]);

	//pObject->Rotate(&pivot, 180);
	//pObject->SetPosition(0, 0, 3000);

	// 1인칭 카메라로 변경
	// pPlayer->ChangeCamera(FRAMEWORK->GetInstance()->m_pd3dDevice, FIRST_PERSON_CAMERA, FRAMEWORK->GetInstance()->m_GameTimer.GetTimeElapsed());

	// 카메라 객체를 생성하dy고 뷰 포트를 설정한다.
	//CCamera *pCamera = pPlayer->GetCamera();

	//pCamera->CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
	//pCamera->SetViewPort(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, 0, 0, FRAMEWORK->GetInstance()->m_nWndClientWidth, FRAMEWORK->GetInstance()->m_nWndClientHeight,
	//	0.0f, 1.0f);

	// 투영 변환 행렬을 생성 한다.
	//pCamera->GenerateProjectionMatrix(1.0f, 5000.0f, float(FRAMEWORK->GetInstance()->m_nWndClientWidth) / float(FRAMEWORK->GetInstance()->m_nWndClientHeight), 60.0f);
	//pCamera->GenerateViewMatrix();
	//pCamera->CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
		
	//if(FRAMEWORK->GetInstance()->m_pScene)
	//{
	//	if (FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA])
	//	{
	//		if (FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain)
	//		{
	//			CHeightMapTerrain *pTerrain = FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain;
	//			D3DXVECTOR3 vScale = pTerrain->m_pHeightMap->GetScale();
	//			float cxTerrain = (float)pTerrain->m_pHeightMap->GetHeightMapWidth();
	//			float czTerrain = (float)pTerrain->m_pHeightMap->GetHeightMapLength();
	//			//*지형의 xz-평면의 가운데에 플레이어가 위치하도록 한다. 플레이어의 y-좌표가 지형의 높이 보다 크고 중력이 작용하도록 플레이어를 설정하였으므로 플레이어는 점차적으로 하강하게 된다.*/
	//			//pPlayer->SetPosition(D3DXVECTOR3(cxTerrain*0.5f*vScale.x, 550.0f, czTerrain*0.5f*vScale.z));
	//			////플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다.
	//			//pPlayer->SetPlayerUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
	//			////카메라의 위치가 변경될 때 지형의 정보에 따라 카메라의 위치를 변경할 수 있도록 설정한다.
	//			//pPlayer->SetCameraUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);

	//			/*지형의 xz-평면의 가운데에 몬스터가 위치하도록 한다. 플레이어의 y-좌표가 지형의 높이 보다 크고 중력이 작용하도록 플레이어를 설정하였으므로 플레이어는 점차적으로 하강하게 된다.*/
	//			//pMonster->SetPosition(D3DXVECTOR3(0.f, 550.0f, 0.f));
	//			////플레이어의 위치가 변경될 때 지형의 정보에 따라 플레이어의 위치를 변경할 수 있도록 설정한다.
	//			//pMonster->SetMonsterUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
	//		}
	//	}
	//}
	//FRAMEWORK->GetInstance()->m_ppPlayers[0] = pPlayer;
	//FRAMEWORK->GetInstance()->m_pMonster = pMonster;

	delete[] ppTextures;

	CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
}

void CAnimationShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBoneTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_BONE_TRANSFORM* pcbBoneTransforms = (VS_CB_BONE_TRANSFORM*)d3dMappedResource.pData;

	for (size_t i = 0; i < boneTransforms.size(); ++i)
		D3DXMatrixTranspose(&pcbBoneTransforms->m_d3dxmtxBoneTransform[i], &boneTransforms[i]);

	pd3dDeviceContext->Unmap(m_pd3dcbBoneTransforms, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_TRANSFORMS, 1, &m_pd3dcbBoneTransforms);

}

void CAnimationShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera)
{
	////정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	
	// 객체 렌더링
	int i = 0;

	for (auto& player : PLAYER_MANAGER->GetPlayerMap())
	{
		if (player.second->IsConnected())
		{
			CShader::UpdateShaderVariables(pd3dDeviceContext, &player.second->m_d3dxmtxWorld);
			UpdateShaderVariables(pd3dDeviceContext, player.second->m_mtxFinalTransforms);
			UpdateShaderVariables(pd3dDeviceContext, &player.second->m_pMaterial->m_Material);
			CShader::UpdateShaderVariables(pd3dDeviceContext, player.second->m_pTexture);
			player.second->Render(pd3dDeviceContext);
		}
	}
	//cout << i << endl;


	//i = 0;
	//for (int j = 0; j < FRAMEWORK->GetInstance()->m_nPlayers; j++)
	//{
	//	if (FRAMEWORK->GetInstance()->m_ppPlayers[j])
	//	{
	//		// 객체의 재질(Material) 정보를 쉐이더 프로그램으로 전달한다.
	//		CShader::UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_d3dxmtxWorld);
	//		if (FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pSkinnedModel) UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_mtxFinalTransforms);
	//		if (FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pMaterial->m_Material);
	//		if (FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pTexture) CShader::UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_ppPlayers[j]->m_pTexture);

	//		FRAMEWORK->GetInstance()->m_ppPlayers[j]->Render(pd3dDeviceContext);
	//	}
	//}
	i = 0;
	/*for (auto& monster : OBJECT_MANAGER->GetMonsterMap())
	{
		if (!monster.second->GetAlive()) continue;
		i++;
		CMonster *pMonster = monster.second;

		CShader::UpdateShaderVariables(pd3dDeviceContext, &pMonster->m_d3dxmtxWorld);
		if (pMonster->m_pSkinnedModel) UpdateShaderVariables(pd3dDeviceContext, pMonster->m_mtxFinalTransforms);
		if (pMonster->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &pMonster->m_pMaterial->m_Material);
		if (pMonster->m_pTexture) CShader::UpdateShaderVariables(pd3dDeviceContext, pMonster->m_pTexture);

		pMonster->Render(pd3dDeviceContext);
	}*/

	int j = 90;
	//cout << i << endl;
	//i = 0;
	//if (FRAMEWORK->GetInstance()->m_pMonster)
	//{
	//	CShader::UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_pMonster->m_d3dxmtxWorld);
	//	if (FRAMEWORK->GetInstance()->m_pMonster->m_pSkinnedModel) UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_pMonster->m_mtxFinalTransforms);
	//	if (FRAMEWORK->GetInstance()->m_pMonster->m_pMaterial) UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->GetInstance()->m_pMonster->m_pMaterial->m_Material);
	//	if (FRAMEWORK->GetInstance()->m_pMonster->m_pTexture) CShader::UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->GetInstance()->m_pMonster->m_pTexture);

	//	FRAMEWORK->GetInstance()->m_pMonster->Render(pd3dDeviceContext);
	//}
}

CBowShader::CBowShader()
{


}
CBowShader::~CBowShader()
{
}

void CBowShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL* pcbMaterial = (MATERIAL*)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CBowShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_BONE_TRANSFORM);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbBoneTransforms);

}
void CBowShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSTexturedLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSTexturedLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CBowShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;
	CTexture **ppTextures = new CTexture*[1];

	ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Skybox2_0.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	SkinnedModel* skinnedModel = new SkinnedModel(FRAMEWORK->GetInstance()->m_pd3dDevice, "..\\Data\\Skybox2_0.SBX", TEXTYPE_STATIC);
	CSBXTestMesh* pTestMesh = new CSBXTestMesh(pd3dDevice);
	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 1.0f, 0);

	CBow* pBow = new CBow;
	pBow->SetSkinned(skinnedModel);

	pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pBow->m_pSkinnedModel->m_vVertices[0], pBow->m_pSkinnedModel->m_vVertices.size());
	pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pBow->m_pSkinnedModel->m_vIndices[0], pBow->m_pSkinnedModel->m_vIndices.size());
	
	pBow->SetMesh(pTestMesh);
	pBow->SetTexture(ppTextures[0]);
	pBow->SetMaterial(ppMaterials[0]);

	FRAMEWORK->m_vecBow.push_back(pBow);

	delete[] ppTextures;

	CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
}

void CBowShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBoneTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_BONE_TRANSFORM* pcbBoneTransforms = (VS_CB_BONE_TRANSFORM*)d3dMappedResource.pData;

	for (size_t i = 0; i < boneTransforms.size(); ++i)
		D3DXMatrixTranspose(&pcbBoneTransforms->m_d3dxmtxBoneTransform[i], &boneTransforms[i]);

	pd3dDeviceContext->Unmap(m_pd3dcbBoneTransforms, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_TRANSFORMS, 1, &m_pd3dcbBoneTransforms);
}

void CBowShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera)
{
	////정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	// 객체 렌더링
	int i = 0;

	for (auto& bow : FRAMEWORK->m_vecBow)
	{
		CShader::UpdateShaderVariables(pd3dDeviceContext, &bow->m_d3dxmtxWorld);
		UpdateShaderVariables(pd3dDeviceContext, bow->m_mtxFinalTransforms);
		UpdateShaderVariables(pd3dDeviceContext, &bow->m_pMaterial->m_Material);
		CShader::UpdateShaderVariables(pd3dDeviceContext, bow->m_pTexture);
		bow->Render(pd3dDeviceContext);
	}
}


CSkyBoxShader::CSkyBoxShader()
{


}
CSkyBoxShader::~CSkyBoxShader()
{
}

void CSkyBoxShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	MATERIAL* pcbMaterial = (MATERIAL*)d3dMappedResource.pData;
	memcpy(pcbMaterial, pMaterial, sizeof(MATERIAL));
	pd3dDeviceContext->Unmap(m_pd3dcbMaterial, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbMaterial);
}
void CSkyBoxShader::CreateShaderVariables(ID3D11Device* pd3dDevice)
{
	CShader::CreateShaderVariables(pd3dDevice);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(MATERIAL);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbMaterial);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_BONE_TRANSFORM);
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbBoneTransforms);

}
void CSkyBoxShader::CreateShader(ID3D11Device* pd3dDevice)
{
	CShader::CreateShader(pd3dDevice);

	D3D11_INPUT_ELEMENT_DESC d3dInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT nElements = ARRAYSIZE(d3dInputLayout);
	CreateVertexShaderFromFile(pd3dDevice, L"..\\Effect.fx", "VSTexturedLighting", "vs_4_0", &m_pd3dVertexShader, d3dInputLayout, nElements, &m_pd3dVertexLayout);
	CreatePixelShaderFromFile(pd3dDevice, L"..\\Effect.fx", "PSTexturedLighting", "ps_4_0", &m_pd3dPixelShader);
}

void CSkyBoxShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	CMaterial **ppMaterials = new CMaterial*[3];
	ppMaterials[0] = new CMaterial();
	ppMaterials[0]->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	ppMaterials[0]->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 10.0f);
	ppMaterials[0]->m_Material.m_d3dxcEmissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11ShaderResourceView *pd3dTexture = NULL;
	CTexture **ppTextures = new CTexture*[1];

	ppTextures[0] = new CTexture(1);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("..\\Data\\Skybox2_0.dds"), NULL, NULL, &pd3dTexture, NULL);
	ppTextures[0]->SetTexture(0, pd3dTexture, pd3dSamplerState);

	SkinnedModel* skinnedModel = new SkinnedModel(FRAMEWORK->GetInstance()->m_pd3dDevice, "..\\Data\\Skybox2_0.SBX", TEXTYPE_STATIC);
	CSBXTestMesh* pTestMesh = new CSBXTestMesh(pd3dDevice);
	D3DXVECTOR3 pivot = D3DXVECTOR3(0, 1.0f, 0);

	CSkyBox* pSky = new CSkyBox;
	pSky->SetSkinned(skinnedModel);

	pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pSky->m_pSkinnedModel->m_vVertices[0], pSky->m_pSkinnedModel->m_vVertices.size());
	pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pSky->m_pSkinnedModel->m_vIndices[0], pSky->m_pSkinnedModel->m_vIndices.size());

	pSky->SetPosition(D3DXVECTOR3(1024.f, 500.f, 1024.f));
	pSky->SetMesh(pTestMesh);
	pSky->SetTexture(ppTextures[0]);
	pSky->SetMaterial(ppMaterials[0]);
		
	FRAMEWORK->m_pSkyBox = pSky;

	delete[] ppTextures;

	CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
}

void CSkyBoxShader::UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBoneTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_BONE_TRANSFORM* pcbBoneTransforms = (VS_CB_BONE_TRANSFORM*)d3dMappedResource.pData;

	for (size_t i = 0; i < boneTransforms.size(); ++i)
		D3DXMatrixTranspose(&pcbBoneTransforms->m_d3dxmtxBoneTransform[i], &boneTransforms[i]);

	pd3dDeviceContext->Unmap(m_pd3dcbBoneTransforms, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_TRANSFORMS, 1, &m_pd3dcbBoneTransforms);
}

void CSkyBoxShader::Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera)
{
	////정점의 입력-레이아웃을 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//정점-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//픽셀-쉐이더를 디바이스 컨텍스트에 연결(설정)한다. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	// 객체 렌더링
	int i = 0;

	if(FRAMEWORK->m_pSkyBox != nullptr)
	{
		CShader::UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->m_pSkyBox->m_d3dxmtxWorld);
		UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->m_pSkyBox->m_mtxFinalTransforms);
		UpdateShaderVariables(pd3dDeviceContext, &FRAMEWORK->m_pSkyBox->m_pMaterial->m_Material);
		CShader::UpdateShaderVariables(pd3dDeviceContext, FRAMEWORK->m_pSkyBox->m_pTexture);
		FRAMEWORK->m_pSkyBox->Render(pd3dDeviceContext);
	}
}