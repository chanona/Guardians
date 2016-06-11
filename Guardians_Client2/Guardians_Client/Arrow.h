#ifndef Arrow_h__
#define Arrow_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CArrow : public Engine::CGameObject
{
private:
	explicit CArrow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CArrow(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CArrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	void Set_Monster(CGameObject* pGameObject);
	void Set_Position(const D3DXVECTOR3& vPosition);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;
	const _matrix*				m_pParentMatrix;
	const _matrix*				m_pParentWorldMatrix;
	CGameObject*				m_pMonster;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};

#endif // Arrow_h__