/*!
 * \file Texture.h
 * \date 2015/07/13 15:27
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
#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class  CTexture
	: public CResources
{
public:
	enum TEXTURETYPE {TYPE_NORMAL, TYPE_CUBE, TYPE_END};
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CTexture(void);
public:
	void Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantTag, const _ulong& dwIndex = 0) {
		pEffect->SetTexture(pConstantTag, m_vecTexture[dwIndex]);}
public:
	virtual CResources* Clone_Resource(void);
public:
	HRESULT Load_Texture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);
public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice
		, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
private:
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
	VECTEXTURE		m_vecTexture;
private:
	_ulong										m_dwContainerSize;
public:
	virtual _ulong Release(void);
};

END

#endif // Texture_h__