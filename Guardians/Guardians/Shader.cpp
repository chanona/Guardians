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

// CShader Ŭ������ CreateVertexShaderFromFile() ��� �Լ��� ������ ���� �����Ѵ�.
// �� �Լ��� ���̴� ���Ͽ��� ���� - ���̴��� �Է� - ���̾ƿ��� �����Ѵ�.
void CShader::CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout, UINT nElements, ID3D11InputLayout **ppd3dVertexLayout)
{
	HRESULT hResult;

	// ���Ĺ����� ������ üũ�ؼ� �����Ѵ�(����Ʈ�� üũ���� ����)
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	// �����ϵ� ����Ʈ�ڵ�� ������ ���̸� ������ �����͸� �����ϱ� ���� ��� �ϴ� �������̽�������
	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ�
	���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� ����-���̴��� �����Ѵ�. 
		pd3dDevice->CreateVertexShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dVertexShader);
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ� �Է� ���̾ƿ����� ���� ���̾ƿ��� �����Ѵ�. 
		pd3dDevice->CreateInputLayout(pd3dInputLayout, nElements, pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), ppd3dVertexLayout);
		pd3dShaderBlob->Release();
	}
}

// CShader Ŭ������ CreatePixelShaderFromFile() ��� �Լ��� ������ ���� �����Ѵ�
// �� �Լ��� ���̴� ���Ͽ��� �ȼ� - ���̴��� �����Ѵ�.
void CShader::CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader)
{
	HRESULT hResult;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pd3dShaderBlob = NULL, *pd3dErrorBlob = NULL;
	/*����(pszFileName)���� ���̴� �Լ�(pszShaderName)�� �������Ͽ� �����ϵ� ���̴� �ڵ��� �޸� �ּ�(pd3dShaderBlob)�� ��ȯ�Ѵ�.*/
	if (SUCCEEDED(hResult = D3DX11CompileFromFile(pszFileName, NULL, NULL, pszShaderName, pszShaderModel, dwShaderFlags, 0, NULL, &pd3dShaderBlob, &pd3dErrorBlob, NULL)))
	{
		//�����ϵ� ���̴� �ڵ��� �޸� �ּҿ��� �ȼ�-���̴��� �����Ѵ�. 
		pd3dDevice->CreatePixelShader(pd3dShaderBlob->GetBufferPointer(), pd3dShaderBlob->GetBufferSize(), NULL, ppd3dPixelShader);
		pd3dShaderBlob->Release();
	}
}

// CShader Ŭ������ CreateShader() ��� �Լ��� Render() ��� �Լ��� ������ ���� �����Ѵ�.
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
	/// �ؽ��İ��� ������Ʈ
	pd3dDeviceContext->PSSetShaderResources(PS_SLOT_TEXTURE, pTexture->m_nTextures, pTexture->m_ppd3dsrvTextures);
	pd3dDeviceContext->PSSetSamplers(PS_SLOT_SAMPLER_STATE, pTexture->m_nTextures, pTexture->m_ppd3dSamplerStates);
}

// ���̴� ������ �Լ��� �����Ѵ�.
void CShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera)
{
	//��ü ������
	//for (auto& bow : FRAMEWORK->m_vecBow)
	//{
	//	UpdateShaderVariables(pd3dDeviceContext, &bow->m_d3dxmtxWorld);
	//	UpdateShaderVariables(pd3dDeviceContext, &bow->m_pMaterial->m_Material);
	//	UpdateShaderVariables(pd3dDeviceContext, bow->m_pTexture);
	//	//������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	//	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//	//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	//	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//	//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	//	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	//}

	//if (!(FRAMEWORK->m_vecBow.empty()))
	//{
	//	////������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	//	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//	//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	//	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//	//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
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
			//������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
			if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
			//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
			if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
			//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
			if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
		}
	}

	//for (int j = 0; j < FRAMEWORK->GetInstance()->m_nPlayers; j++)
	//{
	//	if (FRAMEWORK->GetInstance()->m_ppPlayers[j])
	//	{
	//		// ��ü�� ����(Material) ������ ���̴� ���α׷����� �����Ѵ�.
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
	// ��ü���� ������ ������ ������ / ��� / �Ķ��� 
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
	// ���� Error
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
/************************************ Scene ���̴� *************************************/
/***************************************************************************************/

// CObjectShader Ŭ������ �����ڿ� �Ҹ��ڸ� ������ ���� �����Ѵ�.
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
	
	// ���� ��ȯ ����� ���� ��� ���۸� �����Ѵ�.
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
	
	// ���� ��ȯ ����� ������ۿ� �����Ѵ�.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbWorldMatrix, 0, D3D11_MAP_WRITE_DISCARD, 0,
		&d3dMappedResource);
	VS_CB_WORLD_MATRIX* pcbWorldMatrix = (VS_CB_WORLD_MATRIX*)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbWorldMatrix->m_d3dxmtxWorld, pd3dxmtxWorld);
	pd3dDeviceContext->Unmap(m_pd3dcbWorldMatrix, 0);
	
	// ��� ���۸� ����̽��� ����(VS_SLOT_WORLD_MATRIX)�� �����Ѵ�.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 1, &m_pd3dcbWorldMatrix);
	
}

void CSceneShader::BuildObjects(ID3D11Device* pd3dDevice)
{
	//// ���� ���� ���̰� 12x12x12�� ������ü �޽��� ������.
	//CCubeMesh* pCubeMesh = new CCubeMesh(pd3dDevice, 12.0f, 12.0f, 12.0f);

	//int xObjects = 20, yObjects = 0, zObjects = 20, i = 0;
	//// x�� y�� z������ 23���� �� 23x23x23 = 12167���� ������ü ��ġ.
	//
	//m_nObjects = (xObjects * 2 + 1)*(yObjects * 2 + 1)*(zObjects * 2 + 1);
	//m_ppObjects = new CGameObject*[m_nObjects];
	//
	//float fxPitch = 12.0f;
	//float fyPitch = 12.0f;
	//float fzPitch = 12.0f;
	//
	////������ü ��ü�� �����Ѵ�.

	////CRotatingObject* pRotatingObject = NULL;
	//CGameObject* pObject = new CGameObject();


	//for (int x = -xObjects; x <= xObjects; ++x){
	//	for (int y = -yObjects; y <= yObjects; ++y){
	//		for (int z = -zObjects; z <= zObjects; ++z){

	//			pObject = new CRotatingObject();
	//			pObject->SetMesh(pCubeMesh);
	//			// �� ������ü ��ü�� ��ġ�� �����Ѵ�
	//			pObject->SetPosition(fxPitch*x, fyPitch*y, fzPitch*z);
	//			//// �� ������ü ��ü�� ȸ������ y������ ����
	//			//pRotatingObject->SetRotationAxis(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	//			//pRotatingObject->SetRotationSpeed(10.0f*(i % 10));
	//			m_ppObjects[i++] = pObject;
	//		}
	//	}
	//}

	//������ Ȯ���� ������ �����̴�. x-��� z-���� 8��, y-���� 2�� Ȯ���Ѵ�.
	D3DXVECTOR3 d3dxvScale(100.f, 5.0f, 100.f);
	D3DXCOLOR d3dxColor(0.0f, 0.2f, 0.0f, 0.0f);

	//������ ���� �� �̹��� ������ ����Ͽ� �����Ѵ�. 
	//������ �����ϴ� ���� �޽��� ������ �� 256(16x16)���� �ȴ�.
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

// CObjectShader Ŭ������ Render() ��� �Լ��� ������ ���� �����Ѵ�.
// ���̴� ��ü�� ����� ��ü���� �������ϴ� �Լ��� �����Ѵ�.
void CSceneShader::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CDiffusedShader::Render(pd3dDeviceContext);

	// m_ppObjects �� ������Ʈ�� ���� �Լ��� ȣ�� �ϴ� ���̴�. 
	//for (int j = 0; j < m_nObjects; j++){
	//	if (m_ppObjects[j]){
	//		
	//		UpdateShaderVariables(pd3dDeviceContext, &m_ppObjects[j]->m_d3dxmtxWorld);
	//		m_ppObjects[j]->Render(pd3dDeviceContext,pCamera);
//
	//	}
	//}

	//������ �������Ѵ�.
	if (m_pTerrain)
	{
		UpdateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, &m_pTerrain->m_d3dxmtxWorld);
		m_pTerrain->Render(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, pCamera);
	}
}


/***************************************************************************************/
/************************************ Player ���̴� *************************************/
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
	// �ؽ��� ���ο� ����� ���÷� ���� ��ü�� �����Ѵ�.
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

	// ������ü�� ������ �ؽ��� ��ü�� �����Ѵ�. �̹��� ������ � �̹����� ��� �����Ƿ�
	// ������ ������ �̸��� ����϶�.
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

	// �ؽ��� ���ε� ������ü�� ����� �ؽ��� ������ ����� ������ü�� ����� ��ġ�� ���̴�.
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

	//////////////////////// ���� ����

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

	// ���⵵ ������
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

		// ���� ������
		pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vVertices[0], pMonster->m_pSkinnedModel->m_vVertices.size());
		pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vIndices[0], pMonster->m_pSkinnedModel->m_vIndices.size());

		pMonster->SetMesh(pTestMesh);
		pMonster->SetTexture(ppTextures[0]);
		pMonster->SetMaterial(ppMaterials[0]);
	}



	//CMonster* pMonster = new CMonster();

	//pMonster->SetSkinned(skinnedModel);
	//pMonster->clipName = "Take_001";

	//// ���� ������
	//
	//pTestMesh->SetVertices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vVertices[0], pMonster->m_pSkinnedModel->m_vVertices.size());
	//pTestMesh->SetIndices(FRAMEWORK->GetInstance()->m_pd3dDevice, &pMonster->m_pSkinnedModel->m_vIndices[0], pMonster->m_pSkinnedModel->m_vIndices.size());

	//pMonster->SetMesh(pTestMesh);
	//pMonster->SetTexture(ppTextures[0]);
	//pMonster->SetMaterial(ppMaterials[0]);

	//pObject->Rotate(&pivot, 180);
	//pObject->SetPosition(0, 0, 3000);

	// 1��Ī ī�޶�� ����
	// pPlayer->ChangeCamera(FRAMEWORK->GetInstance()->m_pd3dDevice, FIRST_PERSON_CAMERA, FRAMEWORK->GetInstance()->m_GameTimer.GetTimeElapsed());

	// ī�޶� ��ü�� ������dy�� �� ��Ʈ�� �����Ѵ�.
	//CCamera *pCamera = pPlayer->GetCamera();

	//pCamera->CreateShaderVariables(FRAMEWORK->GetInstance()->m_pd3dDevice);
	//pCamera->SetViewPort(FRAMEWORK->GetInstance()->m_pd3dDeviceContext, 0, 0, FRAMEWORK->GetInstance()->m_nWndClientWidth, FRAMEWORK->GetInstance()->m_nWndClientHeight,
	//	0.0f, 1.0f);

	// ���� ��ȯ ����� ���� �Ѵ�.
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
	//			//*������ xz-����� ����� �÷��̾ ��ġ�ϵ��� �Ѵ�. �÷��̾��� y-��ǥ�� ������ ���� ���� ũ�� �߷��� �ۿ��ϵ��� �÷��̾ �����Ͽ����Ƿ� �÷��̾�� ���������� �ϰ��ϰ� �ȴ�.*/
	//			//pPlayer->SetPosition(D3DXVECTOR3(cxTerrain*0.5f*vScale.x, 550.0f, czTerrain*0.5f*vScale.z));
	//			////�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
	//			//pPlayer->SetPlayerUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);
	//			////ī�޶��� ��ġ�� ����� �� ������ ������ ���� ī�޶��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
	//			//pPlayer->SetCameraUpdatedContext(FRAMEWORK->GetInstance()->m_pScene->m_ppShaders[SHADER_TERRA]->m_pTerrain);

	//			/*������ xz-����� ����� ���Ͱ� ��ġ�ϵ��� �Ѵ�. �÷��̾��� y-��ǥ�� ������ ���� ���� ũ�� �߷��� �ۿ��ϵ��� �÷��̾ �����Ͽ����Ƿ� �÷��̾�� ���������� �ϰ��ϰ� �ȴ�.*/
	//			//pMonster->SetPosition(D3DXVECTOR3(0.f, 550.0f, 0.f));
	//			////�÷��̾��� ��ġ�� ����� �� ������ ������ ���� �÷��̾��� ��ġ�� ������ �� �ֵ��� �����Ѵ�.
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
	////������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);
	
	// ��ü ������
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
	//		// ��ü�� ����(Material) ������ ���̴� ���α׷����� �����Ѵ�.
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
	////������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	// ��ü ������
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
	////������ �Է�-���̾ƿ��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexLayout) pd3dDeviceContext->IASetInputLayout(m_pd3dVertexLayout);
	//����-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dVertexShader) pd3dDeviceContext->VSSetShader(m_pd3dVertexShader, NULL, 0);
	//�ȼ�-���̴��� ����̽� ���ؽ�Ʈ�� ����(����)�Ѵ�. 
	if (m_pd3dPixelShader) pd3dDeviceContext->PSSetShader(m_pd3dPixelShader, NULL, 0);

	// ��ü ������
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