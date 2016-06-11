#include "stdafx.h"
#include "AnimationCtrl.h"

USING(Engine)

CAnimationCtrl::CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
: m_pAniCtrl(pAniCtrl)
, m_dwCurrentTrack(0)
, m_dwNewTrack(1)
, m_fAccTime(0.f)
, m_iCurrentAniIdx(0)
, m_Period(0.f)
{

}

CAnimationCtrl::CAnimationCtrl(const CAnimationCtrl& Instance)
: m_dwCurrentTrack(Instance.m_dwCurrentTrack)
, m_dwNewTrack(Instance.m_dwNewTrack)
, m_fAccTime(Instance.m_fAccTime)
, m_iCurrentAniIdx(Instance.m_iCurrentAniIdx)
, m_Period(Instance.m_Period)
{
	Instance.m_pAniCtrl->CloneAnimationController(Instance.m_pAniCtrl->GetMaxNumAnimationOutputs()
		, m_iMaxAniSet = Instance.m_pAniCtrl->GetMaxNumAnimationSets()
		, Instance.m_pAniCtrl->GetMaxNumTracks()
		, Instance.m_pAniCtrl->GetMaxNumEvents()
		, &m_pAniCtrl);
}

CAnimationCtrl::~CAnimationCtrl(void)
{

}

CAnimationCtrl* CAnimationCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAnimationCtrl*		pAnimationCtrl = new CAnimationCtrl(pAniCtrl);

	return pAnimationCtrl;
}

CAnimationCtrl* CAnimationCtrl::Clone(const CAnimationCtrl& Instance)
{
	CAnimationCtrl*		pAnimationCtrl = new CAnimationCtrl(Instance);

	return pAnimationCtrl;
}

// 현재 메시를 특정 애니메이션 셋으로 셋팅한다.
void CAnimationCtrl::Set_AnimationSet(const _uint& iAniIdx)
{
	if(iAniIdx == m_iCurrentAniIdx)
		return;

	if(iAniIdx >= m_iMaxAniSet)
		return;	

	m_dwNewTrack = m_dwCurrentTrack == 0 ? 1 : 0;

	// 현재 애니메이션 셋을 대표하는 객체
	LPD3DXANIMATIONSET			pAnimationSet = NULL;

	m_pAniCtrl->GetAnimationSet(iAniIdx, &pAnimationSet);

	
	// 현재 애니메이션 셋을 전체 동작하는데 걸리는 시간
	m_Period = pAnimationSet->GetPeriod();	

	// 특정 트랙위에 애니메이션 셋을 올리낟.
	m_pAniCtrl->SetTrackAnimationSet(m_dwNewTrack, pAnimationSet);

	pAnimationSet->Release();

	m_pAniCtrl->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_dwNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_fAccTime + 0.25f);
	m_pAniCtrl->KeyTrackSpeed(m_dwCurrentTrack, 1.f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_dwCurrentTrack, 0.1f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);

	// 특정 트랙을 활성화시킨다.	
	m_pAniCtrl->KeyTrackEnable(m_dwNewTrack, TRUE, m_fAccTime + 0.25f);	
	m_pAniCtrl->KeyTrackSpeed(m_dwNewTrack, 1.f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_dwNewTrack, 0.9f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_dwNewTrack, TRUE);

	m_pAniCtrl->ResetTime();

	m_pAniCtrl->SetTrackPosition(m_dwNewTrack, 0.0);

	m_dwCurrentTrack = m_dwNewTrack;

	m_iCurrentAniIdx = iAniIdx;

	m_fAccTime = 0.f;	
}

void CAnimationCtrl::Move_Frame(const _float& fTimeDelta)
{
	// 애니멩션을 동작시킨다.
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL);

	m_fAccTime += fTimeDelta;	
}

_bool CAnimationCtrl::Check_EndPeriod(void)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(TrackInfo));

	m_pAniCtrl->GetTrackDesc(m_dwCurrentTrack, &TrackInfo);

	if (m_Period < TrackInfo.Position)
	{
		m_pAniCtrl->ResetTime();
		m_fAccTime = 0.f;
		return true;
	}

	return false;
}

_ulong CAnimationCtrl::Release(void)
{
	Engine::Safe_Release(m_pAniCtrl);

	delete this;

	return 0;
}

