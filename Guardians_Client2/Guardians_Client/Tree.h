#ifndef Tree_h__
#define Tree_h__

#include "Defines.h"
#include "LandObject.h"
#include "Texture.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CTree : public CLandObject
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTree(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	_vec3						m_vViewPos;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};


#endif // 
