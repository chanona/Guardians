#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CEffect	: public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);
public:
	const _vec3* Get_ViewPos(void) {
		return &m_vViewPos;}
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CTransform*			m_pTransCom;	
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

private:
	_float						m_fAccFrame;
	_vec3						m_vViewPos;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // Effect_h__
