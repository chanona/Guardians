#ifndef LogoBack_h__
#define LogoBack_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CLogoBack	: public Engine::CGameObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBack(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
private:
	LPD3DXEFFECT				m_pEffect;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // LogoBack_h__
