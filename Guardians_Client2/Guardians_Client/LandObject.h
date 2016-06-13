#ifndef LandObject_h__
#define LandObject_h__

#include "Defines.h"
#include "GameObject.h"
#include "Transform.h"
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
	virtual void Clear() {}
protected:
	Engine::CTransform*				m_pTransCom;
	Engine::VTXTEX*					m_pVertex;
	Engine::CVIBuffer*				m_pBufferCom;

	UINT m_id;
	UINT m_index;
private:
	void Compute_Height(void);
public:
	virtual _ulong Release(void);

	void SetPosition(D3DXVECTOR3& pos) { m_pTransCom->m_vPosition = pos; }
	void SetID(UINT id) { m_id = id; }
	void SetIndex(UINT index) { m_index = index; }

	UINT GetID() const { return m_id; }
	UINT GetIndex() const { return m_index; }
	D3DXVECTOR3& GetPosition() { return m_pTransCom->m_vPosition; }
};


#endif // LandObject_h__
