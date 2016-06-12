/*!
 * \file Layer.h
 * \date 2015/04/03 16:06
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
#ifndef Layer_h__
#define Layer_h__

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class  CLayer : public Engine::CBase
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLayer(void);
public:
	Engine::CComponent* Get_Component(const TCHAR* pComponentTag, const TCHAR* pObjectTag, const _uint& iCnt);
	

public:
	HRESULT Ready_Object(const TCHAR* pObjectTag, CGameObject* pGameObject);

public:
	_int Update(const _float& fTimeDelta);
	void Render(void);
public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<const TCHAR*, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST								m_mapObjlist;
public:	
	list<CGameObject*>* Find_ObjectList(const TCHAR* pObjectTag);

public:
	virtual _ulong Release(void);
};

END

#endif // Layer_h__