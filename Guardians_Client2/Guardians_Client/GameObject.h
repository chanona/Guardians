/*!
 * \file GameObject.h
 * \date 2015/04/03 18:11
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef GameObject_h__
#define GameObject_h__

#include "Base.h"

BEGIN(Engine)

class CComponent;
class  CGameObject : public Engine::CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);
public:
	Engine::CComponent*	Get_Component(const TCHAR* pComponentTag);

public:
	virtual HRESULT	Initialize(void) {return S_OK;}
	virtual HRESULT Add_Component(void) {return S_OK;}
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void){}
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
protected:
	typedef map<const TCHAR*, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
private:
	CComponent* Find_Component(const TCHAR* pComTag);
public:
	virtual _ulong Release(void);
};

END

#endif // GameObject_h__