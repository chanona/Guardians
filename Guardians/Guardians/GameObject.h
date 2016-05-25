#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "Camera.h"
#include "SONData.h"
#include <vector>

using namespace std;

#define DegreeToRadian(x)	((x)*3.1415926535/180.0)

#define DIR_FORWARD			0x01
#define DIR_BACKWARD		0x02
#define DIR_LEFT			0x04
#define DIR_RIGHT			0x08
#define DIR_UP				0x10
#define DIR_DOWN			0x20


struct MATERIAL
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular;	// (r,g,b,a = power)
	D3DXCOLOR m_d3dxcEmissive;
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();
	
	int m_nReferences;
	void AddRef();
	void Release();

	MATERIAL m_Material;
};

/*
 텍스쳐를 표현하기 위한 클래스를 다음과 같이 선언한다. 각 객체는 텍스쳐 클래스 객체를 공유할 수 있다.
*/
class CTexture
{
public:
	CTexture(int nTextures);
	virtual ~CTexture();

public:
	int	m_nReferences;


public:
	void AddRef();
	void Release();

	void SetTexture(int nIndex, ID3D11ShaderResourceView* pd3dsrvTexture, ID3D11SamplerState* pd3dSamplerState);

public:
	ID3D11ShaderResourceView**			m_ppd3dsrvTextures;
	
	ID3D11SamplerState**				m_ppd3dSamplerStates;
	ID3D11DepthStencilState			*m_pd3dDepthStencilState;
	int									m_nTextures;

};

class SkinnedModel
{
private:
	int m_nReferences;

public:
	vector<CMyBoneVertex> m_vVertices;
	vector<WORD> m_vIndices;

	SkinnedData mSkinnedData;
public:
	SkinnedModel(ID3D11Device* pd3dDevice, const string& modelFileName, TEX_TYPE eType);
	~SkinnedModel();

	void AddRef();
	void Release();
};


class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

private:
	int m_nReferences;

	UINT   m_id;
	int    m_index;
	int    m_entityType;
public:
	CMaterial* m_pMaterial;
	void SetMaterial(CMaterial* pMaterial);

public:	

	void SetID(const UINT id) { m_id = id; }
	void SetIndex(const int index) { m_index = index; }

	UINT GetID() const { return m_id; }
	int GetIndex() const { return m_index; }

	// 객체의 위치 설정
	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);
	
	// 로컬 x y z축 방향으로 이동
	virtual void MoveStrafe(float fDistance = 1.0f);
	virtual void MoveUp(float fDistance = 1.0f);
	virtual void MoveForward(float fDistance = 1.0f);

	// 로컬 x y z축 방향으로 회전
	virtual void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	virtual void Rotate(D3DXVECTOR3* pd3dxvAxis, float Angle);
	
	// 객체의 위치, x축 y축 z축 방향 벡터를 반환
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetLookAt();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
	
	
	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;		// 객체를 표현하기위한 월드 변환 행렬
	CMesh *m_pMesh;					// 하나의 메쉬를 공유할 수 있도록 메쉬에 대한 포인터로 선언.
	
	SkinnedModel* m_pSkinnedModel;
	float mTimePos;
	string clipName;
	vector<D3DXMATRIX> m_mtxFinalTransforms;

	/******************************************************************/
	//// Skinned
	//vector<CMyBoneVertex> m_vVertices;
	//vector<USHORT> m_vIndices;
	//SkinnedData mSkinnedData;
	//float mTimePos;
	
	/******************************************************************/

	virtual void SetMesh(CMesh *pMesh);
	virtual void SetSkinned(SkinnedModel* pSkinned);
	virtual void Animate(float fTimeElapsed);

	//객체의 Render() 함수에 카메라가 필요한 이유는 
	//나중에 카메라에 보이는 객체들만 렌더링하도록 
	//변경할 것이기 때문이다(카메라 절두체 컬링(Frustum Culling)).  
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera* pCamera = NULL);


public:
	CTexture* m_pTexture;
	void SetTexture(CTexture* pTexture);
};

class CRotatingObject : public CGameObject
{
private:
	float m_fRotationSpeed;
	D3DXVECTOR3 m_d3dxvRotationAxis;

public:
	CRotatingObject();
	virtual ~CRotatingObject();

	// 자전속도와 회전축 벡터를 설정
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

	virtual void Animate(float fTimeElapse);
	//void Move(float fTimeElapse);

	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera *pCamera);
};

//
//class CSkyBox : public CGameObject
//{
//public:
//	CSkyBox(ID3D11Device *pd3dDevice);
//	virtual ~CSkyBox();
//
//#ifdef _WITH_SKYBOX_TEXTURE_ARRAY
//	ID3D11Buffer				*m_pd3dcbTextureIndex;
//#endif
//
//	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
//};
//
