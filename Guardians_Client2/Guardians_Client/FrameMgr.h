#ifndef FrameMgr_h__
#define FrameMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFrame;
// 시간들을 관리한다.
class  CFrameMgr
{
	DECLARE_SINGLETON(CFrameMgr)
private:
	explicit CFrameMgr(void);
	~CFrameMgr(void);
public: // Getter
	_bool Get_Activate(const TCHAR* pFrameTag, const _float& fTimeDelta);
public:
	HRESULT Ready_Frame(const TCHAR* pFrameTag, const _float& fFps);
private:	
	map<const TCHAR*, CFrame*>			m_mapFrame;
	typedef map<const TCHAR*, CFrame*>	MAPFRAME;
private:
	CFrame* Find_Frame(const TCHAR*);
public:
	void Release(void);
};


END

#endif // FrameMgr_h__
