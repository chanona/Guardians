/*!
 * \file SceneSelector.h
 * \date 2015/03/03 11:48
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 새로운 씬을 생성하는 기능을 수행한다.
 *
 * \note
*/
#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "StageScene.h"
#include "LogoScene.h"

class CSceneSelector
{
public:
	enum SCENE {SCENE_LOGO, SCENE_STAGE, SCENE_END};
public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}
public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice) 
	{
		switch(m_eSceneID)
		{
		case SCENE_LOGO:
			*ppScene = CLogoScene::Create(pDevice);
			break;

		case SCENE_STAGE:
			*ppScene = CStageScene::Create(pDevice);
			break;
		}
	
		if(NULL == *ppScene)
			return E_FAIL;

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__
