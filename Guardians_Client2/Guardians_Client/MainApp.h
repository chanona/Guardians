#ifndef MainApp_h__
#define MainApp_h__

#include "Defines.h"
#include "Base.h"

class CMainApp : Engine::CBase
{
private:
	explicit CMainApp(void);
	virtual ~CMainApp(void);
public:
	HRESULT Initialize(void);
	_int Update(const _float& fTimeDelta);
	void Render(void);
public:
	static CMainApp* Create(void);
public:
	virtual _ulong Release(void);
};


#endif // MainApp_h__
