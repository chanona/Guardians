#include "stdafx.h"
#include "PacketManager.h"
#include "SessionManager.h"
#include "WorldManager.h"


CPacketManager::CPacketManager()
{
}


CPacketManager::~CPacketManager()
{
}

bool CPacketManager::Start()
{
	m_packetProcessFuncTable.reserve(CSPacketType::CSPACKET_TYPE_END);

	m_packetProcessFuncTable[CSPacketType::CS_CHAT] = std::bind(&CPacketManager::ProcessChat,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

	m_packetProcessFuncTable[CSPacketType::CS_DOWN] = std::bind(&CPacketManager::ProcessMoveDown,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

	m_packetProcessFuncTable[CSPacketType::CS_LEFT] = std::bind(&CPacketManager::ProcessMoveLeft,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

	m_packetProcessFuncTable[CSPacketType::CS_RIGHT] = std::bind(&CPacketManager::ProcessMoveRight,
		this,
		std::placeholders::_1,
		std::placeholders::_2);

	m_packetProcessFuncTable[CSPacketType::CS_UP] = std::bind(&CPacketManager::ProcessMoveUp,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_packetProcessFuncTable[CSPacketType::CS_FORWARD] = std::bind(&CPacketManager::ProcessMoveForward,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_packetProcessFuncTable[CSPacketType::CS_BACKWARD] = std::bind(&CPacketManager::ProcessMoveBackward,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_packetProcessFuncTable[CSPacketType::CS_KEYBOARD_MOVE_START] = std::bind(&CPacketManager::ProcessKeyboardMoveStart,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	m_packetProcessFuncTable[CSPacketType::CS_KEYBOARD_MOVE_STOP] = std::bind(&CPacketManager::ProcessKeyboardMoveStop,
		this,
		std::placeholders::_1,
		std::placeholders::_2);
	return true;
}

void CPacketManager::ShutDown()
{
#if defined(SERVER)
	for (UINT i = 0; i < CSPacketType::CSPACKET_TYPE_END; ++i)
	{
		m_packetProcessFuncTable.erase(i);
	}
#else
	for (UINT i = 0; i < SCPacketType::CSPACKET_TYPE_END; ++i)
	{
		m_packetProcessFuncTable.erase(i);
	}
#endif
	m_packetProcessFuncTable.clear();
}

void CPacketManager::Assemble(UINT size_to_process, CClientSession *pClient)
{
	static UINT prev_data_size{ 0 };
	CRingBuffer *recv_ring_buffer = pClient->GetRecvRingBuffer();

	UINT size_to_build_packet{ 0 };
	UINT id{ pClient->GetID() };

	recv_ring_buffer->Commit_Enqueue(size_to_process);

	// Note : Recv 요청을 한번만 하기 때문에 락은 따로 필요없음
	while (size_to_process > 0)
	{
		char        *recv_buffer = recv_ring_buffer->GetDequeuePosttion();
		size_to_build_packet = recv_buffer[0] - prev_data_size;

		if (size_to_process >= size_to_build_packet)	// 패킷 처리 가능
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
				prev_data_size = size_to_process;
			}
			else
			{
				prev_data_size += size_to_process;		// 한번 잘려서 데이터를 맨 앞으로 보냈는데 또 한번 패킷을 완성시키지 못했을 경우
			}
			break;	// size_to_process = 0 대신에 break; -> 안쓰면 무한루프돈다.	
		}
	}
}

void CPacketManager::Assemble(UINT size_to_process, CRingBuffer* recv_ring_buffer, UINT id)
{
	UINT size_to_build_packet{ 0 };

	recv_ring_buffer->Commit_Enqueue(size_to_process);

	// Note : Recv 요청을 한번만 하기 때문에 락은 따로 필요없음
	while (size_to_process > 0)
	{
		char        *recv_buffer = recv_ring_buffer->GetDequeuePosttion();
		size_to_build_packet = recv_buffer[0];

		if (size_to_process >= size_to_build_packet)	// 패킷 처리 가능
		{
			Dispatch(recv_buffer, id);
			size_to_process -= size_to_build_packet;
			recv_ring_buffer->Commit_Dequeue(size_to_build_packet);
		}
		else
		{
			memcpy(recv_ring_buffer->GetBufStartPosition(), recv_buffer, size_to_process);	// 잘린패킷의 데이터를
																							// recv버퍼의 시작점으로 복사시킴
			recv_ring_buffer->Clear();
			recv_ring_buffer->Commit_Enqueue(size_to_process);
			break;
		}
	}
}

void CPacketManager::Dispatch(char * packet, UINT id)
{
	UINT type = packet[1];

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
	send_pkt.chat.size = chat_packet->size;
	send_pkt.chat.type = SCPacketType::SC_CHAT;
	send_pkt.chat.player_id = id;
	memcpy(send_pkt.chat.string, chat_packet->string, strlen(chat_packet->string));	// Warning : 클라이언트에서 받을때 맨 마지막 문자열 뒤에 '\0'삽입시켜야함

#if defined(ONCE_SEND)
	SESSION_MANAGER->FindSession(id)->OnceSend(packet);
#else
	SESSION_MANAGER->FindConnectedClient(id)->PreSend((char *)&send_pkt);
#endif
}

void CPacketManager::ProcessMoveLeft(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float x = pPlayer->GetPositionX();

	x -= 10;

	if (x < 0) x = 10;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = x;
	pos_packet.y = pPlayer->GetPositionY();
	pos_packet.z = pPlayer->GetPositionZ();

	pPlayer->SetPositionX(x);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));

#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif
}

void CPacketManager::ProcessMoveRight(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float x = pPlayer->GetPositionX();

	x += 10;

	if (x >= WORLD_WIDTH) x = WORLD_WIDTH - 10;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = x;
	pos_packet.y = pPlayer->GetPositionY();
	pos_packet.z = pPlayer->GetPositionZ();

	pPlayer->SetPositionX(x);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));

#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif

}

void CPacketManager::ProcessMoveUp(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float y = pPlayer->GetPositionY();

	y -= 10;

	if (y < 0) y = 0;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = pPlayer->GetPositionX();
	pos_packet.y = y;
	pos_packet.z = pPlayer->GetPositionZ();

	pPlayer->SetPositionY(y);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));
#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif
}

void CPacketManager::ProcessMoveDown(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float y = pPlayer->GetPositionY();

	y += 10;

	if (y + 20 >= WORLD_HEIGHT) y = WORLD_HEIGHT - 20;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = pPlayer->GetPositionX();
	pos_packet.y = y;
	pos_packet.z = pPlayer->GetPositionZ();

	pPlayer->SetPositionY(y);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));
#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif
}

void CPacketManager::ProcessMoveForward(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float z = pPlayer->GetPositionZ();

	z += 10;

	if (z >= WORLD_HEIGHT) z = WORLD_HEIGHT - 10;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = pPlayer->GetPositionX();
	pos_packet.y = pPlayer->GetPositionY();
	pos_packet.z = z;

	pPlayer->SetPositionZ(z);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));
#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif
}

void CPacketManager::ProcessMoveBackward(const char * packet, const UINT id)
{
	cs_packet_move_left *pkt = (cs_packet_move_left *)packet;
	CClientSession *pSession = SESSION_MANAGER->FindSession(id);
	CPlayer *pPlayer = pSession->GetPlayer();

	float z = pPlayer->GetPositionZ();

	z -= 10;

	if (z < 0) z = 0;

	sc_packet_player_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SCPacketType::SC_PLAYER_POS;
	pos_packet.player_id = id;
	pos_packet.x = pPlayer->GetPositionX();
	pos_packet.y = pPlayer->GetPositionY();
	pos_packet.z = z;

	pPlayer->SetPositionZ(z);

	pSession->OnceSend(reinterpret_cast<char *>(&pos_packet));
#ifdef VIEW_PROCCESS
	WORLD_MANAGER->ViewProcess(pSession);
	SESSION_MANAGER->BroadCastInView(reinterpret_cast<char *>(&pos_packet), id);
#else
	SESSION_MANAGER->BroadCast(reinterpret_cast<char *>(&pos_packet), id);
#endif
}

void CPacketManager::ProcessKeyboardMoveStart(const char * packet, const UINT id)
{
	cs_packet_player_keyboard_move_start *recived_pkt = (cs_packet_player_keyboard_move_start *)packet;
	sc_packet_player_pos pos_pkt;
	pos_pkt.size = sizeof(pos_pkt);
	pos_pkt.type = SCPacketType::SC_PLAYER_POS;
	pos_pkt.player_id = id;

	CClientSession *pClient = SESSION_MANAGER->FindSession(id);
	CPlayer        *pPlayer = pClient->GetPlayer();

	if (pClient == nullptr) return;

	float new_x = recived_pkt->x;
	float new_y = recived_pkt->y;
	float new_z = recived_pkt->z;
	float new_dx = 0.0f;
	float new_dy = 0.0f;
	float new_dz = 0.0f;

	float new_speed = recived_pkt->speed;

	if (recived_pkt->direction & Direction::LEFT)     new_dx = -1;
	if (recived_pkt->direction & Direction::RIGHT)	  new_dx = 1;
	if (recived_pkt->direction & Direction::UP)		  new_dy = -1;
	if (recived_pkt->direction & Direction::DOWN)	  new_dy = 1;
	if (recived_pkt->direction & Direction::FORWARD)  new_dz = 1;
	if (recived_pkt->direction & Direction::BACKWARD) new_dz = -1;

	if (new_dz == 1)
	{
		cout << endl;
	}

	new_x += (GetTickCount() - recived_pkt->time_stamp) * new_dx * new_speed;
	new_y += (GetTickCount() - recived_pkt->time_stamp) * new_dy * new_speed;
	new_z += (GetTickCount() - recived_pkt->time_stamp) * new_dz * new_speed;

	pPlayer->SetPosition(new_x, new_y, new_z);
	pPlayer->SetDirection(new_dx, new_dy, new_dz);
	pPlayer->SetSpeed(new_speed);

	pos_pkt.x = new_x;
	pos_pkt.y = new_y;
	pos_pkt.z = new_z;
	pos_pkt.dx = new_dx;
	pos_pkt.dy = new_dy;
	pos_pkt.dz = new_dz;

	//cout << new_x << " " << new_y << " " << new_z << endl;

	WORLD_MANAGER->ViewProcess(pClient);

	SESSION_MANAGER->BroadCastInView((char *)&pos_pkt, id);
}

void CPacketManager::ProcessKeyboardMoveStop(const char * packet, const UINT id)
{
	// 할 필요가 없을듯...?
}
