#include "stdafx.h"
#include "ObjectManager.h"
#include "Memory.h"
#include "protocol.h"
#include "Export_Function.h"
CObjectManager::CObjectManager()
{
	m_monsterMappingHashMap.clear();
}


CObjectManager::~CObjectManager()
{
}

bool CObjectManager::Start()
{
	for (int i = 0; i < MAX_MONSTER; ++i)
	{
		CMonster *pMonster = CMonster::Create(Engine::Get_GraphicDev());
		pMonster->SetIndex(i);

		InsertMonsterToMap(i, pMonster);

		if (0 == i)
		{
			//pMonster->SetMonsterType(MonsterType::MONSTER_TYPE_END);
			continue;			// 0은 더미 NPC라서 풀에 넣을 필요없음
		}
		/*else if (i < DEFENDER_MONSTER_START)
		{
			pMonster->SetMonsterType(MonsterType::ATTACKER);
		}
		else if (i < SURPPORT_MONSTER_START)
		{
			pMonster->SetMonsterType(MonsterType::DEFENDER);
		}
		else
		{
			pMonster->SetMonsterType(MonsterType::SUPPORTER);
		}*/
		//if (0 == i) continue;

		m_monsterPool.push(pMonster);
	}

	return true;
}

void CObjectManager::ShutDown()
{
	for (auto mon : m_monsterMap)
	{
		Memory::SAFE_DELETE(mon.second);
	}
	m_monsterMap.clear();
	m_monsterMappingHashMap.clear();
}

void CObjectManager::DeleteMonster(const UINT id)
{
	CMonster* pMon = FindMonster(id);
	pMon->SetAlive(false);

	if (m_monsterMappingHashMap.erase(id))
	{
//#ifdef _DEBUG
		//cout << "CObjectManager Mapping HashMap -> Monster Erase : " << id << endl;
//#endif
	}
	InsertMonsterToPool(pMon);
	
	pMon->Clear();
}

void CObjectManager::DeleteMonster(CMonster * pMonster)
{
	if (m_monsterMappingHashMap.erase(pMonster->GetID()))
	{
//#ifdef _DEBUG
		//cout << "CObjectManager Mapping HashMap -> Monster Erase : " << pMonster->GetID() << endl;
//#endif
	}
	InsertMonsterToPool(pMonster);

	pMonster->Clear();
}

CMonster * CObjectManager::FindMonster(const UINT id)
{
	UINT index = FindIndexFromMappingTable(id);

	auto npc_data = m_monsterMap.find(index);

	return npc_data->second;
}

UINT CObjectManager::FindIndexFromMappingTable(const UINT id)
{
	auto iter = m_monsterMappingHashMap.find(id);

	if (iter == m_monsterMappingHashMap.end())
	{
		return m_monsterMap[0]->GetID();
	}
	return iter->second;
}

CMonster * CObjectManager::GetNewMonster(const int id)
{
	//if (mon_type == MonsterType::MONSTER_TYPE_END) return m_monsterMap[0];

	CMonster* pMon;

	pMon = m_monsterPool.front();
	m_monsterPool.pop();

	pMon->SetID(id);
	InsertMappingData(pMon->GetID(), pMon->GetIndex());

	return pMon;
}

void CObjectManager::InsertMonsterToMap(const UINT index, CMonster * pMon)
{
	m_monsterMap[index] = pMon;
}

void CObjectManager::InsertMappingData(const UINT id, const UINT idx)
{
	m_monsterMappingHashMap.insert(make_pair(id, idx));
}

void CObjectManager::InsertMonsterToPool(CMonster * pMon)
{
	m_monsterPool.push(pMon);
}
