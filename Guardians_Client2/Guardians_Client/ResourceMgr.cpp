#include "stdafx.h"
#include "ResourceMgr.h"
#include "Resources.h"
#include "RcTex.h"
#include "Texture.h"
#include "TerrainTex.h"	
#include "CubeTex.h"


USING(Engine)
IMPLEMENT_SINGLETON(Engine::CResourceMgr)

Engine::CResourceMgr::CResourceMgr(void)
: m_pmapResource(NULL)
, m_wReservedSize(0)
{

}

Engine::CResourceMgr::~CResourceMgr(void)
{

}



Engine::CResources* Engine::CResourceMgr::Clone_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey)
{
	CResources*		pResource = Find_Resource(wContainerIdx, pResourceKey);

	if(NULL == pResource)
		return NULL;

	return pResource->Clone_Resource();
}

HRESULT Engine::CResourceMgr::Reserve_ContainerSize(const _ushort& wSize)
{
	if(0 != m_wReservedSize
		|| NULL != m_pmapResource )	
		return E_FAIL;

	m_pmapResource = new MAPRESOURCE[wSize];

	m_wReservedSize = wSize;
	return S_OK;
}

HRESULT Engine::CResourceMgr::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev
										, const _ushort& wContainerIdx 
										, CVIBuffer::BUFFERTYPE eBufferType
										, const TCHAR* pResourceKey 
										, const _ushort& wCntX /*= 0*/
										, const _ushort& wCntZ /*= 0*/
										, const _ushort& wItv /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CVIBuffer*		pBuffer = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::TYPE_RCTEX:
		pBuffer = CRcTex::Create(pGraphicDev);
		break;

	case CVIBuffer::TYPE_TERRAIN:
		pBuffer = CTerrainTex::Create(pGraphicDev, wCntX, wCntZ, wItv);
		break;

	case CVIBuffer::TYPE_CUBE:
		pBuffer = CCubeTex::Create(pGraphicDev);
		break;
	}

	NULL_CHECK_RETURN(pBuffer, E_FAIL);

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pBuffer));

	return S_OK;
}


HRESULT Engine::CResourceMgr::Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev 
										 , const _ushort& wContainerIdx 
										 , const TCHAR* pResourceKey
										 , CTexture::TEXTURETYPE eTextureType 
										 , const TCHAR* pFilePath 
										 , const _ushort& wCnt /*= 1*/)
{
	NULL_CHECK_RETURN_MSG(m_wReservedSize, E_FAIL, L"ReservedSize Not Failed");
	NULL_CHECK_RETURN_MSG(m_pmapResource, E_FAIL, L"pmapResource Not Failed");

	CResources*		pTexture = Find_Resource(wContainerIdx, pResourceKey);
	if(pTexture != NULL)
	{
		return E_FAIL;
	}

	pTexture = CTexture::Create(pGraphicDev, eTextureType, pFilePath, wCnt);

	NULL_CHECK_RETURN(pTexture, E_FAIL);

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pResourceKey, pTexture));
	return S_OK;
}

HRESULT Engine::CResourceMgr::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx , CMesh::MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName)
{
	CResources*		pResource = Find_Resource(wContainerIdx, pMeshKey);	

	switch(eMeshType)
	{
	case CMesh::TYPE_STATIC:
		pResource = CStaticMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;

	case CMesh::TYPE_DYNAMIC:
		pResource = CDynamicMesh::Create(pGraphicDev, pFilePath, pFileName);
		break;
	}
	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_pmapResource[wContainerIdx].insert(MAPRESOURCE::value_type(pMeshKey, pResource));

	return S_OK;
}

void Engine::CResourceMgr::Reset_Resource(const _ushort& wContainerIdx)
{
	for_each(m_pmapResource[wContainerIdx].begin(), m_pmapResource[wContainerIdx].end(), CRelease_Pair());

	m_pmapResource[wContainerIdx].clear();
}

Engine::CResources* Engine::CResourceMgr::Find_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey)
{
	MAPRESOURCE::iterator	iter = find_if(m_pmapResource[wContainerIdx].begin()
		, m_pmapResource[wContainerIdx].end(), Engine::CTagFinder(pResourceKey));

	if(iter == m_pmapResource[wContainerIdx].end())
		return NULL;

	return iter->second;
}

void Engine::CResourceMgr::Release(void)
{
	if(0 == m_wReservedSize)
		return;

	for (size_t i = 0; i < m_wReservedSize; ++i)
	{
		for_each(m_pmapResource[i].begin(), m_pmapResource[i].end(), CRelease_Pair());
		m_pmapResource[i].clear();
	}	

	Engine::Safe_Delete_Array(m_pmapResource);

	delete this;
}