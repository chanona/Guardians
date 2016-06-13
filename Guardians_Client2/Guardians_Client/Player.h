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

	// Server
	bool m_bConnected;
	UINT m_hp;
private:
	void Set_ContantTable(void);
	void Check_KeyState(const _float& fTimeDelta);

public:

	void Set_MouseCol(CMouseCol* pMouse);
	void SetConnected(const bool connected) { m_bConnected = connected; }
	void SetHP(const UINT hp) { m_hp = hp; }

	UINT GetHP() const { return m_hp; }
public:
	virtual _ulong Release(void);


};


#endif // Player_h__
