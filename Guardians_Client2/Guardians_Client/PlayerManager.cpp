#include "stdafx.h"
#include "PlayerManager.h"
#include "Shader.h"
#include "Player.h"
#include "protocol.h"
#include "Memory.h"
#include "ClientNetEngine.h"
#include "Export_Function.h"

CPlayerManager::CPlayerManager()
{

}


CPlayerManager::~CPlayerManager()
{
}

bool CPlayerManager::Start()
{
	for (int i = 0; i <= MAX_CLIENT; ++i)
	{
		CPlayer *player = CPlayer::Create(Engine::Get_GraphicDev());

		player->SetIndex(i);

		InsertPlayerToMap(i, player);

		if (i == 0) continue;

		InsertPlayerToPool(player);
	}
	return true;
}

void CPlayerManager::ShutDown()
{
	for (auto& player : m_playersMap)
	{
		Memory::SAFE_DELETE(player.second);
	}
	m_playersMap.clear();
	m_playerMappingTable.clear();
}

void CPlayerManager::InsertPlayerToMap(int index, CPlayer * player)
{
	m_playersMap[index] = player;
}

void CPlayerManager::InsertPlayerToPool(CPlayer * player)
{
	m_playerPool.push(player);
}

void CPlayerManager::InsertPlayerMappingData(const UINT id, const UINT idx)
{
	m_playerMappingTable.insert(make_pair(id, idx));
}

CPlayer * CPlayerManager::AddPlayer(const UINT id)
{
	if (m_playerMappingTable.find(id) != m_playerMappingTable.end())		// 이미 ID가 존재
	{
		return m_playersMap[0];
	}

	CPlayer *new_player = m_playerPool.front();
	m_playerPool.pop();

	m_playerMappingTable.insert(make_pair(id, new_player->GetIndex()));

	new_player->SetID(id);
	//new_player->SetConnected(true);

	return new_player;
}

void CPlayerManager::DeletePlayer(const UINT id)
{
	int idx = FindIndexFromPlayerMappingTable(id);

	m_playersMap[idx]->SetConnected(false);

	m_playerPool.push(m_playersMap[idx]);

	m_playerMappingTable.erase(id);	// idx 가 아닌 id인걸 주의
}

CPlayer * CPlayerManager::FindPlayer(const UINT id)
{
	int idx = FindIndexFromPlayerMappingTable(id);

	return m_playersMap[idx];
}

UINT CPlayerManager::FindIndexFromPlayerMappingTable(UINT id)
{
	auto iter = m_playerMappingTable.find(id);
	if (iter != m_playerMappingTable.end())
	{
		return iter->second;
	}
	return 0;
}
