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

// ���� �޽ø� Ư�� �ִϸ��̼� ������ �����Ѵ�.
void CAnimationCtrl::Set_AnimationSet(const _uint& iAniIdx)
{
	if(iAniIdx == m_iCurrentAniIdx)
		return;

	if(iAniIdx >= m_iMaxAniSet)
		return;	

	m_dwNewTrack = m_dwCurrentTrack == 0 ? 1 : 0;

	// ���� �ִϸ��̼� ���� ��ǥ�ϴ� ��ü
	LPD3DXANIMATIONSET			pAnimationSet = NULL;

	m_pAniCtrl->GetAnimationSet(iAniIdx, &pAnimationSet);

	
	// ���� �ִϸ��̼� ���� ��ü �����ϴµ� �ɸ��� �ð�
	m_Period = pAnimationSet->GetPeriod();	

	// Ư�� Ʈ������ �ִϸ��̼� ���� �ø���.
	m_pAniCtrl->SetTrackAnimationSet(m_dwNewTrack, pAnimationSet);

	pAnimationSet->Release();

	m_pAniCtrl->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_dwNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_fAccTime + 0.25f);
	m_pAniCtrl->KeyTrackSpeed(m_dwCurrentTrack, 1.f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_dwCurrentTrack, 0.1f, m_fAccTime, 0.25f, D3DXTRANSITION_LINEAR);

	// Ư�� Ʈ���� Ȱ��ȭ��Ų��.	
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
	// �ִϸ���� ���۽�Ų��.
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

