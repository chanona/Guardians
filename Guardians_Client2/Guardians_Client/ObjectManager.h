#pragma once
#include "Monster.h"
#include <hash_map>

#define OBJECT_MANAGER CObjectManager::GetInstance()

class CObjectManager
{
public:
	typedef queue<CMonster *> MonsterPool;
	typedef hash_map<UINT /* ID */, UINT /* INDEX */> MonsterMappingHashMap;
	typedef unordered_map<UINT /* INDEX */, CMonster *> MonsterMap;

	CObjectManager();
	~CObjectManager();

	static CObjectManager* GetInstance()
	{
		static CObjectManager instance;
		return &instance;
	}

	bool	  Start();
	void	  ShutDown();

	void      DeleteMonster(const UINT id);
	void      DeleteMonster(CMonster *pMonster);

	CMonster* FindMonster(const UINT id);
	UINT      FindIndexFromMappingTable(const UINT id);

	CMonster* GetNewMonster(const int mon_type);

	const MonsterMap& GetMonsterMap() { return m_monsterMap; }

	void	  InsertMappingData(const UINT id, const UINT idx);
private :
	void	  InsertMonsterToMap(const UINT index, CMonster *pMon);

	void	  InsertMonsterToPool(CMonster *pMon);
private :
	MonsterPool           m_monsterPool;
	MonsterMappingHashMap m_monsterMappingHashMap;
	MonsterMap            m_monsterMap;
};

