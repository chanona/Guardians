#ifndef HeightMap_h__
#define HeightMap_h__

#include "GameFrameWork.h"

// ���� �� �̹����� ��Ÿ���� Ŭ����

#define HMAP CHeightMap::GetInstance()

class CHeightMap
{
private:
	//���� �� �̹��� �ȼ�(8-��Ʈ)���� ������ �迭�̴�. �� �ȼ��� 0~255�� ���� ���´�.
	BYTE *m_pHeightMapImage;
	//���� �� �̹����� ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;
	//���� ���� ������ �� �� Ȯ���Ͽ� ����� ���ΰ��� ��Ÿ���� ������ �����̴�.
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

	//���� �� �̹������� (x, z) ��ġ�� �ȼ� ���� ����� ������ ���̸� ��ȯ�Ѵ�.
	float GetHeight(float x, float z, bool bReverseQuad = false);
	//���� �� �̹������� (x, z) ��ġ�� ���� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

#endif // HeightMap_h__
