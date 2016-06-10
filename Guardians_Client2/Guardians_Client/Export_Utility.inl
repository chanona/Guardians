
//For.Getter------------------------------------------------------------------------------------------------------------
inline Engine::CComponent* Engine::Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt/* = 0*/)
{
	return Engine::CManagement::GetInstance()->Get_Component(pComponentTag, pLayerTag, pObjectTag, iCnt);
}
inline list<Engine::CGameObject*>* Find_ObjectList(const TCHAR* pLayerTag, const TCHAR* pObjectTag)
{
	return Engine::CManagement::GetInstance()->Find_ObjectList(pLayerTag, pObjectTag);
}
inline LPD3DXEFFECT Engine::Get_EffectHandle(const TCHAR* pShaderTag)
{
	return Engine::CShaderMgr::GetInstance()->Get_EffectHandle(pShaderTag);
}

inline const D3DLIGHT9* Engine::Get_LightInfo(const _ulong& dwLightIdx)
{
	return Engine::CLightMgr::GetInstance()->Get_LightInfo(dwLightIdx);
}

//For.Setter------------------------------------------------------------------------------------------------------------
inline void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, float fViewZ)
{
	Engine::CManagement::GetInstance()->Add_RenderGroup(eType, pGameObject, fViewZ);
}
//For.Generic------------------------------------------------------------------------------------------------------------
inline HRESULT Engine::Ready_ShaderFromFiles(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	return Engine::CShaderMgr::GetInstance()->Ready_ShaderFromFiles(pGraphicDev, pShaderTag, pFilePath);
}
inline HRESULT Engine::Ready_Management(LPDIRECT3DDEVICE9 pDevice)
{
	return CManagement::GetInstance()->Ready_Management(pDevice);
}
inline _int Engine::Update_Management(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update(fTimeDelta);
}
inline void Engine::Render_Management(void)
{
	CManagement::GetInstance()->Render();
}

template <typename T> HRESULT Engine::Change_Scene(T& Functor)
{
	return CManagement::GetInstance()->SceneChange(Functor);
}

inline HRESULT Engine::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, dwLightIdx);
}

inline _bool Engine::Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld)
{
	return CCollision::GetInstance()->Collision_AABB(vDestMin, vDestMax, mDestWorld, vSourMin, vSourMax, mSourWorld);
}
inline _bool Engine::Collision_OBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld)
{
	return CCollision::GetInstance()->Collision_OBB(vDestMin, vDestMax, mDestWorld, vSourMin, vSourMax, mSourWorld);
}


//For.Release------------------------------------------------------------------------------------------------------------
inline void Engine::Release_Light(void)
{
	CLightMgr::GetInstance()->DestroyInstance();
}

inline void Engine::Release_Utility(void)
{
	CShaderMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}