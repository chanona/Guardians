#ifndef Npc_h__
#define Npc_h__

#include "Defines.h"
#include "LandObject.h"
#include "MouseCol.h"
#include "Material.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CNpc : public CLandObject
{
public:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc(void);

	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	void Move(const _float& fTimeDelta);
public:
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphicDev);
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

private:		// MonsterStat
	int			m_iHP;
	int			m_iAtt;

private:
	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);
};


#endif // Monster_h__
