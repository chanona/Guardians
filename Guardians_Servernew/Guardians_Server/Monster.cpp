#include "stdafx.h"
#include "Monster.h"
#include "WorldManager.h"
#include "SessionManager.h"
#include "ObjectManager.h"
#include "NetEngine.h"

CMonster::CMonster() : CEntity(EntityType::MONSTER)
{
	m_index     = 0;
	m_bAlive    = false;
	m_bPet      = false;
	m_bActive   = false;
	m_position  = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_speed     = 10;

	// ToDo : ���߿� Collision �߰�
	m_pWorld   = WORLD_MANAGER->GetWorld(WORLD_TYPE::FOREST);
	m_sector_x = 0;
	m_sector_z = 0;
	m_pSector  = nullptr;

	m_viewBoundingSphere.Center.x = 0;
	m_viewBoundingSphere.Center.y = 0;
	m_viewBoundingSphere.Center.z = 0;
	m_viewBoundingSphere.Radius = VIEW_RADIUS;

	SetHP(100);
}


CMonster::~CMonster()
{
}

void CMonster::Initalize()
{
	// XML�� �޾ƿ� ����...
	//Clear();
}

void CMonster::Clear()
{
	GetSector()->DelMonster(GetID());
	CEntity::Clear();

	SetPosition(XMFLOAT3(PLAYER_INIT_X, PLAYER_INIT_Y, PLAYER_INIT_Z ));
	m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	
	m_sector_x = m_position.x / SECTOR_WIDTH;
	m_sector_z = m_position.y / SECTOR_HEIGHT;
	m_pSector  = nullptr;	// �� nullptr �� �ؾ� ó�� ����������� ���;ȿ� ����Ʈ�� ������.
	m_bAlive   = false;		// false��..? ���߿� �������Լ� ���� ������
	m_bActive  = false;

	SetHP(100);
}

void CMonster::Move()
{	
	if (m_bActive == false) return;

	sc_packet_monster_pos pos_pkt;
	sc_packet_put_monster put_pkt;
	UINT monster_id = GetID();

	put_pkt.size         = sizeof(sc_packet_put_monster);
	put_pkt.type         = SCPacketType::SC_PUT_MONSTER;
	put_pkt.monster_id   = monster_id;
	put_pkt.monster_type = GetMonsterType();
	put_pkt.hp           = GetHP();

	pos_pkt.size	   = sizeof(sc_packet_monster_pos);
	pos_pkt.type	   = SCPacketType::SC_MONSTER_POS;
	pos_pkt.monster_id = monster_id;

	
	XMFLOAT3 old_pos = m_position;
	XMFLOAT3 new_pos;

	int sector_x = WORLD_MANAGER->GetNewSectorXWithObject(this);
	int sector_y = WORLD_MANAGER->GetNewSectorZWithObject(this);
	
	UpdateSector(sector_x, sector_y);

	CPlayer::ViewList old_player_view_list;

	// �̵���Ű������ OldList�� ���� ���߿� NewList�� ��
	for (int y = sector_y - 1; y <= sector_y + 1; ++y)				
	{
		if (y < 0 || y >= SECTOR_Z_LENGTH) continue;
		for (int x = sector_x - 1; x <= sector_x + 1; ++x)
		{
			if (x < 0 || x >= SECTOR_X_LENGTH) continue;

			CSector* sector = GetWorld()->GetSector(x, y);
			
			sector->RegisterPlayerInRange(this, old_player_view_list);
		}
	}

	new_pos.x = old_pos.x + rand() % 50 - 10;
	new_pos.y = old_pos.y;	// ���߿� Z�� ����
	new_pos.z = old_pos.z + rand() % 50 - 10;

	if (new_pos.x < 0) new_pos.x = 0;
	if (new_pos.x >= WORLD_WIDTH) new_pos.x = WORLD_WIDTH - 1;
	if (new_pos.z < 0) new_pos.z = 0;
	if (new_pos.z >= WORLD_HEIGHT) new_pos.z = WORLD_HEIGHT - 1;

	put_pkt.x = pos_pkt.x = new_pos.x;
	put_pkt.x = pos_pkt.y = new_pos.y;
	put_pkt.x = pos_pkt.z = new_pos.z;

	SetPosition(new_pos);

	// NewViewList�� ����� ���� ������ ���� �̿��ؼ� �ٽ� Sector ������Ʈ
	sector_x = WORLD_MANAGER->GetNewSectorXWithObject(this);
	sector_y = WORLD_MANAGER->GetNewSectorZWithObject(this);

	UpdateSector(sector_x, sector_y);

	CSector::ObjectList new_player_view_list;

	for (int y = sector_y - 1; y <= sector_y + 1; ++y)
	{
		if (y < 0 || y >= SECTOR_Z_LENGTH) continue;
		for (int x = sector_x - 1; x <= sector_x + 1; ++x)
		{
			if (x < 0 || x >= SECTOR_X_LENGTH) continue;

			CSector* sector = GetWorld()->GetSector(x, y);

			sector->RegisterPlayerInRange(this, new_player_view_list);
		}
	}

	// ���� ���� �� �÷��̾� ����Ʈ���� �� �÷��̾ ���� �ִ� ���� ����Ʈ�� �� ���Ͱ� �ֳ� ������ ���� ó��
	for (auto player_id : new_player_view_list)
	{
		CClientSession* target_session = SESSION_MANAGER->FindSession(player_id);
		CPlayer*        player         = target_session->GetPlayer();
		
		if (!player->GetOldViewMonsterList().count(monster_id))			//  �÷��̾ ���� ����Ʈ�� �ش� ���Ͱ� �������� ������
		{
			target_session->OnceSend((char *)&put_pkt);		 	// �ڽſ� �þ߿� Ÿ�� �÷��̾ ���̰� �Ѵ�.
		}
		else
		{
			target_session->OnceSend((char *)&pos_pkt);
		}
	}

	sc_packet_remove_monster rem_pkt;
	rem_pkt.size       = sizeof(rem_pkt);
	rem_pkt.type       = SC_REMOVE_MONSTER;
	rem_pkt.monster_id = this->GetID();

	for (auto player_id : old_player_view_list)
	{
		if (new_player_view_list.count(player_id)) continue;

		CClientSession* target_session = SESSION_MANAGER->FindSession(player_id);
		CPlayer*        player         = target_session->GetPlayer();

		target_session->OnceSend((char *)&rem_pkt);

		player->DelMonsterInList(monster_id);
	}

	if (new_player_view_list.size() == 0)
	{
		this->Sleep();
	}
	else
	{
		SetPreAiTime(GetPreAiTime() + 1000);

		CEventMessage* evt = new CEventMessage(GetID(), GetID(), GetPreAiTime(), EventType::NPC_MOVE, GetEntityType(), GetEntityType());
		NETWORK_ENGINE->GetTimerThread()->AddEvent(evt);
	}
}

bool CMonster::UpdateSector(int new_sector_x, int new_sector_y)
{
	if (m_pSector == nullptr)
	{
		m_pSector = m_pWorld->GetSector(new_sector_x, new_sector_y);		// Warning : �ؿ� �����̶� �Ȱ����� �ִٰ� if�� ������ ������ �ȵ�!
		m_sector_x = new_sector_x;
		m_sector_z = new_sector_y;
		m_pSector->AddMonster(GetID());
		return false;
	}
	if (m_sector_x == new_sector_x && m_sector_z == new_sector_y) return true;

	m_pSector->DelMonster(GetID());
	m_pSector = m_pWorld->GetSector(new_sector_x, new_sector_y);

	m_sector_x = new_sector_x;
	m_sector_z = new_sector_y;

	m_pSector->AddMonster(GetID());

	return false;
}

void CMonster::Awake()
{
	SetActive(true);
	SetPreAiTime(GetTickCount());

	CEventMessage* evt = new CEventMessage(GetID(), GetID(), GetPreAiTime(), EventType::NPC_MOVE, GetEntityType(), GetEntityType());
	NETWORK_ENGINE->GetTimerThread()->AddEvent(evt);
}

void CMonster::Sleep()
{
	SetActive(false);
}

int CMonster::GetDamagaed(const int hit_damage)
{
	if (m_bActive == false) return 0;

	InterlockedAdd((volatile LONG *)m_hp, -hit_damage);
	
	if (m_hp < 0) {
		m_hp = 0;
		this->SetActive(false);
	}

	return m_hp;
}

