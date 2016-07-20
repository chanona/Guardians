#ifndef Shader_h__
#define Shader_h__

#include "Base.h"

// hpp, fx에 정의되어있는 hlsl이라는 언어를 번역해서 보관한다.
BEGIN(Engine)

class  CShader : public Engine::CBase
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShader(void);
public:
	LPD3DXEFFECT Get_EffectHandle(void) {
		 return m_pEffect == NULL ? NULL : m_pEffect;}
public:
	HRESULT Init_Shader(const TCHAR* pFilePath);
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath);
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrBuffer;
public:
	virtual _ulong Release(void);
};

END

#endif // Shader_h__
