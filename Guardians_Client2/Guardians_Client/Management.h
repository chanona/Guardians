/*!
 * \file Management.h
 * \date 2015/04/02 22:59
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
#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;
class CComponent;
class  CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement(void);
	~CManagement(void);
public:
	Engine::CComponent* Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt = 0);
	list<Engine::CGameObject*>* Find_ObjectList(const TCHAR* pLayerTag, const TCHAR* pObjectTag);
public:
	void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, _float fViewZ){
		if(NULL != m_pRenderer) m_pRenderer->Add_RenderGroup(eType, pGameObject, fViewZ);}
	void Add_Object(const TCHAR* pLayerTag, const TCHAR* pObjectTag, CGameObject* pGameObject);
public:
	HRESULT Ready_Management(LPDIRECT3DDEVICE9 pDevice);
	_int Update(const _float& fTimeDelta);
	void Render(void);

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	void Release(void);

private:
	CScene*		m_pScene;
	CRenderer*	m_pRenderer;

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		Engine::Safe_Release(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pGraphicDev), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);

	m_pRenderer->Clear_RenderGroup();

	return S_OK;
}

END

#endif // Management_h__