#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CTimer;
// 시간들을 관리한다.
class  CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)
private:
	explicit CTimeMgr(void);
	~CTimeMgr(void);
public:
	_float Get_TimeDelta(const TCHAR*);
public:
	void Set_TimeDelta(const TCHAR*);
public:
	HRESULT Ready_Timer(const TCHAR*);
	
private:	
	map<const TCHAR*, CTimer*>			m_mapTimer;
	typedef map<const TCHAR*, CTimer*>	MAPTIMER;
private:
	CTimer* Find_Timer(const TCHAR*);
public:
	void Release(void);
};


END

#endif // TimeMgr_h__
