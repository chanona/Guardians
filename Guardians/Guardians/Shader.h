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

	//���Ͽ��� ����-���̴��� �����ϴ� �Լ��� �����Ѵ�. 
	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, 
		LPCSTR pszShaderName, LPCSTR pszShaderModel,ID3D11VertexShader **ppd3dVertexShader,
		D3D11_INPUT_ELEMENT_DESC *pd3dInputLayout,UINT nElements, ID3D11InputLayout **ppd3dVertexLayout);
	
	//���Ͽ��� �ȼ�-���̴��� �����ϴ� �Լ��� �����Ѵ�. 
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11PixelShader **ppd3dPixelShader);
	
	//void CreateGeometryShaderFromFile(ID3D11Device* pd3dDevice, WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, 
	//	ID3D11GeometryShader** ppd3dGeometryShader);




	//CShader �Ļ� Ŭ�������� ���̴��� �����ϱ� ���� ȣ���ϴ� �����Լ��� �����Ѵ�.
	virtual void CreateShader(ID3D11Device *pd3dDevice);
	// ���̴� Ŭ������ ��� ���۸� �����ϰ� ��ȯ�ϴ� ��� �Լ��� �����Ѵ�.
	virtual void CreateShaderVariables(ID3D11Device* pd3dDevice);
	
	// ���̴� Ŭ������ ��� ���۸� �����ϴ� ��� �Լ��� �����Ѵ�.
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, D3DXMATRIX* pd3dxmtxWorld = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, MATERIAL* pMaterial = NULL);
	virtual void UpdateShaderVariables(ID3D11DeviceContext* pd3dDeviceContext, CTexture* pTexture);


	virtual void BuildObjects(ID3D11Device* pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fItmeElapsed);
	
	//CShader �Ļ� Ŭ�������� �������ϱ� ���� ȣ���ϴ� �����Լ��� �����Ѵ�.
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* pCamera = NULL);


public:
	//����-���̴� �������̽� �����Ϳ� �Է�-���̾ƿ� �������̽� �����͸� �����Ѵ�.
	ID3D11VertexShader *m_pd3dVertexShader;
	ID3D11InputLayout *m_pd3dVertexLayout;

	//�ȼ�-���̴� �������̽� �����͸� �����Ѵ�.
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


// ������ ���� ������� ������ �޽��� �׸��� ���� CDiffusedShader Ŭ������ ������ ���� ����
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

// ��ü���� �׸��� ���� CSceneShader Ŭ����
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

// �÷��̾ �׸��� ���� ���̴� Ŭ���� CPlayerShader

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
 �ؽ��� ������ ����Ͽ� �������ϱ� ���� CTexturedShader Ŭ����
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
 �ؽ��� ���ΰ� ������ ����Ͽ� �������ϱ� ���� CIlluminatedTexturedShader
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