#include "stdafx.h"
#include "PacketManager.h"
#include "ClientNetEngine.h"
#include "Resource.h"
#include "PlayerManager.h"
#include "Player.h"
#include "ObjectManager.h"
#include "HeightMap.h"

CPacketManager::CPacketManager()
{
}


CPacketManager::~CPacketManager()
{
}

bool CPacketManager::Start()
{
	m_packetProcessFuncTable.reserve(SCPacketType::SCPACKET_TYPE_END);

	m_packetProcessFuncTable[SCPacketType::SC_CHAT] = std::bind(&CPacketManager::ProcessChat, 
															     this, 
																 std::placeholders::_1, 
																 std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_PLAYER_POS] = std::bind(&CPacketManager::ProcessPlayerPosition,
															this,
															std::placeholders::_1,
															std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_LOGIN] = std::bind(&CPacketManager::ProcessLogin,
														this,
														std::placeholders::_1,
														std::placeholders::_2);

	m_packetProcessFuncTable[SCPacketType::SC_PUT_PLAYER] = std::bind(&CPacketManager::ProcessPutPlayer,
																	this,
																	std::placeholders::_1,
																	std::placeholders::_2);

	m_packetProcessFuncTable[SCPacketType::SC_REMOVE_PLAYER] = std::bind(&CPacketManager::ProcessRemovePlayer,
																	this,
																	std::placeholders::_1,
																	std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_PLAYER_HP] = std::bind(&CPacketManager::ProcessPlayerHP,
																	this,
																	std::placeholders::_1,
																	std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_MONSTER_POS] = std::bind(&CPacketManager::ProcessMonsterPosition,
																		this,
																		std::placeholders::_1,
																		std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_PUT_MONSTER] = std::bind(&CPacketManager::ProcessPutMonster,
																	this,
																	std::placeholders::_1,
																	std::placeholders::_2);
	m_packetProcessFuncTable[SCPacketType::SC_REMOVE_MONSTER] = std::bind(&CPacketManager::ProcessRemoveMonster,
																	this,
																	std::placeholders::_1,
																	std::placeholders::_2);
	return true;
}

void CPacketManager::ShutDown()
{
#if defined(SERVER)
	for (int i = 0; i < CSPacketType::CSPACKET_TYPE_END; ++i)
	{
		m_packetProcessFuncTable.erase(i);
	}
#else
	for (int i = 0; i < SCPacketType::SCPACKET_TYPE_END; ++i)
	{
		m_packetProcessFuncTable.erase(i);
	}
#endif
	m_packetProcessFuncTable.clear();
}

void CPacketManager::Assemble(UINT size_to_process, CRingBuffer* recv_ring_buffer, UINT id)
{
	static UINT prev_data_size{ 0 };
	UINT size_to_build_packet{ 0 };

	recv_ring_buffer->Commit_Enqueue(size_to_process);

	// Note : Recv 요청을 한번만 하기 때문에 락은 따로 필요없음
	while (size_to_process > 0)
	{
		char        *recv_buffer = recv_ring_buffer->GetDequeuePosttion();	
		size_to_build_packet     = recv_buffer[0] - prev_data_size;

		if (size_to_process  >= size_to_build_packet)	// 패킷 처리 가능
		{
			Dispatch(recv_buffer, id);					// 패킷 처리
			size_to_process -= size_to_build_packet;
			recv_ring_buffer->Commit_Dequeue(size_to_build_packet + prev_data_size);
			prev_data_size = 0;
		}
		else
		{
			if (prev_data_size == 0)
			{
				memcpy(recv_ring_buffer->GetBufStartPosition(), recv_buffer, size_to_process);	// 잘린패킷의 데이터를
																								// recv버퍼의 시작점으로 복사시킴
				recv_ring_buffer->Clear();
				recv_ring_buffer->Commit_Enqueue(size_to_process);
				prev_data_size  = size_to_process;
			}
			else
			{	
				prev_data_size += size_to_process;
			}
			break;	// size_to_process = 0 대신에 break; -> 안쓰면 무한루프돈다.
		}
	}
}

void CPacketManager::Dispatch(char * packet, UINT id)
{
	int type = packet[1];

#if defined(SERVER)
	assert(type < CSPacketType::CSPACKET_TYPE_END);
#else
	assert(type < SCPacketType::SCPACKET_TYPE_END);
#endif
	m_packetProcessFuncTable[type](packet, id);
}

void CPacketManager::ProcessChat(const char * packet, const UINT id)
{
	cs_packet_chat *chat_packet = (cs_packet_chat *)packet;

	cout << chat_packet->string << endl;

	// for Echo 
	sc_packet_any send_pkt;
	send_pkt.chat.size      = chat_packet->size;
	send_pkt.chat.type      = SCPacketType::SC_CHAT;
	send_pkt.chat.player_id = id;
	memcpy(send_pkt.chat.string, chat_packet->string, strlen(chat_packet->string));	// Warning : 클라이언트에서 받을때 맨 마지막 문자열 뒤에 '\0'삽입시켜야함

#if defined(ONCE_SEND)
	NETWORK_ENGINE->SendPacket((char *)&send_pkt);
	//SESSION_MANAGER->FindConnectedClient(id)->OnceSend(packet);
#else
	SESSION_MANAGER->FindConnectedClient(id)->PreSend((char *)&send_pkt);
#endif
}

void CPacketManager::ProcessPlayerPosition(const char * packet, const UINT id)
{
	sc_packet_player_pos *pkt = (sc_packet_player_pos *)packet;
	
	CPlayer *pPlayer = PLAYER_MANAGER->FindPlayer(pkt->player_id);
	//UCHAR& dir = pPlayer->GetDirection();
	//dir = 0;
	//pPlayer->SetPosition(D3DXVECTOR3(pkt->x, pkt->y, pkt->z));

	BYTE newDir = 0;

	if (pkt->dx == 1)  newDir |= Direction::RIGHT;
	if (pkt->dx == -1) newDir |= Direction::LEFT;
	if (pkt->dy == 1)  newDir |= Direction::UP;
	if (pkt->dy == -1) newDir |= Direction::DOWN;
	if (pkt->dz == 1)  newDir |= Direction::FORWARD;
	if (pkt->dz == -1) newDir |= Direction::BACKWARD;
	
	pPlayer->GetDeadReckoningQueue().push(D3DXVECTOR3(pkt->x, pkt->y, pkt->z));
	pPlayer->SetDirection(newDir);
	pPlayer->SetSpeed(pkt->speed);
	//pPlayer->SetPosition(pkt->x, pkt->y, pkt->z);
	//
	//if (pkt->dx == 1) dir |= Direction::RIGHT;
	//else if (pkt->dx == -1) dir |= Direction::LEFT;

	//if (pkt->dy == 1) dir |= Direction::UP;
	//else if (pkt->dy == -1) dir |= Direction::DOWN;

	//if (pkt->dz == 1) dir |= Direction::FORWARD;
	//else if (pkt->dz == -1) dir |= Direction::BACKWARD;
	//DUMMY_ENGINE->SetScrollLeft(pPlayer->GetPositionX() - pkt->x);
	//DUMMY_ENGINE->SetScrollTop(pPlayer->GetPositionY() - pkt->y);

	//cout << "ProcessPlayerPosition : " << pkt->player_id << endl;
}

void CPacketManager::ProcessPutPlayer(const char * packet, const UINT id)
{
	//TCHAR name[100];
	sc_packet_put_player *pkt = (sc_packet_put_player *)packet;
	CPlayer *pPlayer;

	if (pkt->player_id <= 0) return;

	pPlayer = PLAYER_MANAGER->AddPlayer(pkt->player_id);

	// 자기 플레이어 제외하고 
	pPlayer->SetPosition(D3DXVECTOR3(pkt->x, pkt->y, pkt->z));
	//pPlayer->SetHP(pkt->hp);
	//wsprintf(name, L"Player%d", pkt->player_id);
	//pPlayer->SetName(name);
	pPlayer->SetConnected(true);

	//cout << "ProcessOutPlayer : " << pkt->player_id << " 추가" << endl;
}

// Warning : 여기서 인자로 넣어진 ID를 쓰지말 것! 아직 ID 등록 안된 상태
void CPacketManager::ProcessLogin(const char * packet, const UINT id)
{
	//TCHAR name[100];
	sc_packet_login *pkt = (sc_packet_login *)packet;	

	if (pkt->player_id <= 0) return;

	CPlayer *pPlayer = PLAYER_MANAGER->AddPlayer(pkt->player_id);
	pPlayer->Initalize();

	pPlayer->SetID(pkt->player_id);
	NETWORK_ENGINE->SetMyPlayer(pPlayer);
	NETWORK_ENGINE->SetMyID(pkt->player_id);
	//pPlayer->SetID(pkt->player_id);  Node : AddPlayer 할때 안에서 SetID 해줌
	//pPlayer->SetConnected(true);			  AddPlayer 할때 안에서 Connect Set 해줌
	
	// 지형의 y값
	pPlayer->SetPosition(D3DXVECTOR3(pkt->x, pkt->y, pkt->z));
	pPlayer->Move(Direction::STOP);		// 임시로
	//pPlayer->SetHP(pkt->hp);
	//wsprintf(name,L"Player%d", pkt->player_id);
	//pPlayer->SetName(name);

	//cout << "ProcessLogin : " << pkt->player_id << " 로그인" << endl;
}

void CPacketManager::ProcessRemovePlayer(const char * packet, const UINT id)
{
	sc_packet_remove_player *pkt = (sc_packet_remove_player *)packet;

	PLAYER_MANAGER->DeletePlayer(pkt->player_id);
}

void CPacketManager::ProcessPlayerHP(const char * packet, const UINT id)
{
	sc_packet_player_hp *pkt = (sc_packet_player_hp *)packet;
	CPlayer *pPlayer = PLAYER_MANAGER->FindPlayer(pkt->player_id);
	//pPlayer->SetHP(pkt->hp);
}

void CPacketManager::ProcessMonsterPosition(const char * packet, const UINT id)
{
	sc_packet_monster_pos *pkt = (sc_packet_monster_pos *)packet;

	CMonster *pMon = OBJECT_MANAGER->FindMonster(pkt->monster_id);
	
	// 클라의 몬스터 pos는 서버의 패킷 pos보다 한발 느린것을 이용한다.
	D3DXVECTOR3 vDir = D3DXVECTOR3(pkt->x, HMAP->GetHeight(pkt->x, pkt->z), pkt->z) - pMon->GetPosition();

	D3DXVec3Normalize(&vDir, &vDir);

	pMon->SetDir(vDir.x, vDir.z);

	/*float fDegree = atan2f(pkt->z - vPrepos.z, pkt->x - vPrepos.x) * 180 / 3.141592f;

	float iXDist = cos(fDegree);
	float iZDist = cos(fDegree);*/

	pMon->GetDeadReckoningQueue().push(D3DXVECTOR3(pkt->x, HMAP->GetHeight(pkt->x, pkt->z), pkt->z));
	
	//pMon->SetPosition(D3DXVECTOR3(pkt->x, pkt->y, pkt->z));
}

void CPacketManager::ProcessPutMonster(const char * packet, const UINT id)
{
	sc_packet_put_monster *pkt = (sc_packet_put_monster *)packet;

	if (pkt->monster_id <= 0) return;
	
	CMonster *pMon = OBJECT_MANAGER->GetNewMonster(pkt->monster_id);
	pMon->Initalize();
	pMon->SetID(pkt->monster_id);
	pMon->SetPosition(D3DXVECTOR3(pkt->x, HMAP->GetHeight(pkt->x, pkt->z), pkt->z));
	OBJECT_MANAGER->InsertMappingData(pMon->GetID(), pMon->GetIndex());
	//pMon->SetHP(pkt->hp);	
	//pMon->SetMonsterType(pkt->monster_type);
	pMon->SetAlive(true);
}

void CPacketManager::ProcessRemoveMonster(const char * packet, const UINT id)
{
	sc_packet_remove_monster *pkt = (sc_packet_remove_monster *)packet;

	OBJECT_MANAGER->DeleteMonster(pkt->monster_id);
	//DUMMY_ENGINE->DeleteMonster(pkt->monster_id);
}
