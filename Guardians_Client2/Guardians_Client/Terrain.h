#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CTerrain	: public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CTexture*			m_pFloorTextureCom;
	Engine::CTexture*			m_pTileTextureCom;
	Engine::CTexture*			m_pColorHeightTextureCom;
private:
	LPD3DXEFFECT				m_pEffect;
	LPDIRECT3DTEXTURE9			m_pGaraTexture;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // Terrain_h__
