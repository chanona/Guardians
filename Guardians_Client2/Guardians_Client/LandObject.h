#ifndef LandObject_h__
#define LandObject_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTransform;
};

// 지형을 태운다.
class CLandObject : public Engine::CGameObject
{
protected:
	explicit CLandObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLandObject(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void) {}
protected:
	Engine::CTransform*				m_pTransCom;
	Engine::VTXTEX*					m_pVertex;
	Engine::CVIBuffer*				m_pBufferCom;
private:
	void Compute_Height(void);
public:
	virtual _ulong Release(void);
};


#endif // LandObject_h__
