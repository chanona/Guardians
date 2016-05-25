#pragma once

const ULONG MAX_SAMPLE_COUNT = 50;	// 50회의 프레임 처리시간을 누적하여 평균한다.

class CGameTimer
{
private:
	bool m_bHardwareHasPerformanceCounter;	    // 컴퓨터가 Performance Counter를 가지고 있는 가
	float m_fTimeScale;							// Scale Counter의 양
	float m_fTimeElapsed;						// 마지막 프레임 이후 지나간 시간
	__int64 m_nCurrentTime;						// 현재의 시간
	__int64 m_nLastTime;						// 마지막 프레임의 시간
	__int64 m_PerformanceFrequency;				// 컴퓨터의 PerformanceFrequency											
	float m_fFrameTime[MAX_SAMPLE_COUNT];		// 프레임 시간을 누적하기 위한 배열
	ULONG m_nSampleCount;						// 누적된 프레임 횟수
	unsigned long m_nCurrentFrameRate;			// 현재의 프레임 레이트
	unsigned long m_FramePerSecond;				// 초당 프레임 수 
	float m_fFPSTimeElapsed;					// 프레임 레이트 계산 소요시간.
public:
	CGameTimer();
	virtual ~CGameTimer();
	void Tick(float fLockFPS = 0.0f);											// 타이머의 시간을 갱신한다.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);	// 프레임 레이트를 반환한다.
	float GetTimeElapsed();														// 프레임의 평균 경과 시간을 반환한다.
};

