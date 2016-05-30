#include "stdafx.h"
#include "TimerThread.h"
#include "Memory.h"
#include "SessionManager.h"
#include "ObjectManager.h"
#include "type.h"
#include "NetEngine.h"

CTimerThread::CTimerThread()
{
}


CTimerThread::~CTimerThread()
{
	
}

void CTimerThread::WaitThreads()
{
	m_pTimerThread->join();
}

bool CTimerThread::Create()
{
	// Register Event Process Functions
	m_eventProcessFuncTable.reserve(EventType::EVENT_END);
	
	m_eventProcessFuncTable[EventType::NPC_MOVE] = bind(&CTimerThread::MoveNpc, this, std::placeholders::_1);

	m_pTimerThread = new thread{ CTimerThread::EntryPoint, this };
	m_pTimerQueue = new TimerQueue();
	
	return true;
}

void CTimerThread::Release()
{
	while (!m_pTimerQueue->empty())
	{
		CEventMessage *pEvt;
		m_pTimerQueue->try_pop(pEvt);
		delete pEvt;
	}
	Memory::SAFE_DELETE(m_pTimerQueue);
	Memory::SAFE_DELETE(m_pTimerThread);
}

void CTimerThread::AddEvent(CEventMessage * evt_msg)
{
	m_pTimerQueue->push(evt_msg);
}

void CTimerThread::DelEvent(CEventMessage *evt_msg)
{
	Memory::SAFE_DELETE(evt_msg);
}


void CTimerThread::Dispatch(CEventMessage * evt_msg)
{
	assert(evt_msg->m_eventType >= 0);
	assert(evt_msg->m_eventType < EventType::EVENT_END);

	m_eventProcessFuncTable[evt_msg->m_eventType](evt_msg);
}

void CTimerThread::MoveNpc(CEventMessage * evt_msg)
{
	MoveOverlappedEx *move_overlapped = new MoveOverlappedEx();
	PostQueuedCompletionStatus(NETWORK_ENGINE->GetIOCPHandle(), 1, evt_msg->m_sourceID, &move_overlapped->overlapped);
}

UINT CTimerThread::Run()
{
	CEventMessage *pNewEvent = nullptr;

	while (1)
	{
		Sleep(1);
		
		if (!pNewEvent && !m_pTimerQueue->try_pop(pNewEvent)) continue;

		while (pNewEvent->m_wakeupTime <= GetTickCount()) // ToDo : GetTickCount 대신에 시간 가져오는 클래스 나중에 따로 만들 것!
		{
			Dispatch(pNewEvent);
			DelEvent(pNewEvent);

			if (!m_pTimerQueue->try_pop(pNewEvent)) break;
		}
	}
	return 0;
}

void CTimerThread::EntryPoint(void * arg)
{
	CTimerThread *tt = (CTimerThread *)arg;

	tt->Run();
}

CEventMessage::CEventMessage(const UINT src_id, const UINT target_id, const UINT wakeup_time, const BYTE event_type, const BYTE src_type, const BYTE target_type)
{
	m_sourceID   = src_id;
	m_targetID   = target_id;
	m_wakeupTime = wakeup_time;
	m_eventType  = event_type;
	m_srcType    = src_type;
	m_targetType = target_type;
}
