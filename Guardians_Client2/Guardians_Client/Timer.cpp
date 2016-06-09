#include "stdafx.h"
#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer(void)
{

}

Engine::CTimer::~CTimer(void)
{

}

HRESULT CTimer::Init_Timer(void)
{
	//  cpu�� ������� ī������ ����
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceCounter(&m_FixTime);

	// cpu�� 1�ʴ� ������ų �� �ִ� ����
	QueryPerformanceFrequency(&m_CpuTick);

	return S_OK;
}

void CTimer::Set_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_FixTime.QuadPart > m_CpuTick.QuadPart)
	{
		// cpu�� 1�ʴ� ������ų �� �ִ� ����
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}

	m_fTimeDelta = _float(m_FrameTime.QuadPart - m_LastTime.QuadPart) / m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
}

CTimer* CTimer::Create(void)
{
	CTimer*		pTimer = new CTimer;

	if(FAILED(pTimer->Init_Timer()))
	{
		MSG_BOX("CTimer Create Failed");
		Engine::Safe_Release(pTimer);	
	}

	return pTimer;
}

_ulong Engine::CTimer::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

