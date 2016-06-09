#ifndef MouseCol_h__
#define MouseCol_h__

#include "Collision.h"
#include "Defines.h"
#include "GameObject.h"

class CMouseCol
	: public Engine::CGameObject
{
private:
	CMouseCol(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMouseCol(void);

public:
	static POINT GetMousePos(void);
	static CMouseCol* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	void PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx);
	void PickObject(D3DXVECTOR3* pOut
		, const Engine::VTXTEX* pVertex
		, const D3DXMATRIX* pmatWorld);

private:
	HRESULT Initialize(void);
	void Translation_ViewSpace(void);
	void Translation_Local(const D3DXMATRIX* pWorld);

public:
	void Set_Proj(D3DXMATRIX matProj) { m_matCameraProj = matProj; }
	void Set_View(D3DXMATRIX matView) { m_matCameraView = matView; }

private:
	D3DXVECTOR3		m_vPivotPos;
	D3DXVECTOR3		m_vRayDir;

	D3DXMATRIX		m_matCameraView;
	D3DXMATRIX		m_matCameraProj;

public:
	virtual _ulong Release(void);
};

#endif // MouseCol_h__
