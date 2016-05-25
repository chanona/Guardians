#include "stdafx.h"
#include "HeightMapTerrain.h"
#include "HeightMap.h"

CHeightMapTerrain::CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor)
{
	m_pHeightMap = HMAP;

	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;

	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//d3dxvScale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
	m_d3dxvScale = d3dxvScale;

	//지형에 사용할 높이 맵을 생성한다.
	m_pHeightMap->Initialize(pFileName, nWidth, nLength, d3dxvScale);
		//= new CHeightMap(pFileName, nWidth, nLength, d3dxvScale);

	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
	int cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	int czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	m_nMeshes = cxBlocks * czBlocks;
	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.
	m_ppMesh = new CHeightMapGridMesh*[m_nMeshes];

	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, xStart, zStart, nBlockWidth, nBlockLength, d3dxvScale, d3dxColor, m_pHeightMap);
			m_ppMesh[x + (z*cxBlocks)] = pHeightMapGridMesh;
		}
	}
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
}

CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_ppMesh)
	{
		for (int i = 0; i < m_nMeshes; i++) if (m_ppMesh[i]) delete m_ppMesh[i];
		delete[] m_ppMesh;
	}
	if (m_pHeightMap) delete m_pHeightMap;
}

void CHeightMapTerrain::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera /*= NULL*/)
{
	//지형을 렌더링하는 것은 지형을 구성하는 각 격자 메쉬를 렌더링하는 것이다.
	for (int i = 0; i < m_nMeshes; i++) m_ppMesh[i]->Render(pd3dImmediateDeviceContext);
}

