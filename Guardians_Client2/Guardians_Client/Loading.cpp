#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include <process.h>

CLoading::CLoading(LOADINGTYPE eLoadingType)
: m_LoadType(eLoadingType)
, m_bComplete(false)
, m_pGraphicDev(NULL)
{
	ZeroMemory(m_szLoadMessage, sizeof(TCHAR) * 128);
}

CLoading::~CLoading(void)
{
	
}

CLoading::LOADINGTYPE CLoading::GetLoadType(void){	return m_LoadType; }
CRITICAL_SECTION* CLoading::GetCSKey(void){	return &m_CSKey; }
bool CLoading::GetComplete(void){ return m_bComplete; }
const TCHAR* CLoading::GetLoadingMessage(void){	return m_szLoadMessage;}

void CLoading::InitLoading(void)
{
	m_pGraphicDev = Engine::Get_GraphicDev();

	InitializeCriticalSection(&m_CSKey);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
}

void CLoading::StageLoading(void)
{
	HRESULT		hr = NULL;

	lstrcpy(m_szLoadMessage, L"버퍼 생성중...");
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_TERRAIN
		, L"Buffer_Terrain", VTXCNTX, VTXCNTZ, VTXITV)))	
	{
		MSG_BOX("Buffer_Terrain Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STAGE, Engine::CVIBuffer::TYPE_CUBE
		, L"Buffer_Cube")))	
	{
		MSG_BOX("Buffer_Cube Ready Failed");
		return;
	}

	lstrcpy(m_szLoadMessage, L"메쉬 로딩중...");
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Arrow", L"../Resource/Meshes/StaticMesh/Arrow/", L"Bow.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Stone", L"../Resource/Meshes/StaticMesh/Stone/", L"Stone.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	// 다이나믹 메쉬
	if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Player", L"../Resource/Meshes/DynamicMesh/PlayerXFile/", L"Player2.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Monster", L"../Resource/Meshes/DynamicMesh/Monster/", L"Salamanda.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Npc", L"../Resource/Meshes/DynamicMesh/Npc/", L"Npc.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Shop", L"../Resource/Meshes/StaticMesh/Shop/", L"Shop.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Pet", L"../Resource/Meshes/DynamicMesh/Pet/", L"Wolf.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC, L"Mesh_Boss", L"../Resource/Meshes/DynamicMesh/Monster/Boss/", L"Kero_Binary.X")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}

	// UI 
	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_PlayerState")))
	{
		MSG_BOX("Buffer_PlayerState Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_PlayerFace")))
	{
		MSG_BOX("Buffer_PlayerFace Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_PlayerHp")))
	{
		MSG_BOX("Buffer_PlayerHp Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_PlayerMp")))
	{
		MSG_BOX("Buffer_PlayerMp Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_PlayerEx")))
	{
		MSG_BOX("Buffer_PlayerEx Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_Tree")))
	{
		MSG_BOX("Buffer_Tree Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_QuestStart")))
	{
		MSG_BOX("Buffer_Quest1 Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_QuestIng")))
	{
		MSG_BOX("Buffer_Quest2 Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX
		, L"Buffer_QuestOk")))
	{
		MSG_BOX("Buffer_Quest3 Ready Failed");
		return;
	}

	/*if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Mesh_Sword", L"../Resource/Meshes/DynamicMesh/Sword/", L"Sword.x")))
	{
		MSG_BOX("Ready_Mesh Ready Failed");
		return;
	}*/
		
	lstrcpy(m_szLoadMessage, L"텍스쳐 로딩중...");

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/grass_%d.tga", 1)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile2", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/grass_%d.tga", 1)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile3", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/grass_%d.tga", 1)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile4", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/grass_%d.tga", 1)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}

	/*if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tile", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/tile%d.jpg", 1)))
	{
		MSG_BOX("Texture_Tile Ready Failed");
		return;
	}	
*/
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_TileNormal", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/TileNomal_%d.jpg", 1)))
	{
		MSG_BOX("Texture_TileNormal Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_ColorHeight", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/ColorHeight%d.bmp", 1)))
	{
		MSG_BOX("Texture_Tile Ready Failed");
		return;
	}	

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Floor", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Terrain/Aura%d.tga", 1)))
	{
		MSG_BOX("Texture_Terrain Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Sky", Engine::CTexture::TYPE_CUBE
		, L"../Resource/Texture/StageScene/Environment/Skybox/burger%d.dds", 4)))
	{
		MSG_BOX("Texture_Sky Ready Failed");
		return;
	}

	if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Effect", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Explosion/Explosion%d.png", 90)))
	{
		MSG_BOX("Texture_Effect Ready Failed");
		return;
	}
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerState", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/UI/PlayerState%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerState Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerFace", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/UI/PlayerFace%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerState Ready Failed");
		return;
	}
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerHp", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/UI/PlayerHp%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerState Ready Failed");
		return;
	}
	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerMp", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/UI/PlayerMp%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerState Ready Failed");
		return;
	}
	/*if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerMp", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/UI/PlayerMp%d.png", 1)))
	{
		MSG_BOX("Texture_PlayerMp Ready Failed");
		return;
	}*/

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerEx", Engine::CTexture::TYPE_NORMAL,
		L"../Resource/Texture/StageScene/UI/PlayerEx%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerEx Ready Failed");
		return;
	}

	/*if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerExBack", Engine::CTexture::TYPE_NORMAL,
		L"../Resource/Texture/StageScene/UI/PlayerExBack%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerStamina_Red Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STATIC, L"Texture_PlayerExBack2", Engine::CTexture::TYPE_NORMAL,
		L"../Resource/Texture/StageScene/UI/PlayerExBack2%d.tga", 1)))
	{
		MSG_BOX("Texture_PlayerStamina_Red Ready Failed");
		return;
	}*/

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_Tree", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Tree/Tree%d.png", 1)))
	{
		MSG_BOX("Texture_Tree Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_QuestStart", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Quest/QuestStart%d.png", 1)))
	{
		MSG_BOX("Texture_Quest00 Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_QuestIng", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Quest/QuestIng%d.png", 1)))
	{
		MSG_BOX("Texture_Quest01 Ready Failed");
		return;
	}

	if (FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_STAGE, L"Texture_QuestOk", Engine::CTexture::TYPE_NORMAL
		, L"../Resource/Texture/StageScene/Quest/QuestOk%d.png", 1)))
	{
		MSG_BOX("Texture_Quest02 Ready Failed");
		return;
	}

	lstrcpy(m_szLoadMessage, L"로딩 완료...");
	m_bComplete = true;
}



CLoading* CLoading::Create(LOADINGTYPE eLoadingType)
{
	CLoading*	pLoding = new CLoading(eLoadingType);

	pLoding->InitLoading();

	return pLoding;
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*	pLoading = (CLoading*)pArg;

	EnterCriticalSection(pLoading->GetCSKey());

	switch(pLoading->GetLoadType())
	{
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;
	}

	LeaveCriticalSection(pLoading->GetCSKey());
	return 0;
}

_ulong CLoading::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		DeleteCriticalSection(&m_CSKey);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		delete this;
	}
	return dwRefCnt;	
}
