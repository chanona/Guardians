
//For.Getter------------------------------------------------------------------------------------------------------------
CResourceMgr* Get_ResourceMgr(void)
{
	return CResourceMgr::GetInstance();
}

//For.Setter------------------------------------------------------------------------------------------------------------


//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Engine::Reserve_ContainerSize(const _ushort& wSize)
{
	return Engine::CResourceMgr::GetInstance()->Reserve_ContainerSize(wSize);
}
inline HRESULT Engine::Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, CVIBuffer::BUFFERTYPE eBufferType, const TCHAR* pResourceKey, const _ushort& wCntX/* = 0*/, const _ushort& wCntZ/* = 0*/, const _ushort& wItv/* = 1*/)
{
	return Engine::CResourceMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, eBufferType, pResourceKey, wCntX, wCntZ, wItv);	
}
inline HRESULT Engine::Ready_Texture(LPDIRECT3DDEVICE9 pDevice, const _ushort& wContainerIdx, const TCHAR* pResourceKey, CTexture::TEXTURETYPE eTextureType, const TCHAR* pFilePath, const _ushort& wCnt/* = 1*/)
{
	return Engine::CResourceMgr::GetInstance()->Ready_Texture(pDevice, wContainerIdx, pResourceKey, eTextureType, pFilePath, wCnt);
}
inline HRESULT Engine::Ready_Mesh(LPDIRECT3DDEVICE9 pDevice, const _ushort& wContainerIdx, CMesh::MESHTYPE eMeshType, const TCHAR* pMeshKey, const TCHAR* pFilePath, const TCHAR* pFileName)
{
	return Engine::CResourceMgr::GetInstance()->Ready_Mesh(pDevice, wContainerIdx, eMeshType, pMeshKey, pFilePath, pFileName);
}

inline CResources* Engine::Clone_Resource(const _ushort& wContainerIdx, const TCHAR* pResourceKey)
{
	return Engine::CResourceMgr::GetInstance()->Clone_Resource(wContainerIdx, pResourceKey);
}

inline void Engine::Reset_Resource(const _ushort& wContainerIdx)
{
	Engine::CResourceMgr::GetInstance()->Reset_Resource(wContainerIdx);
}

//For.Release------------------------------------------------------------------------------------------------------------
inline void Engine::Release_Resource(void)
{

	Engine::CResourceMgr::GetInstance()->DestroyInstance();
}