#pragma once

#define PLAYER_MANAGER CPlayerManager::GetInstance()

class CPlayer;
class CPlayerManager
{
public:
	typedef unordered_map<int, CPlayer*>  PlayerMap;
	typedef map<int, int>                 PlayerMappingTable;
	typedef queue<CPlayer *>              PlayerPool;

	CPlayerManager();
	~CPlayerManager();

	static CPlayerManager* GetInstance()
	{
		static CPlayerManager instance;
		return &instance;
	}

	bool Start();
	void ShutDown();

	void InsertPlayerToMap(int index, CPlayer *player);
	void InsertPlayerToPool(CPlayer *player);
	void InsertPlayerMappingData(const UINT id, const UINT idx);

	CPlayer* AddPlayer(const UINT id);
	void	 DeletePlayer(const UINT id);

	CPlayer* FindPlayer(const UINT id);
	UINT     FindIndexFromPlayerMappingTable(UINT id);
private :
	ID3D11Device *m_pDeivce;

	PlayerMap m_playersMap;
	PlayerMappingTable m_playerMappingTable;
	PlayerPool  m_playerPool;
public :
	const PlayerMap& GetPlayerMap() { return m_playersMap; }
};

