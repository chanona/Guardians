#include "stdafx.h"
#include "HeightMapTerrain.h"
#include "HeightMap.h"

CHeightMapTerrain::CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor)
{
	m_pHeightMap = HMAP;

	//������ ����� ���� ���� ����, ������ ũ���̴�.
	m_nWidth = nWidth;
	m_nLength = nLength;

	/*���� ��ü�� ���� �޽����� �迭�� ���� ���̴�. nBlockWidth, nBlockLength�� ���� �޽� �ϳ��� ����, ���� ũ���̴�. cxQuadsPerBlock, czQuadsPerBlock�� ���� �޽��� ���� ����� ���� ���� �簢���� �����̴�.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//d3dxvScale�� ������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ����.
	m_d3dxvScale = d3dxvScale;

	//������ ����� ���� ���� �����Ѵ�.
	m_pHeightMap->Initialize(pFileName, nWidth, nLength, d3dxvScale);
		//= new CHeightMap(pFileName, nWidth, nLength, d3dxvScale);

	//�������� ���� ����, ���� �������� ���� �޽��� �� ���� �ִ� ���� ��Ÿ����.
	int cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	int czBlocks = (m_nLength - 1) / czQuadsPerBlock;
	//���� ��ü�� ǥ���ϱ� ���� ���� �޽��� �����̴�.
	m_nMeshes = cxBlocks * czBlocks;
	//���� ��ü�� ǥ���ϱ� ���� ���� �޽��� �����̴�.
	//���� ��ü�� ǥ���ϱ� ���� ���� �޽��� ���� ������ �迭�� �����Ѵ�.
	m_ppMesh = new CHeightMapGridMesh*[m_nMeshes];

	CHeightMapGridMesh *pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//������ �Ϻκ��� ��Ÿ���� ���� �޽��� ���� ��ġ�̴�.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//������ �Ϻκ��� ��Ÿ���� ���� �޽��� �����Ͽ� ���� �޽��� �����Ѵ�.
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
	//������ �������ϴ� ���� ������ �����ϴ� �� ���� �޽��� �������ϴ� ���̴�.
	for (int i = 0; i < m_nMeshes; i++) m_ppMesh[i]->Render(pd3dImmediateDeviceContext);
}

