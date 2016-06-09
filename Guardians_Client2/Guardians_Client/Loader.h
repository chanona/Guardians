#ifndef Loader_h__
#define Loader_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CHierarchyLoader : public ID3DXAllocateHierarchy
{
public:
	explicit CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath);
	virtual ~CHierarchyLoader(void);

	//------------------------------------------------------------------------
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
		LPD3DXFRAME *ppNewFrame) ;
	//------------------------------------------------------------------------
	STDMETHOD(CreateMeshContainer)(THIS_ 
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData, 
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer) ;
	//------------------------------------------------------------------------
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) ; 
	//------------------------------------------------------------------------
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) ; 
private:
	// 이름을 할당한다(복사)
	HRESULT Allocate_Name(char** pDestName, const char* pSourName);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	const TCHAR*					m_pPath;

};

END


#endif // Loader_h__
