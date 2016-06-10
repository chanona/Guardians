/*!
 * \file Scene.h
 * \date 2015/04/02 23:10
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
#ifndef Scene_h__
#define Scene_h__

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)
class  CScene : public Engine::CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene(void);
public:
	Engine::CComponent* Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt);
	list<Engine::CGameObject*>* Find_ObjectList(const TCHAR* pLayerTag, const TCHAR* pObjectTag);
public:
	virtual HRESULT Init_Scene(void) PURE;
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
protected:
	typedef map<const TCHAR*, CLayer*>		MAPLAYER;
	MAPLAYER								m_mapLayer;
private:
	CLayer* Find_Layer(const TCHAR* pLayerTag);
public:
	virtual _ulong Release(void);
};

END

#endif // Scene_h__