/*!
 * \file ResourceMgr.h
 * \date 2015/08/13 12:55
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

#ifndef ResourceMgr_h__
#define ResourceMgr_h__

#include "Engine_Defines.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

BEGIN(Engine)

class  CResourceMgr
{
	DECLARE_SINGLETON(CResourceMgr)
private:
	explicit CResourceMgr(void);
	~CResourceMgr(void);

public: //Get	
	CResources* Clone_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey);

public:
	HRESULT Reserve_ContainerSize(const _ushort& wSize);

public:
	HRESULT Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx
		, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey
		, const _ushort& wCntX = 0, const _ushort& wCntZ = 0, const _ushort& wItv = 1);

	HRESULT Ready_Texture(LPDIRECT3DDEVICE9 pDevice
		, const _ushort& wContainerIdx
		, const TCHAR* pResourceKey
		, CTexture::TEXTURETYPE eTextureType
		, const TCHAR* pFilePath
		, const _ushort& wCnt = 1);

	HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pDevice, const _ushort& wContainerIdx
		, CMesh::MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName);
public:
	void Reset_Resource(const _ushort& wContainerIdx);
private:
	map<const TCHAR*, CResources*>*				m_pmapResource;
	typedef map<const TCHAR*, CResources*>		MAPRESOURCE;	
	_ushort										m_wReservedSize;
private:
	CResources* Find_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey);
public:
	void Release(void);

};

END

#endif // ResourceMgr_h__