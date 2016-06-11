#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "LandObject.h"
#include "MouseCol.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CPlayer	: public CLandObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	void Move(const _float& fTimeDelta);
	void MoveToMonster(const _float& fTimeDelta);
	void SetPush(int iIndex);
public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:	
	Engine::CDynamicMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_float						m_fTimeDelta;
	_bool						m_bPush;
	_uint						m_iAniIdx;

	CMouseCol*					m_pMouseCol;
	Engine::CGameObject*		m_pMonster;

	bool			m_bMove;
	D3DXVECTOR3		m_vDestPos;

private:
	void Set_ContantTable(void);
	void Check_KeyState(const _float& fTimeDelta);

public:
	void Set_MouseCol(CMouseCol* pMouse);


public:
	virtual _ulong Release(void);


};


#endif // Player_h__
