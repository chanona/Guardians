#ifndef Timer_h__
#define Timer_h__

#include "Base.h"

BEGIN(Engine)

class  CTimer : public Engine::CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);
public:
	_float Get_TimeDelta(void) {
		return m_fTimeDelta;}
public:
	void Set_TimeDelta(void);
public:
	HRESULT Init_Timer(void);
private:
	LARGE_INTEGER				m_FrameTime;
	LARGE_INTEGER				m_FixTime;
	LARGE_INTEGER				m_LastTime;
	LARGE_INTEGER				m_CpuTick;
private:
	_float						m_fTimeDelta;

public:
	static CTimer* Create(void);
public:
	virtual _ulong Release(void);
};


END

#endif // Timer_h__
