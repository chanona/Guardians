#ifndef Pet_h__
#define Pet_h__

#include "Defines.h"
#include "LandObject.h"
#include "MouseCol.h"
#include "Material.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CPet : public CLandObject
{
public:
	explicit CPet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPet(void);

	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	virtual void Clear();

	void Move(const _float& fTimeDelta);
public:
	static CPet* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CDynamicMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_float						m_fTimeDelta;
	_uint						m_iAniIdx;

	bool			m_bMove;
	D3DXVECTOR3		m_vDestPos;

	Engine::CMaterial*			m_pMtrlCom;

	// Server
	bool m_bAlive;
private:		// MonsterStat
	int			m_iHP;
	int			m_iAtt;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);

	bool GetAlive() const { return m_bAlive; }
	void SetAlive(const bool alive) { m_bAlive = alive; }
	void SetHP(const int hp) { m_iHP = hp; }
	int GetHP() { return m_iHP; }

	void Reset();
};


#endif // Pet_h__
