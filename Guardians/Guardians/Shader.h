#pragma once

#include "GameObject.h"
#include "HeightMapTerrain.h"

#define SHADER_MANAGER CShader::GetInstance()

struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX m_d3dxmtxWorld;

};

struct VS_CB_BONE_TRANSFORM
{
	D3DXMATRIX m_d3dxmtxBoneTransform[96];
};

class CHeightMapTerrain;
class CShader
{
public:
	CShader();
	virtual ~CShader();
	
	static CShader* GetInstance()
	{
		static CShader instance;
		return &instance;
	}

	//파일에서 정점-쉐이더를 생성하는 함수를 선언한다. 
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderModel,ID3D11VertexShader **ppd3dVertexShader,
		D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout,UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	
	//파일에서 픽셀-쉐이더를 생성하는 함수를 선언한다. 
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11PixelShader **ppd3dPixelShader);
	
	//void CreateGeometryShaderFromFile(ID3D11Device* pd3dDevice, WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, 
	//	ID3D11GeometryShader** ppd3dGeometryShader);




	//CShader 파생 클래스에서 쉐이더를 생성하기 위해 호출하는 가상함수를 선언한다.
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	// 쉐이더 클래스의 상수 버퍼를 생성하고 반환하는 멤버 함수를 선언한다.
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	
	// 쉐이더 클래스의 상수 버퍼를 갱신하는 멤버 함수를 선언한다.
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, CTexture* pTexture);


	virtual void BuildObjects(ID3D11Device* pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fItmeElapsed);
	
	//CShader 파생 클래스에서 렌더링하기 위해 호출하는 가상함수를 선언한다.
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera = NULL);


public:
	//정점-쉐이더 인터페이스 포인터와 입력-레이아웃 인터페이스 포인터를 선언한다.
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	//픽셀-쉐이더 인터페이스 포인터를 선언한다.
	ID3D11PixelShader *m_pd3dPixelShader;
	ID3D11GeometryShader *m_pd3dGeometryShader;
	
	//CGameObject **m_ppObjects;
	//int m_nObjects;

	ID3D11Buffer *m_pd3dcbWorldMatrix;
	ID3D11Buffer *m_pd3dcbMaterial;
	ID3D11Buffer *m_pd3dcbBoneTransforms;
	
public:
	CHeightMapTerrain *m_pTerrain;
};

class CIlluminatedShader : public CShader
{
public:
	CIlluminatedShader();
	~CIlluminatedShader();
	
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);

	virtual void BuildObjects(ID3D11Device* pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera = NULL);
};


// 색상을 가진 정점들로 구성된 메쉬를 그리기 위한 CDiffusedShader 클래스를 다음과 같이 선언
class CDiffusedShader : public CShader
{
public:
	CDiffusedShader();
	~CDiffusedShader();
	
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld);

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera = NULL);
};

// 객체들을 그리기 위한 CSceneShader 클래스
class CSceneShader : public CDiffusedShader
{
public:
	CSceneShader();
	~CSceneShader();
	
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera = NULL);
	
	//CGameObject** m_ppObjects;
	//int m_nObjects;
	
	//ID3D11Buffer* m_pd3dcbWorldMatrix;
};

// 플레이어를 그리기 위한 쉐이더 클래스 CPlayerShader

class CPlayerShader : public CDiffusedShader
{
public:
	CPlayerShader();
	~CPlayerShader();
	
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld = NULL);

	virtual void ReleaseObjects();
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera = NULL);
	//ID3D11Buffer* m_pd3dcbWorldMatrix;
};


/*
 텍스쳐 맵핑을 사용하여 렌더링하기 위한 CTexturedShader 클래스
*/

class CTexturedShader : public CShader
{
public:
	CTexturedShader();
	~CTexturedShader();

	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);
};

/*
 텍스쳐 맵핑과 조명을 사용하여 렌더링하기 위한 CIlluminatedTexturedShader
*/

class CIlluminatedTexturedShader : public CShader
{
public:
	CIlluminatedTexturedShader();
	~CIlluminatedTexturedShader();
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);

	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);
};

class CAnimationShader : public CShader
{
public:
	CAnimationShader();
	~CAnimationShader();

	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms);
	
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);

	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera = NULL);
};

class CBowShader : public CShader
{
public:
	CBowShader();
	~CBowShader();

	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms);

	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);

	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera = NULL);
};


class CSkyBoxShader : public CShader
{
public:
	CSkyBoxShader();
	~CSkyBoxShader();

	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, vector<D3DXMATRIX>& boneTransforms);

	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	virtual void CreateShader(ID3D11Device* pd3dDevice);
	virtual void BuildObjects(ID3D11Device* pd3dDevice);

	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCmaera = NULL);
};