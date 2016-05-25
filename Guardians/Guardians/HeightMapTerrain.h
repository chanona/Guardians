#ifndef HeightMapTerrain_h__
#define HeightMapTerrain_h__

#include "GameFrameWork.h"
#include "HeightMap.h"


class CHeightMap;
class CHeightMapTerrain
{
public:
	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor);
	~CHeightMapTerrain();

	//������ ���� ������ ����� �̹����̴�.
	CHeightMap *m_pHeightMap;

	//������ ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;

	//������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ���� ������ �����̴�.
	D3DXVECTOR3 m_d3dxvScale;
	//���� ��ü�� � ��ġ�� ������ ���� ���� ��Ÿ���� ���� ��ȯ ����̴�.
	D3DXMATRIX m_d3dxmtxWorld;

	//������ �����ϴ� ���� �޽����� �迭�� �޽����� �����̴�.
	CHeightMapGridMesh **m_ppMesh;
	int m_nMeshes;

	void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera = NULL);
};

#endif // HeightMapTerrain_h__
