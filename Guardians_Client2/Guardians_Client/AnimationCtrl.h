#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class  CAnimationCtrl
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& Instance);
	~CAnimationCtrl(void);
public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl* Clone(const CAnimationCtrl& Instance);
public:
	void Set_AnimationSet(const _uint& iAniIdx);
	const _uint& Get_AnimationSet(void) {
		return m_iCurrentAniIdx;
	}
	void Move_Frame(const _float& fTimeDelta); // �ִϸ��̼��� ���۽�Ų��.
	_bool Check_EndPeriod(void);
private:	
	// �ִϸ��̼��� �����ϱ����� ��ü
	// �޽��� �ִϸ��̼������� ��ǥ�Ѵ�.
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;
private:
	_uint								m_iMaxAniSet;
	_uint								m_iCurrentAniIdx;
	_ulong								m_dwCurrentTrack;
	_ulong								m_dwNewTrack;
	_float								m_fAccTime;
	_double								m_Period;
public:
	_ulong Release(void);
};

END

#endif // AnimationCtrl_h__
