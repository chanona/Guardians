#ifndef Monster_h__
#define Monster_h__

#include "Defines.h"
#include "LandObject.h"
#include "MouseCol.h"

namespace Engine
{
	class CTransform;
	class CDynamicMesh;
}

class CMonster : public CLandObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	void Move(const _float& fTimeDelta);
public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CDynamicMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_float						m_fTimeDelta;
	_uint						m_iAniIdx;

	bool			m_bMove;
	D3DXVECTOR3		m_vDestPos;

private:
	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);
};


#endif // Monster_h__
