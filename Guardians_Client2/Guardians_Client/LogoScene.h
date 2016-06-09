#ifndef LogoScene_h__
#define LogoScene_h__

#include "Defines.h"
#include "Scene.h"

class CLoading;
class CLogoScene : public Engine::CScene
{
private:
	explicit CLogoScene(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~CLogoScene(void);
public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CLogoScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Resources(void);
	HRESULT Ready_Environment(void);
private:
	CLoading*				m_pLoading;
public:
	virtual _ulong Release(void);
};



#endif // LogoScene_h__
