#ifndef Frame_h__
#define Frame_h__

#include "Base.h"

BEGIN(Engine)

class  CFrame : public Engine::CBase
{
private:
	explicit CFrame(void);
	virtual ~CFrame(void);	
public: // Getter
	_bool Get_Activate(const _float& fTimeDelta);

public:
	HRESULT Init_Frame(const _float& fFps);
public:
	static CFrame* Create(const _float& fFps);
private:
	_float				m_fFpsRate;
	_float				m_fAccFrame;
public:
	virtual _ulong Release(void);
};


END

#endif // Frame_h__
