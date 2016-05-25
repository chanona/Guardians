#include "stdafx.h"
#include "GameObject.h"
#include "SONImporter.h"


/************************************* CMaterial ***************************************/
CMaterial::CMaterial()
{
	m_nReferences = 0;
}
CMaterial::~CMaterial()
{

}
void CMaterial::AddRef()
{
	m_nReferences++; 
}
void CMaterial::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences == 0) delete this;
}

/************************************* CTexture ***************************************/
CTexture::CTexture(int nTextures)
{
	m_nReferences = 0;
	m_nTextures = nTextures;

	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; ++i) 
		m_ppd3dsrvTextures[i] = NULL;

	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nTextures];
	for (int i = 0; i < m_nTextures; ++i)
		m_ppd3dSamplerStates[i] = NULL;
}
CTexture::~CTexture()
{
	if (m_ppd3dsrvTextures) delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) delete[] m_ppd3dSamplerStates;
}

void CTexture::AddRef()
{
	m_nReferences++;
}
void CTexture::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	for (int i = 0; i < m_nReferences; ++i)
	{
		if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->Release();
		if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->Release();
	}
	if (m_nReferences == 0) delete this;
}

void CTexture::SetTexture(int nIndex, ID3D11ShaderResourceView* pd3dsrvTexture, ID3D11SamplerState* pd3dSamplerState)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->AddRef();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->AddRef();
}

/************************************ CGameObject ***************************************/


SkinnedModel::SkinnedModel(ID3D11Device* pd3dDevice, const string& modelFileName, TEX_TYPE eType)
{
	m_nReferences = 0;

	SBXImporter sbxImporter;

	bool isImport = false;
	isImport = sbxImporter.ImportSBX(modelFileName, m_vVertices, m_vIndices, mSkinnedData, eType);
	
	if (isImport)
		cout << "임포트 성공" << endl;
	else
		cout << "임포트 실패" << endl;

}
SkinnedModel::~SkinnedModel()
{
}
void SkinnedModel::AddRef()
{
	m_nReferences++;
}
void SkinnedModel::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}

CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_pMesh = NULL;
	m_pSkinnedModel = NULL;
	mTimePos = 0;

	//m_nReferences = 1;
	m_pMaterial = NULL;
	
	m_pTexture = NULL;
}

CGameObject :: ~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture) m_pTexture->Release();
}

void CGameObject::SetMaterial(CMaterial* pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}
void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxWorld._41 = d3dxvPosition.x;
	m_d3dxmtxWorld._42 = d3dxvPosition.y;
	m_d3dxmtxWorld._43 = d3dxvPosition.z;
}

D3DXVECTOR3 CGameObject::GetPosition()
{
	return D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43);
}

D3DXVECTOR3 CGameObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	
	return d3dxvLookAt;
}
D3DXVECTOR3 CGameObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	
	return(d3dxvRight);
}

void CGameObject::MoveStrafe(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}
void CGameObject::MoveUp(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}
void CGameObject::MoveForward(float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}	

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw),
		(float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3* pd3dxvAxis, float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

/*
 객체에 메쉬를 연결하기 위한 함수.
 이미 연결된 메쉬가 있으면 연결된 메쉬의 참조 카운터를 감소시킨다.
 새로운 메쉬를 연결하고 새로운 메쉬의 참조 카운터를 증가시키도록 AddRef() 함수를 호출한다.
*/
void CGameObject::SetMesh(CMesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
	
}
void CGameObject::SetSkinned(SkinnedModel* pSkinned)
{
	if (m_pSkinnedModel) m_pSkinnedModel->Release();
	m_pSkinnedModel = pSkinned;
	if (m_pSkinnedModel) m_pSkinnedModel->AddRef();
}
void CGameObject::SetTexture(CTexture* pTexture)
{
	/// 기존에 텍스쳐를 갖고 있으면 버린다.
	if (m_pTexture) m_pTexture->Release();

	/// 텍스처 포인터를 준다.
	m_pTexture = pTexture;
	
	/// 레퍼런스를 하나 증가 시킨다.
	if (m_pTexture) m_pTexture->AddRef();
}
void CGameObject::Animate(float fTimeElapsed)
{
	mTimePos += fTimeElapsed;

	m_pSkinnedModel->mSkinnedData.GetFinalTransforms(clipName, mTimePos, m_mtxFinalTransforms);
	
	if (mTimePos > m_pSkinnedModel->mSkinnedData.GetClipEndTime(clipName))
		mTimePos = 0.0f;
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera)
{
	//객체를 렌더링한다는 것은 객체에 연결된 메쉬를 렌더링 하는 것.
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}


/////////////////////////////////////////////////////////////////////////////////////////

CRotatingObject::CRotatingObject()
{
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);
}


void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
{
	CGameObject::Render(pd3dDeviceContext,pCamera);
}

//
//CSkyBox::CSkyBox(ID3D11Device *pd3dDevice) : CGameObject()
//{
//	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
//	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
//	d3dDepthStencilDesc.DepthEnable = false;
//	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
//	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
//	d3dDepthStencilDesc.StencilEnable = false;
//	d3dDepthStencilDesc.StencilReadMask = 0xFF;
//	d3dDepthStencilDesc.StencilWriteMask = 0xFF;
//	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
//	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
//	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
//	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
//	pd3dDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dDepthStencilState);
//
//#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
//	D3D11_BUFFER_DESC d3dBufferDesc;
//	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
//	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	d3dBufferDesc.ByteWidth = sizeof(int) * 4;
//	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbTextureIndex);
//
//	ID3D11DeviceContext *pd3dDeviceContext;
//	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
//	pd3dDeviceContext->PSSetConstantBuffers(PS_CB_SLOT_SKYBOX, 1, &m_pd3dcbTextureIndex);
//	if (pd3dDeviceContext) pd3dDeviceContext->Release();
//#endif
//}
//
//CSkyBox::~CSkyBox()
//{
//#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
//	if (m_pd3dcbTextureIndex) m_pd3dcbTextureIndex->Release();
//#endif
//}
//
//void CSkyBox::Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera)
//{
//	D3DXVECTOR3 d3dxvCameraPos = pCamera->GetPosition();
//	SetPosition(d3dxvCameraPos.x, d3dxvCameraPos.y, d3dxvCameraPos.z);
//	Update(NULL);
//
//	CGameObject::UpdateShaderVariable(pd3dDeviceContext, &m_d3dxmtxWorld);
//
//	if (m_pShader) m_pShader->Render(pd3dDeviceContext, pCamera);
//
//	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);
//
//	CSkyBoxMesh *pSkyBoxMesh = (CSkyBoxMesh *)m_ppMeshes[0];
//	pSkyBoxMesh->OnPrepareRender(pd3dDeviceContext);
//
//#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
//	m_pMaterial->m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
//#else
//#ifdef _WITH_SKYBOX_TEXTURE_CUBE
//	m_pMaterial->m_pTexture->UpdateShaderVariable(pd3dDeviceContext);
//#else
//	m_pMaterial->m_pTexture->UpdateSamplerShaderVariable(pd3dDeviceContext, 0, PS_SLOT_SAMPLER_SKYBOX);
//#endif
//#endif
//	pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilState, 1);
//
//	for (int i = 0; i < 6; i++)
//	{
//#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
//		D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
//		pd3dDeviceContext->Map(m_pd3dcbTextureIndex, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
//		int *pfIndex = (int *)d3dMappedResource.pData;
//		pfIndex[0] = pfIndex[1] = pfIndex[2] = pfIndex[3] = i;
//		pd3dDeviceContext->Unmap(m_pd3dcbTextureIndex, 0);
//#else
//#ifndef _WITH_SKYBOX_TEXTURE_CUBE
//		m_pMaterial->m_pTexture->UpdateTextureShaderVariable(pd3dDeviceContext, i, PS_SLOT_TEXTURE_SKYBOX);
//#endif
//#endif
//		pd3dDeviceContext->DrawIndexed(4, 0, i * 4);
//	}
//
//	pd3dDeviceContext->OMSetDepthStencilState(NULL, 1);
//}
