#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CShader;
class  CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)
private:
	explicit CShaderMgr(void);
	~CShaderMgr(void);
public: // Getter
	LPD3DXEFFECT Get_EffectHandle(const TCHAR* pShaderTag);
public:
	HRESULT Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);
private:
	map<const TCHAR*, CShader*>			m_mapShaders;
	typedef map<const TCHAR*, CShader*>	MAPSHADERS;
private:
	CShader* Find_Shader(const TCHAR* pShaderTag);
public:
	void Release(void);
};


END


#endif // ShaderMgr_h__
