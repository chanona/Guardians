#pragma once

#include "MemoryPool.h"
#include "SRWLock.h"
#include <unordered_map>

class CEventMessage : public CMemoryPool<CEventMessage>
{
public :
	BYTE m_eventType;
	UINT m_sourceID;
	UINT m_targetID;
	UINT m_wakeupTime;
	BYTE m_srcType;
	BYTE m_targetType;

	CEventMessage() {};
	CEventMessage(const UINT src_id, const UINT target_id, const UINT wakeup_time, const BYTE event_type, const BYTE src_type, const BYTE target_type);

	bool operator() (const CEventMessage *lhs, const CEventMessage *rhs) const
	{
		return lhs->m_wakeupTime > rhs->m_wakeupTime;
	}

};

class CTimerThread
{
public:
	CTimerThread();
	~CTimerThread();

	typedef tbb::concurrent_priority_queue<CEventMessage *, CEventMessage> TimerQueue;
	typedef function<void(CEventMessage *)> EventProcessFunc;

	void WaitThreads();
	bool Create(); 
	void Release();
	UINT Run();
	static void EntryPoint(void* arg);

	void AddEvent(CEventMessage *evt_msg);
	void DelEvent(CEventMessage *evt_msg);
	void Dispatch(CEventMessage *evt_msg);

	// Event Functions

	void MoveNpc(CEventMessage *evt_msg);
	//void IncreaseHPEvent(CEventMessage *evt_msg);
	//void DecreaseHPEvent(CEventMessage *evt_msg);
private :
	thread  *m_pTimerThread;
	TimerQueue *m_pTimerQueue;

	unordered_map<int /* Event Type */, EventProcessFunc> m_eventProcessFuncTable;

	CSRWLock m_tqLock;
};

