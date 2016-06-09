#ifndef Base_h__
#define Base_h__

#include "Engine_Defines.h"

// 내가 게임내에 구현하는 거의 대부분 클래스들의 부모가 될 클래스.
BEGIN(Engine)

class  CBase
{
protected:
	explicit CBase(void);
	virtual ~CBase(void);
public:
	void Add_Ref(void);
protected:
	_ulong			m_dwRefCnt; // 레퍼런스카운팅(참조숫자)
public:
	virtual _ulong Release(void);
};

END

#endif // Base_h__
