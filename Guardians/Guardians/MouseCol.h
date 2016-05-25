#ifndef MouseCol_h__
#define MouseCol_h__

#define MOUSECOL CMouseCol::GetInstance()

#include "HeightMapTerrain.h"

class CMouseCol
{
public:
	CMouseCol();
	~CMouseCol();

	static CMouseCol* GetInstance()
	{
		static CMouseCol instance;
		return &instance;
	}

	POINT GetMousePos(void);
	void Translation_ViewSpace(void);
	void Translation_Local(const D3DXMATRIX* pWorld);
	int PickTerrain(CDiffusedVertex* pTerrainVtx, WORD wCntX, WORD wCntZ);

	D3DXVECTOR3		m_vPivotPos;
	D3DXVECTOR3		m_vRayDir;

	D3DXMATRIX		m_matProj;

	void Release();
};


#endif // MouseCol_h__
