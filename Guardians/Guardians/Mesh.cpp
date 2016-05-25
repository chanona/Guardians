#include "stdafx.h"
#include "Mesh.h"
#include "SonImporter.h"
#include "HeightMap.h"
#include "PlayerManager.h"


/*****************   Mesh Class   ********************/
CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;

	m_nStride = sizeof(CVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_nReferences = 1;
	m_pd3dRasterizerState = NULL;
}

CMesh::~CMesh()
{

	if (m_pd3dVertexBuffer)	 m_pd3dVertexBuffer->Release();
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dIndexBuffer)	 m_pd3dIndexBuffer->Release();
	
}
void CMesh::AddRef()
{
	// ���� ��ü�� CMesh Ŭ���� ��ü�� ����Ѵٸ� AddRef() �Լ��� ȣ��.
	m_nReferences++;		// ���۷��� ī���͸� 1 ������Ų��.
}
void CMesh::Release()
{
	m_nReferences--;				// �ϳ��� ��ü�� ���ٰ�� ���۷��� ī���͸� 1 ���ҽ�Ű��.
	if (m_nReferences == 0) delete this;		// 0�̵Ǵ¼��� mesh������ ��ü�� �Ҹ�.
}
void CMesh::CreateRasterizerState(ID3D11Device* pd3dDevice)
{
	
}
void CMesh::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
{
	// �޽��� �׸���. �޽� Ŭ������ ���� ���۸� 
	// ����̽� ���ؽ�Ʈ�� �����ϰ� ������Ƽ�� ������ �����Ѵ�.  
	if (m_pd3dVertexBuffer) pd3dImmediateDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_nStride, &m_nOffset);
	// �ε��� ���۰� ������ �ε��� ���۸� ����
	if (m_pd3dIndexBuffer) pd3dImmediateDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dImmediateDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	
	// �����Ͷ����� ���¸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dRasterizerState)
		pd3dImmediateDeviceContext->RSSetState(m_pd3dRasterizerState);
	
	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽��� �������ϰ� ������ 
	// ���� ���۸��� ����Ͽ� �޽��� �������Ѵ�.
	if (m_pd3dIndexBuffer) pd3dImmediateDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else pd3dImmediateDeviceContext->Draw(m_nVertices, m_nOffset);
}



/*************************************************  �ﰢ��  *******************************************/
CTriangleMesh::CTriangleMesh(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*����(�ﰢ���� ������)�� ������ �ð���� ������� 
	������, ���, �Ķ������� �����Ѵ�.D3DXCOLOR ��ũ�δ� 
	RGBA(Red, Green, Blue, Alpha) 4���� �Ķ���͸� ����Ͽ� ������ ǥ���ϱ� ���Ͽ� ����Ѵ�
	�� �Ķ���ʹ� 0.0~1.0 ������ �Ǽ����� ������.*/

	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(0.0f, 0.5f, 0.0f), 
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f), 
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(-0.5f,- 0.5f, 0.0f),
		D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));


	//���� ���۸� �����Ѵ�. CreateBuffer() �غ�
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// Default : GPU�� �б�/����� ����ϳ� CPU�� ���� �Ұ�
	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;			// �ѹ����� ũ��
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ������ ���۰� � ���������ο� ����� ���ΰ� ?
	d3dBufferDesc.CPUAccessFlags = 0;						// CPU�� Aceess ���� �ʰڴ�.

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	CreateRasterizerState(pd3dDevice);
}

void CTriangleMesh::CreateRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	// �����Ͷ����� �ܰ迡�� �ø��� ���� �ʵ��� �����Ͷ����� ���¸� �����Ѵ�.
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
	
}

// CTriangleMesh Ŭ������ Render() ��� �Լ�
// CMesh�� Render() �Լ��� �״�� ȣ���Ѵ�.
void CTriangleMesh::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
}


/************************** ���� ��ü ********************************/
CCubeMesh::CCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight,
	float fDepth,D3DXCOLOR d3dxcolor) : CMesh(pd3dDevice)
{
	m_nVertices = 8;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;			// �������� - Ʈ���̾ޱ� ��Ʈ��

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	
	
	//���� ���۴� ������ü�� ������ 8���� ���� ���� �����͸� ������.
	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, -fz), RANDOM_COLOR);		// - - -
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, +fz), RANDOM_COLOR);		// - - +
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, +fz), RANDOM_COLOR);		// + - +
	pVertices[3] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, -fz), RANDOM_COLOR);		// + - -
	pVertices[4] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, -fz), RANDOM_COLOR);		// - + -
	pVertices[5] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, +fz), RANDOM_COLOR);		// - + +
	pVertices[6] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, +fz), RANDOM_COLOR);		// + + +
	pVertices[7] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, -fz), RANDOM_COLOR);		// + + -
	

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


	/*�ε��� ���۴� ������ü�� 6���� ��(�簢��)�� ���� ���� ������ ���´�.
	�ﰢ�� ����Ʈ�� ������ü�� ǥ���� ���̹Ƿ� �� ���� 2���� �ﰢ���� ������ 
	�� �ﰢ���� 3���� ������ �ʿ��ϴ�. 
	��, �ε��� ���۴� ��ü 36(=6*2*3)���� �ε����� ������.*/
	m_nIndices = 18;

	WORD pIndices[18];

	pIndices[0]  = 5;	// 5 6 4
	pIndices[1]  = 6;	// 6 4 7
	pIndices[2]  = 4;   // 4 7 0 
	pIndices[3]  = 7;	// 7 0 3
	pIndices[4]  = 0;	// 0 3 1
	pIndices[5]  = 3;	// 3 1 2 
	pIndices[6]  = 1;   // 1 2 2
	pIndices[7]  = 2;	// 2 2 3
	pIndices[8]  = 2;	// 2 3 3	- Degenerated Index (3)
	pIndices[9]  = 3;	// 3 3 7	- Degenerated Index (3)
	pIndices[10] = 3;   // 3 7 2	- Degenerated Index (3)
	pIndices[11] = 7;   // 7 2 6 
	pIndices[12] = 2;	// 2 6 1 
	pIndices[13] = 6;	// 6 1 5
	pIndices[14] = 1;   // 1 5 0
	pIndices[15] = 5;   // 5 0 4 
	pIndices[16] = 0;
	pIndices[17] = 4; 


	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;

	// �ε������� ����
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);
	CreateRasterizerState(pd3dDevice);
}
CCubeMesh :: ~CCubeMesh()
{
}
void CCubeMesh::CreateRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}
void CCubeMesh::Render(ID3D11DeviceContext* pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
	
}


/********************************** CMeshIlluminated **************************************/

CMeshIlluminated::CMeshIlluminated(ID3D11Device* pd3dDevice) : CMesh(pd3dDevice)
{

}
CMeshIlluminated::~CMeshIlluminated()
{

}

// �� �Լ��� ���� �����Ϳ� �ε��� �����͸� ����Ͽ� ������ ���� ���͸� ����Ѵ�.
void CMeshIlluminated::CalculateVertexNormal(BYTE* pVertices, WORD* pIndices)
{
	switch (m_d3dPrimitiveTopology)
	{
		// ������Ƽ�갡 �ﰢ�� ����Ʈ�� �� �ε��� ���۰� �ִ� ���� ���� ��츦 �����Ͽ�
		// ������ ���� ���͸� ����Ѵ�. 
		// 1. �ε��� ���۸� ������� �ʴ� ���, �� ������ �������ʹ�
		// �� ������ ���Ե� �ﰢ���� ���� ���ͷ� ����Ѵ�. 
		// 2. �ε��� ���۸� ����ϴ� ���, �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������ 
		// ���� ������ �������(���Ͽ�) ����Ѵ�.
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (!pIndices)
			SetTriAngleListVertexNormal(pVertices);
		else
			SetAverageVertexNormal(pVertices, pIndices, (m_nIndices / 3), 3, false);
		
		break;
		
		// ������Ƽ�갡 �ﰢ�� ��Ʈ���� �� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������
		// ���� ������ �������(���Ͽ�) ����Ѵ�.
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pVertices, pIndices, (pIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		
		break;
	default:
		break;
	
	}
}

// �ε��� ���۸� ������� �ʴ� �ﰢ�� ����Ʈ�� ���Ͽ� ������ ���� ���͸� ����Ѵ�.
void CMeshIlluminated::SetTriAngleListVertexNormal(BYTE* pVertices)
{
	D3DXVECTOR3 d3dxvNormal;
	CNormalVertex* pVertex = NULL;
	
	// �ﰢ��(������Ƽ��)�� ������ ���ϰ� �� �ﰢ���� ���� ���͸� ����ϰ� �ﰢ���� �����ϴ� 
	// �� ������ ���� ���ͷ� �����Ѵ�.
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		d3dxvNormal = CalculateTriAngleNormal(pVertices, (i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
	
		pVertex = (CNormalVertex*)(pVertices + ((i * 3 + 0) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		
		pVertex = (CNormalVertex*)(pVertices + ((i * 3 + 1) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
		
		pVertex = (CNormalVertex*)(pVertices + ((i * 3 + 2) * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvNormal;
	}
}

// �ﰢ���� �� ������ �̿��Ͽ� �ﰢ���� ���� ���͸� ����Ѵ�
D3DXVECTOR3 CMeshIlluminated::CalculateTriAngleNormal(BYTE* pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 d3dxvP0 = *((D3DXVECTOR3*)(pVertices + (m_nStride * nIndex0)));
	D3DXVECTOR3 d3dxvP1 = *((D3DXVECTOR3*)(pVertices + (m_nStride * nIndex1)));
	D3DXVECTOR3 d3dxvP2 = *((D3DXVECTOR3*)(pVertices + (m_nStride * nIndex2)));
	
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);

	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	
	return d3dxvNormal;
	


}

// ������Ƽ�갡 �ε��� ���۸� ����ϴ� �ﰢ�� ����Ʈ �Ǵ� �ﰢ�� ��Ʈ���� ��� ������
// �������ʹ� �� ������ �����ϴ� �ﰢ���� ���� ���͵��� ������� ����Ѵ�.
void CMeshIlluminated::SetAverageVertexNormal(BYTE* pVertices, WORD* pIndices, int nPrimitves, int nOffset, bool bStrip)
{
	D3DXVECTOR3 d3dxvSumOfNormal(0.0f, 0.0f, 0.0f);
	CNormalVertex* pVertex = NULL;
	USHORT nIndex0, nIndex1, nIndex2;
	
	for (UINT j = 0; j < m_nVertices; j++)
	{
		d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitves; i++)
		{
			nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (pIndices) nIndex0 = pIndices[nIndex0];
			
			nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (pIndices) nIndex1 = pIndices[nIndex1];

			nIndex2 = (pIndices) ? pIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j))
				d3dxvSumOfNormal += CalculateTriAngleNormal(pVertices, nIndex0, nIndex1, nIndex2);

		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pVertex = (CNormalVertex*)(pVertices + (j * m_nStride));
		pVertex->m_d3dxvNormal = d3dxvSumOfNormal;
	}
}
void CMeshIlluminated::Render(ID3D11DeviceContext* pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
}

/********************************** CCubeMeshIlluminated **************************************/

CCubeMeshIlluminated::CCubeMeshIlluminated(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth)
	: CMeshIlluminated(pd3dDevice)
{
	int i = 0;
	m_nVertices = 8;
	m_nStride = sizeof(CNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;
	CNormalVertex pVertices[8];
	
	pVertices[0] = CNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[1] = CNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[2] = CNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[3] = CNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[4] = CNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[5] = CNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[6] = CNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pVertices[7] = CNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_nIndices = 36;
	
	WORD pIndices[36];
	pIndices[0] = 3; pIndices[1] = 1; pIndices[2] = 0;
	pIndices[3] = 2; pIndices[4] = 1; pIndices[5] = 3;
	pIndices[6] = 0; pIndices[7] = 5; pIndices[8] = 4;
	pIndices[9] = 1; pIndices[10] = 5; pIndices[11] = 0;
	pIndices[12] = 3; pIndices[13] = 4; pIndices[14] = 7;
	pIndices[15] = 0; pIndices[16] = 4; pIndices[17] = 3;
	pIndices[18] = 1; pIndices[19] = 6; pIndices[20] = 5;
	pIndices[21] = 2; pIndices[22] = 6; pIndices[23] = 1;
	pIndices[24] = 2; pIndices[25] = 7; pIndices[26] = 6;
	pIndices[27] = 3; pIndices[28] = 7; pIndices[29] = 2;
	pIndices[30] = 6; pIndices[31] = 4; pIndices[32] = 5;
	pIndices[33] = 7; pIndices[34] = 4; pIndices[35] = 6;

	// ���� ���͸� ����Ѵ�.
	CalculateVertexNormal((BYTE*)pVertices, pIndices);
	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CNormalVertex) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);
	
	SetRasterizerState(pd3dDevice);
}
CCubeMeshIlluminated ::~CCubeMeshIlluminated()
{

}
void CCubeMeshIlluminated::SetRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);

}
void CCubeMeshIlluminated::Render(ID3D11DeviceContext* pd3dImmediateDeviceContext)
{
	CMeshIlluminated::Render(pd3dImmediateDeviceContext);
	

}

/********************************** CSpehreMeshIlluminated **************************************/

CSphereMeshIlluminated::CSphereMeshIlluminated(ID3D11Device* pd3dDevice, float fRadius, int nSlices, int nStacks) : CMeshIlluminated(pd3dDevice)
{
	m_nVertices = (nSlices * nStacks) * 3 * 2;
	m_nStride = sizeof(CNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CNormalVertex* pVertices = new CNormalVertex[m_nVertices];
	float theta_i, theta_ii, phi_j, phi_jj;
	int k = 0;
	for (int j = 0; j < nStacks; j++)
	{
		phi_j = float(D3DX_PI * j) / (nStacks - 1);
		phi_jj = float(D3DX_PI * (j + 1)) / (nStacks - 1);
		for (int i = 0; i < nSlices; i++)
		{
			theta_i = float(2 * D3DX_PI * i) / (nSlices - 1);
			theta_ii = float(2 * D3DX_PI * (i + 1)) / (nSlices - 1);
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_j), fRadius*sinf(theta_i)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_i)*sinf(phi_jj), fRadius*sinf(theta_i)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_jj), fRadius*sinf(theta_ii)*sinf(phi_jj), fRadius*cosf(phi_jj)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pVertices[k++] = CNormalVertex(D3DXVECTOR3(fRadius*cosf(theta_ii)*sinf(phi_j), fRadius*sinf(theta_ii)*sinf(phi_j), fRadius*cosf(phi_j)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	CalculateVertexNormal((BYTE*)pVertices, NULL);
	
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
	
	delete[] pVertices;

	SetRasterizerState(pd3dDevice);
}

CSphereMeshIlluminated::~CSphereMeshIlluminated()
{

}
void CSphereMeshIlluminated::SetRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}
void CSphereMeshIlluminated::Render(ID3D11DeviceContext* pd3dImmediateDeviceContext)
{
	CMeshIlluminated::Render(pd3dImmediateDeviceContext);
}


///---------------------------------------------- CTexturedCubeMesh

CTexturedCubeMesh::CTexturedCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth)
	:CMesh(pd3dDevice)
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	CTexturedVertex pVertices[36];
	int i = 0;

	// ������ü�� �� �鿡 �ϳ���(�ٸ�) �ؽ��� �̹��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR2(0.0f, 1.0f));

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

	// ������ü�� �� �鿡 �ٸ� �ؽ��ĸ� �����Ϸ��� �ε����� ����� �� �����Ƿ� �ε��� ���۴� �������� �ʴ´�.

	SetRasterizerState(pd3dDevice);
}
CTexturedCubeMesh ::~CTexturedCubeMesh()
{

}

void CTexturedCubeMesh::SetRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}
void CTexturedCubeMesh::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}

///---------------------------------------------- CCubeMeshIlluminatedTextured

CCubeMeshIlluminatedTextured::CCubeMeshIlluminatedTextured(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth)
	: CMeshIlluminated(pd3dDevice)
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth *0.5f;

	CTexturedNormalVertex pVertices[36];
	int i = 0;
	
	// ������ü�� �� �鿡 �ؽ��� ��ü�� ���εǵ��� �ؽ��� ��ǥ�� �����Ѵ�.
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f));

	// �� ������ �������͸� ����Ѵ�.
	CalculateVertexNormal((BYTE*)pVertices, NULL);

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

	SetRasterizerState(pd3dDevice);
}

CCubeMeshIlluminatedTextured::~CCubeMeshIlluminatedTextured()
{

}

void CCubeMeshIlluminatedTextured::SetRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CCubeMeshIlluminatedTextured::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMeshIlluminated::Render(pd3dDeviceContext);
}



///---------------------------------------------- CFbxModelCube 

CSBXTestMesh::CSBXTestMesh(ID3D11Device* pd3dDevice)
	:CMeshIlluminated(pd3dDevice)
{
}
CSBXTestMesh::~CSBXTestMesh()
{
}

//template <typename VertexType>
//void CSBXTestMesh::SetVertices(ID3D11Device* pd3dDevice, const VertexType* pVertices, UINT vCount)
//{
//	//cubeFile = *(SONImporter::Instance()->GetSBXFile()[0]);
//	m_nVertices = vCount;
//	m_nStride = sizeof(VertexType);
//	m_nOffset = 0;
//	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;			// �������� - Ʈ���̾ޱ� ��Ʈ��
//
//	D3D11_BUFFER_DESC d3dBufferDesc;
//	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
//	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	d3dBufferDesc.ByteWidth = m_nStride * m_nVertices;
//	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	d3dBufferDesc.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA d3dBufferData;
//	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
//	d3dBufferData.pSysMem = pVertices;
//	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);
//
//}

void CSBXTestMesh::SetIndices(ID3D11Device* pd3dDevice, const WORD* pIndices, UINT iCount)
{
	m_nIndices = iCount;

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;

	// �ε������� ����
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);
	
	CreateRasterizerState(pd3dDevice);
	SetRasterizerState(pd3dDevice);
}
void CSBXTestMesh::SetRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CSBXTestMesh::Render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMeshIlluminated::Render(pd3dDeviceContext);
}

//// ��

CHeightMapGridMesh::CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale /*= D3DXVECTOR3(1.0f, 1.0f, 1.0f)*/, D3DXCOLOR d3dxColor /*= D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f)*/, void *pContext /*= NULL*/)
	: CMesh(pd3dDevice)
{
	//������ ����(����)�� ������ (nWidth * nLength)�̴�.
	m_nVertices = nWidth * nLength;
	m_nStride = sizeof(CDiffusedVertex);
	m_nOffset = 0;
	//���ڴ� �ﰢ�� ��Ʈ������ �����Ѵ�.
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_pVertices = new CDiffusedVertex[m_nVertices];

	m_nWidth = nWidth;
	m_nLength = nLength;
	m_d3dxvScale = d3dxvScale;

	float fHeight = 0.0f;
	//d3dxColor�� ������ �⺻ �����̴�.
	D3DXCOLOR d3dxVertexColor = d3dxColor;
	
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++)
		{
			fHeight = OnSetHeight(x, z, pContext);
			d3dxVertexColor = OnSetColor(x, z, pContext) + d3dxColor;
			m_pVertices[i++] = CDiffusedVertex(D3DXVECTOR3((x*m_d3dxvScale.x), fHeight, (z*m_d3dxvScale.z)), d3dxVertexColor);
		}
	}

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);
	
	//m_pd3dVertexBuffer->

	delete[] m_pVertices;

	m_nIndices = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
	WORD *pIndices = new WORD[m_nIndices];
	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			//Ȧ�� ��° ���̹Ƿ�(z = 0, 2, 4, ...) �ε����� ���� ������ ���ʿ��� ������ �����̴�.
			for (int x = 0; x < nWidth; x++)
			{
				//ù ��° ���� �����ϰ� ���� �ٲ� ������(x == 0) ù ��° �ε����� �߰��Ѵ�.
				if ((x == 0) && (z > 0)) pIndices[j++] = (USHORT)(x + (z * nWidth));
				//�Ʒ�, ���� ������ �ε����� �߰��Ѵ�.
				pIndices[j++] = (USHORT)(x + (z * nWidth));
				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			//¦�� ��° ���̹Ƿ�(z = 1, 3, 5, ...) �ε����� ���� ������ �����ʿ��� ���� �����̴�.
			for (int x = nWidth - 1; x >= 0; x--)
			{
				//���� �ٲ� ������(x == (nWidth-1)) ù ��° �ε����� �߰��Ѵ�.
				if (x == (nWidth - 1)) pIndices[j++] = (USHORT)(x + (z * nWidth));
				//�Ʒ�, ���� ������ �ε����� �߰��Ѵ�.
				pIndices[j++] = (USHORT)(x + (z * nWidth));
				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
			}
		}
	}

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	delete[] pIndices;

	SetRasterizerState(pd3dDevice);

}

CHeightMapGridMesh::~CHeightMapGridMesh()
{

}

float CHeightMapGridMesh::OnSetHeight(int x, int z, void *pContext)
{
	//���� �� ��ü�� ���� �� �̹����� �ȼ� ���� ������ ���̷� ��ȯ�Ѵ�. 
	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
	int cxTerrain = pHeightMap->GetHeightMapWidth();
	float fHeight = pHeightMapImage[x + (z*cxTerrain)] * d3dxvScale.y;
	return(fHeight);
}

D3DXCOLOR CHeightMapGridMesh::OnSetColor(int x, int z, void *pContext)
{
	//������ ���� �����̴�. 
	D3DXVECTOR3 d3dxvLightDirection = -D3DXVECTOR3(0.650945f, -0.390567f, 0.650945f);
	CHeightMap *pHeightMap = (CHeightMap *)pContext;
	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
	float fRed = 1.0f, fGreen = 0.8f, fBlue = 0.6f, fScale = 0.25f;
	fScale = D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z + 1), &d3dxvLightDirection);
	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z + 1), &d3dxvLightDirection);
	fScale = (fScale / 4.0f) + 0.05f;
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;
	D3DXCOLOR d3dxColor = D3DXCOLOR(fRed * fScale, fGreen * fScale, fBlue * fScale, 1.0f);
	return(d3dxColor);


}

void CHeightMapGridMesh::SetRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CHeightMapGridMesh::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
{
	CMesh::Render(pd3dImmediateDeviceContext);
}
