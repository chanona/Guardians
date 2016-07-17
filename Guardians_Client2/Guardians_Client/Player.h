#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "LandObject.h"
#include "MouseCol.h"
#include "Quest.h"
#include "Monster.h"
#include "Picking.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CPlayer	: public CLandObject
{
public:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Clear();

	void Move(const _float& fTimeDelta);
	void MoveToMonster(const _float& fTimeDelta);
	void SetPush(int iIndex);
	void SetAngle(float fAngle, Engine::CTransform::ANGLE eAngle);
	float GetAngle(Engine::CTransform::ANGLE eAngle) {
		return m_pTransCom->m_fAngle[eAngle];
	}
	void AutoHunt(CMonster *pMonster);
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

	// Server
	bool m_bConnected;
	UINT m_hp;

	Engine::CGameObject*		m_pNpc;
	bool		m_bNpc;
public :
	bool			m_bMove;
	D3DXVECTOR3		m_vDestPos;

	int			m_iQuestMonCnt;
	CQuest*		m_pQuest;

	bool		m_bAlive;
	bool		m_bLMouseDown;
	bool		m_bRMouseDown;
	POINT		mousePoint;
	CPicking*	picker;

private:
	void Set_ContantTable(void);
	void Check_KeyState(const _float& fTimeDelta);

public:

	void Set_MouseCol(CMouseCol* pMouse);
	void SetConnected(const bool connected) { m_bConnected = connected; }
	void SetHP(const UINT hp) { m_hp = hp; }
	void SetQuest(CQuest* pQuest) { m_pQuest = pQuest; }
	UINT GetHP() const { return m_hp; }
	void SetAlive(bool bAlive) { m_bAlive = bAlive; }

	void Reset();

public:
	virtual _ulong Release(void);


};


#endif // Player_h__
