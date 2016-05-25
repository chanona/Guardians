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

	//지형의 높이 맵으로 사용할 이미지이다.
	CHeightMap *m_pHeightMap;

	//지형의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;

	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.
	D3DXVECTOR3 m_d3dxvScale;
	//지형 전체가 어떤 위치와 방향을 갖는 가를 나타내는 월드 변환 행렬이다.
	D3DXMATRIX m_d3dxmtxWorld;

	//지형을 구성하는 격자 메쉬들의 배열과 메쉬들의 개수이다.
	CHeightMapGridMesh **m_ppMesh;
	int m_nMeshes;

	void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera = NULL);
};

#endif // HeightMapTerrain_h__
