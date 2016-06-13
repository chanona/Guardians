#ifndef PlayerState_h__
#define PlayerState_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CPlayerState : public Engine::CGameObject
{
private:
	explicit CPlayerState(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerState(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
	enum STATE_TYPE { STATE_FACE, STATE_HP, STATE_MP, STATE_STATE,  STATE_END };
public:
	static CPlayerState* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	LPD3DXEFFECT				m_pEffect;
private:
	_matrix						m_matOrtho;
	_matrix						m_matView[STATE_END];
	_float						m_fX[STATE_END], m_fY[STATE_END];
	_float						m_fSizeX[STATE_END], m_fSizeY[STATE_END];
	_float						m_fHp;
	_float						m_fMp;
	_float						m_fMaxHp;
	_float						m_fMaxMp;

	_float fPlayerHp;
	_float fPlayerMp;

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom[STATE_END];
	Engine::CTexture*			m_pTextureCom[STATE_END];
	Engine::VTXTEX*				m_pVertex[STATE_END];

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};



#endif // UI_h__
