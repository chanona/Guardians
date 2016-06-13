#ifndef Quest_h__
#define Quest_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CQuest : public Engine::CGameObject
{
private:
	explicit CQuest(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CQuest(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	enum STATE_TYPE { QUEST_START, QUEST_ING, QUEST_OK, QUEST_END };

	STATE_TYPE		m_eQuest;
	bool			m_bQuest;	// Äù½ºÆ®¸¦ ³¡³Â´Â°¡

public:
	static CQuest* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_matrix						m_matOrtho;
	_matrix						m_matView[QUEST_END];
	_float						m_fX[QUEST_END], m_fY[QUEST_END];
	_float						m_fSizeX[QUEST_END], m_fSizeY[QUEST_END];
	_float						m_fHp;
	_float						m_fMp;
	_float						m_fMaxHp;
	_float						m_fMaxMp;

	_float fPlayerHp;
	_float fPlayerMp;

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom[QUEST_END];
	Engine::CTexture*			m_pTextureCom[QUEST_END];
	Engine::VTXTEX*				m_pVertex[QUEST_END];

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};


#endif // Quest_h__
