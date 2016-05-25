#pragma once

// ������ ������ �������� ����
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

/************************************** Vertex ****************************************/
class CVertex
{
private:
	//������ ��ġ ����(3���� ����)�� �����ϱ� ���� ��� ������ �����Ѵ�. 
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//CVertex Ŭ������ �����ڿ� �Ҹ��ڸ� ������ ���� �����Ѵ�. 
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};

class CDiffusedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;

	// ���� ������ ��Ÿ���� ��� ���� (D3DXCOLOR ����ü)�� �����Ѵ�.
	D3DXCOLOR m_d3dxcDiffuse;

	// ������ & �Ҹ���
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z);
		m_d3dxcDiffuse = d3dxcDiffuse;
	}
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxcDiffuse = d3dxcDiffuse;
	}
	CDiffusedVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	~CDiffusedVertex(){}
};

class CNormalVertex
{
	D3DXVECTOR3 m_d3dxvPosition;

public:
	// ������ ����������ϱ� ���Ͽ� �������Ͱ� �ʿ��ϴ�.
	D3DXVECTOR3 m_d3dxvNormal;

	CNormalVertex(float x, float y, float z, float nx, float ny, float nz)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z); 
		m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz);
	}
	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
	}
	CNormalVertex() 
	{ 
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	~CNormalVertex() {}



};

class CTexturedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR2 m_d3dxvTexCoord;

	CTexturedVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}
	CTexturedVertex(float x, float y, float z, float u, float v)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z);
		m_d3dxvTexCoord = D3DXVECTOR2(u, v);
	}
	CTexturedVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR2 d3dxvTexture)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvTexCoord = d3dxvTexture;
	}
	~CTexturedVertex(){}
};

class CTexturedNormalVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

	CTexturedNormalVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z);
		m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz);
		m_d3dxvTexCoord = D3DXVECTOR2(u, v);
	}
	CTexturedNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvTexCoord)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
		m_d3dxvTexCoord = d3dxvTexCoord;
	}
	CTexturedNormalVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}
	~CTexturedNormalVertex() {}
};



/*************************************** Mesh *****************************************/
class CMesh
{
public:
	//CMesh Ŭ������ �����ڿ� �Ҹ��ڸ� �����Ѵ�. 
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	//CMesh Ŭ���� ��ü�� ����(Reference)�� ���õ� ��� ������ �Լ��� �����Ѵ�.
private:
	int m_nReferences;

public:
	void AddRef();
	void Release();

protected:
	//���� �����͸� �����ϱ� ���� ���� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11Buffer *m_pd3dVertexBuffer;
	// �ε��� ���������� : ���� ���ۿ� ���� �ε����� ������.
	ID3D11Buffer* m_pd3dIndexBuffer;

	/*���� ������ ���� ����, ������ ����Ʈ ��, ���� �����Ͱ�
	���� ������ ��𿡼����� �����ϴ� ���� ��Ÿ���� ������ �����Ѵ�.*/
	UINT m_nVertices;
	UINT m_nStride;
	UINT m_nOffset;

	// �ε��� ���۰� �����ϴ� �ε����� ����
	UINT m_nIndices;
	// �ε��� ���ۿ��� �޽��� ǥ���ϱ� ���� ���Ǵ� ���� �ε���
	UINT m_nStartIndex;
	// �� �ε����� ������ �ε����̴�.
	int m_nBaseVertex;
	

	

	//���� �����Ͱ� � ������Ƽ�긦 ǥ���ϰ� �ִ� ���� ��Ÿ���� ��� ������ �����Ѵ�.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// �����Ͷ����� ���� ��ü�� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11RasterizerState* m_pd3dRasterizerState;
	



	//���� �����͸� �������ϴ� ��� �Լ��� �����Ѵ�.
public:
	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device* pd3dDevice);
	

};




class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D11Device *pd3dDevice);
	virtual ~CTriangleMesh(){};

	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device* pd3dDevice);
	
};



class CCubeMesh : public CMesh
{
public:
	// ������ü�� ����, ����, ������ ũ�⸦ �����Ͽ� ������ü�� �޽��� ����
	CCubeMesh(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f,
		float fDepth = 2.0f, D3DXCOLOR d3dxcolor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	
	virtual ~CCubeMesh();

	virtual void CreateRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};

/********************************** ����޽� Ŭ���� ***********************************/

// �� Ŭ������ ������ ������ �޴� �޽��� ���̽� Ŭ�����̴�.
class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device* pd3dDevice);
	virtual ~CMeshIlluminated();

public:
	//������ ���Ե� �ﰢ���� �������͸� ����ϴ� �Լ��̴�.
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE* pVertices, USHORT nIndex0, USHORT nIndex1,
		USHORT nIndex2);
	void SetTriAngleListVertexNormal(BYTE* pVertices);

	// ������ ���������� ����� ����ϴ� �Լ��̴�.
	void SetAverageVertexNormal(BYTE* pVertices, WORD* pIndices, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(BYTE* pVertices, WORD* pIndices);
	
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};
// ������ ����ϴ� ������ü �޽� Ŭ����
class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
	CCubeMeshIlluminated(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);

	virtual ~CCubeMeshIlluminated();

	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};

// ������ ����ϴ� �� �޽� Ŭ����
class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshIlluminated();
	
	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);



};
/*
CTexturedCubeMesh Ŭ������ CMesh Ŭ�������� �Ļ��Ͽ� ������ ���� �����Ѵ�. ��Ŭ������
�ؽ��� ������ ����ϴ� ������ü �޽� Ŭ������ ���Ѵ�.
*/

class CTexturedCubeMesh : public CMesh
{
public:
	CTexturedCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth);
	virtual ~CTexturedCubeMesh();

	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
};

/*
�� Ŭ������ ����� �ؽ��� ������ ����ϴ� ������ü �޽� Ŭ�����̴�.
*/

class CCubeMeshIlluminatedTextured : public CMeshIlluminated
{
public:
	CCubeMeshIlluminatedTextured(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminatedTextured();

	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);
};

class CSBXTestMesh : public CMeshIlluminated
{
public:
	CSBXTestMesh(ID3D11Device* pd3dDevice);
	virtual ~CSBXTestMesh();
	
	template <typename VertexType>
	void SetVertices(ID3D11Device* pd3dDevice, const VertexType* pVertices, UINT vCount);
	void SetIndices(ID3D11Device* pd3dDevice, const WORD* pIndices, UINT iCount);

	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext);


};


template <typename VertexType>
void CSBXTestMesh::SetVertices(ID3D11Device* pd3dDevice, const VertexType* pVertices, UINT vCount)
{
	//cubeFile = *(SONImporter::Instance()->GetSBXFile()[0]);
	m_nVertices = vCount;
	m_nStride = sizeof(VertexType);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;			// �������� - Ʈ���̾ޱ� ��Ʈ��

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);
}

class CHeightMapGridMesh : public CMesh
{
protected:
	//������ ũ��(����: x-����, ����: z-����)�̴�.
	int m_nWidth;
	int m_nLength;
	/*������ ������(����: x-����, ����: z-����, ����: y-����) �����̴�. ���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������ x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ���´�. ��, ���� ������ x-�� ������ ������ 1�� �ƴ϶� ������ ������ x-��ǥ�� �ȴ�. �̷��� �ϸ� ���� ���ڸ� ����ϴ��� ū ���ڸ� ������ �� �ִ�.*/
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
	virtual ~CHeightMapGridMesh();

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	CDiffusedVertex* m_pVertices;

	CDiffusedVertex* GetVertex() { return m_pVertices; }

	//������ ����(����)�� ���̸� �����Ѵ�.
	virtual float OnSetHeight(int x, int z, void *pContext);
	//������ ����(����)�� ������ �����Ѵ�.
	virtual D3DXCOLOR OnSetColor(int x, int z, void *pContext);

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};