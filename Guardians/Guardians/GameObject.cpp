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
		if (m_ppd3dsrvTextures[i]) 
			m_ppd3dsrvTextures[i]->Release();

		if (m_ppd3dSamplerStates[i]) 
			m_ppd3dSamplerStates[i]->Release();
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

bool CGameObject::CircleCollision(D3DXVECTOR3 pPos1, D3DXVECTOR3 pPos2)
{
	D3DXVECTOR3 vDir = pPos1 - pPos2;

	float fLength = D3DXVec3Length(&vDir);
	
	if (fLength < 2.5f)
		return true;
	return false;
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

void CGameObject::Animate(float fTimeElapsed, TEX_TYPE eType)
{
	mTimePos += fTimeElapsed;

	if(eType == TEXTYPE_DYNAMIC)
		m_pSkinnedModel->mSkinnedData.GetFinalTransforms(clipName, mTimePos, m_mtxFinalTransforms, TEXTYPE_DYNAMIC);
	else
		m_pSkinnedModel->mSkinnedData.GetFinalTransforms(clipName, mTimePos, m_mtxFinalTransforms, TEXTYPE_STATIC);
	
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
