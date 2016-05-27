#include "stdafx.h"
#include "Player.h"
#include "WorldManager.h"
#include "Sector.h"

CPlayer::CPlayer() : CEntity(EntityType::PLAYER)
{
	ZeroMemory(m_chatBuffer, MAX_CHAT_SIZE);
	
	m_position  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_speed     = PLAYER_SHIFT;

	m_viewBoundingSphere.Center.x = m_position.x + PLAYER_WIDTH / 2;	
	m_viewBoundingSphere.Center.y = m_position.y + PLAYER_HEIGHT / 2;
	m_viewBoundingSphere.Center.z = 0;
	m_viewBoundingSphere.Radius   = VIEW_RADIUS;

	// ToDo : 나중에 Collision 추가, Init으로 빼놓을 예정
	m_pWorld   = WORLD_MANAGER->GetWorld(WORLD_TYPE::FOREST);
	m_pSector  = nullptr;
	m_sector_x = m_position.x / SECTOR_WIDTH;
	m_sector_z = m_position.y / SECTOR_HEIGHT;

	SetHP(100);
}


CPlayer::~CPlayer()
{
}

void CPlayer::Clear()
{
	CEntity::Clear();

	ZeroMemory(m_chatBuffer, MAX_CHAT_SIZE);

	m_position  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_speed     = PLAYER_SHIFT;

	m_sector_x = m_position.x / SECTOR_WIDTH;
	m_sector_z = m_position.y / SECTOR_HEIGHT;

	SetHP(100);

	m_playerViewLock.WriteEnter();
	m_oldViewPlayerList.clear();
	m_playerViewLock.WriteLeave();

	m_monsterViewLock.WriteEnter();
	m_oldViewMonsterList.clear();
	m_monsterViewLock.WriteLeave();

	m_itemViewLock.WriteEnter();
	m_oldItemList.clear();
	m_itemViewLock.WriteLeave();
}

// 데드레커닝 하기 위해 무브함수 변경까지 한거까지 함
void CPlayer::Move(const float delta_time)
{
	m_position.x += m_direction.x * m_speed * delta_time;
	m_position.y += m_direction.y * m_speed * delta_time;
	m_position.z += m_direction.z * m_speed * delta_time;

	m_viewBoundingSphere.Center.x += m_direction.x * m_speed * delta_time;
	m_viewBoundingSphere.Center.y += m_direction.y * m_speed * delta_time;
	m_viewBoundingSphere.Center.z += m_direction.z * m_speed * delta_time;
}

void CPlayer::MoveX(const float delta_time)
{
	m_position.x += m_direction.x * m_speed * delta_time;

	m_viewBoundingSphere.Center.x += m_direction.x * m_speed * delta_time;
}

void CPlayer::MoveY(const float delta_time)
{
	m_position.y += m_direction.y * m_speed * delta_time;

	m_viewBoundingSphere.Center.y += m_direction.y * m_speed * delta_time;
}

void CPlayer::MoveZ(const float delta_time)
{
	m_position.z += m_direction.z * m_speed * delta_time;

	m_viewBoundingSphere.Center.z += m_direction.z * m_speed * delta_time;
}


void CPlayer::IncreaseHP(const int delta_hp)
{
	InterlockedAdd((LONG *)&GetHP(), delta_hp);
}

void CPlayer::DecreaseHP(const int delta_hp)
{
	InterlockedAdd((LONG *)&GetHP(), -delta_hp);
}

void CPlayer::AddPlayerInList(UINT player_id)
{
	//m_playerViewLock.WriteEnter();
	m_oldViewPlayerList.insert(player_id);
	//m_playerViewLock.WriteLeave();
}

void CPlayer::DelPlayerInList(UINT player_id)
{
	m_playerViewLock.WriteEnter();
	m_oldViewPlayerList.unsafe_erase(player_id);
	m_playerViewLock.WriteLeave();
}

void CPlayer::AddMonsterInList(UINT monster_id)
{
	//m_monsterViewLock.WriteEnter();
	m_oldViewMonsterList.insert(monster_id);
	//m_monsterViewLock.WriteLeave();
}

void CPlayer::DelMonsterInList(UINT monster_id)
{
	m_monsterViewLock.WriteEnter();
	m_oldViewMonsterList.unsafe_erase(monster_id);
	m_monsterViewLock.WriteLeave();
}

void CPlayer::SetChatBuffer(char * chatBuf, int size)
{
	assert(chatBuf != nullptr);
	assert(size != 0);

	strcpy_s(m_chatBuffer, size, chatBuf);
}

bool CPlayer::UpdateSector(int new_sector_x, int new_sector_z)
{
	if (m_pSector == nullptr)
	{
		m_pSector = m_pWorld->GetSector(new_sector_x, new_sector_z);		// Warning : 밑에 문장이랑 똑같은게 있다고 if문 밖으로 빼내면 안됨!
		m_sector_x = new_sector_x;
		m_sector_z = new_sector_z;
		m_pSector->AddPlayer(GetID());
		return false;
	}
	if (m_sector_x == new_sector_x && m_sector_z == new_sector_z) return true;

	m_pSector->DelPlayer(GetID());
	m_pSector = m_pWorld->GetSector(new_sector_x, new_sector_z);

	m_sector_x = new_sector_x;
	m_sector_z = new_sector_z;

	m_pSector->AddPlayer(GetID());

	return false;
}
