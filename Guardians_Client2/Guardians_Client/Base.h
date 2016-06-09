#ifndef Base_h__
#define Base_h__

#include "Engine_Defines.h"

// ���� ���ӳ��� �����ϴ� ���� ��κ� Ŭ�������� �θ� �� Ŭ����.
BEGIN(Engine)

class  CBase
{
protected:
	explicit CBase(void);
	virtual ~CBase(void);
public:
	void Add_Ref(void);
protected:
	_ulong			m_dwRefCnt; // ���۷���ī����(��������)
public:
	virtual _ulong Release(void);
};

END

#endif // Base_h__
