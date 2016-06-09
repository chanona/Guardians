/*!
 * \file Renderer.h
 * \date 2015/04/02 23:05
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 씬의 렌더링을 수행하는 클래스
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Renderer_h__
#define Renderer_h__

#include "Base.h"

BEGIN(Engine)

class CScene;
class CGameObject;
class  CRenderer : public Engine::CBase
{
public:
	enum RENDERTYPE {RENDER_PRIORITY, RENDER_ZSORT, RENDER_UI, RENDER_END, RENDER_ALPHA};
private:	
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
	~CRenderer(void);

public:
	void Add_RenderGroup(RENDERTYPE eType, Engine::CGameObject* pGameObject, _float fViewZ) 
	{
		if(eType < RENDER_END)
			m_RenderGroup[eType].push_back(pGameObject);
		if(eType == RENDER_ALPHA)	
		{
			m_mapAlpha.insert(MAPALPHA::value_type(fViewZ, pGameObject));		
		}
	}
	void SetCurrentScene(CScene* pScene);
public:
	HRESULT Ready_Renderer(void);
	void Render(void);
	void Clear_RenderGroup(void);
public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	CScene*								m_pScene;
private:
	list<Engine::CGameObject*>			m_RenderGroup[RENDER_END];
	typedef list<Engine::CGameObject*>  RENDERLIST;

	multimap<float, Engine::CGameObject*, greater<float>>			m_mapAlpha;
	typedef multimap<float, Engine::CGameObject*, greater<float>>	MAPALPHA;
private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
private:
	void Render_Priority(void);
	void Render_ZSort(void);
	void Render_Alpha(void);
	void Render_UI(void);


public:
	virtual _ulong Release(void);
};


END

#endif // Renderer_h__