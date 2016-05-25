#ifndef HeightMap_h__
#define HeightMap_h__

#include "GameFrameWork.h"

// 높이 맵 이미지를 나타내는 클래스

#define HMAP CHeightMap::GetInstance()

class CHeightMap
{
private:
	//높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 각 픽셀은 0~255의 값을 갖는다.
	BYTE *m_pHeightMapImage;
	//높이 맵 이미지의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;
	//높이 맵을 실제로 몇 배 확대하여 사용할 것인가를 나타내는 스케일 벡터이다.
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMap();
	~CHeightMap(void);

	static CHeightMap* GetInstance()
	{
		static CHeightMap instance;
		return &instance;
	}
	
	void Initialize(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale);

	//높이 맵 이미지에서 (x, z) 위치의 픽셀 값에 기반한 지형의 높이를 반환한다.
	float GetHeight(float x, float z, bool bReverseQuad = false);
	//높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다.
	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

#endif // HeightMap_h__
