#pragma once

#include "protocol.h"
#include "RingBuffer.h"
#include "SendMemoryPool.h"
#include "PlayerManager.h"

#define SERVER

#define NETWORK_ENGINE CClientNetEngine::GetInstance()

class CPlayer;
class CClientNetEngine
{
public:
	CClientNetEngine();
	~CClientNetEngine();

	static CClientNetEngine* GetInstance()
	{
		static CClientNetEngine instance;
		return &instance;
	}

	bool Start(const HWND hWnd);
	void ShutDown();

	void CreateSocket();
	void Connect(const char *ip, const USHORT port);	

	void RegisterAsyncSelect(const HWND hWnd);

	void SendPacket(char *packet);
	void RecvPacket();

	void IsNagle(SOCKET sock, bool flag);
private:
	SOCKET  m_socket;
	UINT     m_id;
	UINT	 m_index;
	WSABUF  m_wsaSendBuf;
	WSABUF  m_wsaRecvBuf;
	CPlayer *m_myPlayer;

	CSendMemoryPool *m_pSendMemoryPool;
	CRingBuffer     *m_pRecvRingBuffer;
public:
	// Setter, Getter
	void SetMyPlayer( CPlayer *player) { m_myPlayer = player; }
	void         SetMyID(const UINT id) { m_id = id; }
	void		SetMyIndex(const UINT index) { m_index = index; }

	CPlayer* GetMyPlayer() { return m_myPlayer; }
	UINT		GetMyIndex() const { return m_index; }
	SOCKET		 GetSocket()   const { return m_socket; }
	UINT			 GetID()	   const { return m_id; }
	//CRingBuffer* GetSendRingBuffer() { return m_pSendRingBuffer; }
	CRingBuffer*     GetRecvRingBuffer() const { return m_pRecvRingBuffer; }
	CSendMemoryPool* GetSendMemoryPool() const { return m_pSendMemoryPool; }
};

// 소켓 함수 오류 출력 후 종료
void err_quit(TCHAR *msg);

// 소켓 함수 오류 출력
void err_display(TCHAR *msg);