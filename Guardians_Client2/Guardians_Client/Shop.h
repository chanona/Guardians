#ifndef Shop_h__
#define Shop_h__

#include "Defines.h"
#include "LandObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CShop : public CLandObject
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShop(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CShop* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CStaticMesh*		m_pMeshCom;
private:
	_bool						m_bColl;
private:
	LPD3DXEFFECT				m_pEffect;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // Shop_h__
