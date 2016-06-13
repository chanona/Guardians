#ifndef StageScene_h__
#define StageScene_h__

#include "Defines.h"
#include "Scene.h"
#include "Quest.h"

class CStageScene : public Engine::CScene
{
private:
	explicit CStageScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageScene(void);
public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CStageScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Environment(void);
	HRESULT Ready_GameLogic(void);
public:
	virtual _ulong Release(void);

public:
	CQuest*		m_pQuest;
	bool		m_bStart;

	TCHAR m_szNum[128];
};


#endif // StageScene_h__
