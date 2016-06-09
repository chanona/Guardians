#include "stdafx.h"
#include "TimeMgr.h"
#include "Timer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CTimeMgr)

Engine::CTimeMgr::CTimeMgr(void)
{

}

Engine::CTimeMgr::~CTimeMgr(void)
{

}

_float CTimeMgr::Get_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void CTimeMgr::Set_TimeDelta(const TCHAR* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if(NULL == pTimer)
		return;

	pTimer->Set_TimeDelta();
}

HRESULT CTimeMgr::Ready_Timer(const TCHAR* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if(NULL != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();

	if(NULL == pTimer)
		return E_FAIL;

	m_mapTimer.insert(MAPTIMER::value_type(pTimerTag, pTimer));

	return S_OK;	
}

CTimer* CTimeMgr::Find_Timer(const TCHAR* pTimerTag)
{
	MAPTIMER::iterator	iter = find_if(m_mapTimer.begin(), m_mapTimer.end(), CTagFinder(pTimerTag));

	if(iter == m_mapTimer.end())
		return NULL;
	
	return iter->second;
}

void CTimeMgr::Release(void)
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CRelease_Pair());
	m_mapTimer.clear();

	delete this;
}

