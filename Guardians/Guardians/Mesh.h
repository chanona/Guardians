#pragma once

// 정점의 색상을 무작위로 설정
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

/************************************** Vertex ****************************************/
class CVertex
{
private:
	//정점의 위치 정보(3차원 벡터)를 저장하기 위한 멤버 변수를 선언한다. 
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//CVertex 클래스의 생성자와 소멸자를 다음과 같이 선언한다. 
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};

class CDiffusedVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;

	// 정점 색상을 나타내는 멤버 변수 (D3DXCOLOR 구조체)를 선언한다.
	D3DXCOLOR m_d3dxcDiffuse;

	// 생성자 & 소멸자
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
	// 조명의 영향을계산하기 위하여 법선벡터가 필요하다.
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
	//CMesh 클래스의 생성자와 소멸자를 선언한다. 
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	//CMesh 클래스 객체의 참조(Reference)와 관련된 멤버 변수와 함수를 선언한다.
private:
	int m_nReferences;

public:
	void AddRef();
	void Release();

protected:
	//정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터를 선언한다.
	ID3D11Buffer *m_pd3dVertexBuffer;
	// 인덱스 버퍼포인터 : 정점 버퍼에 대한 인덱스를 가진다.
	ID3D11Buffer* m_pd3dIndexBuffer;

	/*정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가
	정점 버퍼의 어디에서부터 시작하는 가를 나타내는 변수를 선언한다.*/
	UINT m_nVertices;
	UINT m_nStride;
	UINT m_nOffset;

	// 인덱스 버퍼가 포함하는 인덱스의 개수
	UINT m_nIndices;
	// 인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스
	UINT m_nStartIndex;
	// 각 인덱스에 더해질 인덱스이다.
	int m_nBaseVertex;
	

	

	//정점 데이터가 어떤 프리미티브를 표현하고 있는 가를 나타내는 멤버 변수를 선언한다.
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// 래스터라이저 상태 객체에 대한 인터페이스 포인터를 선언한다.
	ID3D11RasterizerState* m_pd3dRasterizerState;
	



	//정점 데이터를 렌더링하는 멤버 함수를 선언한다.
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
	// 직육면체의 가로, 세로, 높이의 크기를 지정하여 직육면체의 메쉬를 생성
	CCubeMesh(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f,
		float fDepth = 2.0f, D3DXCOLOR d3dxcolor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	
	virtual ~CCubeMesh();

	virtual void CreateRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};

/********************************** 조명메쉬 클래스 ***********************************/

// 이 클래스는 조명의 영향을 받는 메쉬의 베이스 클래스이다.
class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device* pd3dDevice);
	virtual ~CMeshIlluminated();

public:
	//정점이 포함된 삼각형의 법선벡터를 계산하는 함수이다.
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE* pVertices, USHORT nIndex0, USHORT nIndex1,
		USHORT nIndex2);
	void SetTriAngleListVertexNormal(BYTE* pVertices);

	// 정점의 법선벡터의 평균을 계산하는 함수이다.
	void SetAverageVertexNormal(BYTE* pVertices, WORD* pIndices, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(BYTE* pVertices, WORD* pIndices);
	
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};
// 조명을 사용하는 직육면체 메쉬 클래스
class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
	CCubeMeshIlluminated(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);

	virtual ~CCubeMeshIlluminated();

	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);
};

// 조명을 사용하는 구 메쉬 클래스
class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device* pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshIlluminated();
	
	virtual void SetRasterizerState(ID3D11Device* pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dImmediateDeviceContext);



};
/*
CTexturedCubeMesh 클래스를 CMesh 클래스에서 파생하여 다음과 같이 선언한다. 이클래스는
텍스쳐 맵핑을 사용하는 직육면체 메쉬 클래스를 뜻한다.
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
이 클래스는 조명과 텍스쳐 맵핑을 사용하는 직육면체 메쉬 클래스이다.
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
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;			// 토폴로지 - 트라이앵글 스트립

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
	//격자의 크기(가로: x-방향, 세로: z-방향)이다.
	int m_nWidth;
	int m_nLength;
	/*격자의 스케일(가로: x-방향, 세로: z-방향, 높이: y-방향) 벡터이다. 실제 격자 메쉬의 각 정점의 x-좌표, y-좌표, z-좌표는 스케일 벡터의 x-좌표, y-좌표, z-좌표로 곱한 값을 갖는다. 즉, 실제 격자의 x-축 방향의 간격은 1이 아니라 스케일 벡터의 x-좌표가 된다. 이렇게 하면 작은 격자를 사용하더라도 큰 격자를 생성할 수 있다.*/
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
	virtual ~CHeightMapGridMesh();

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	CDiffusedVertex* m_pVertices;

	CDiffusedVertex* GetVertex() { return m_pVertices; }

	//격자의 교점(정점)의 높이를 설정한다.
	virtual float OnSetHeight(int x, int z, void *pContext);
	//격자의 교점(정점)의 색상을 설정한다.
	virtual D3DXCOLOR OnSetColor(int x, int z, void *pContext);

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
};