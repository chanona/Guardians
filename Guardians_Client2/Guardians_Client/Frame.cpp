#include "stdafx.h"
#include "Frame.h"

USING(Engine)

Engine::CFrame::CFrame(void)
: m_fFpsRate(0.0f)
, m_fAccFrame(0.f)
{

}

Engine::CFrame::~CFrame(void)
{

}

_bool Engine::CFrame::Get_Activate(const _float& fTimeDelta)
{
	m_fAccFrame += fTimeDelta;

	if(m_fAccFrame > m_fFpsRate)
	{		
		m_fAccFrame = 0.f;
		return true;
	}	
	return false;
}

HRESULT Engine::CFrame::Init_Frame(const _float& fFps)
{
	m_fFpsRate = 1.f / fFps;

	return S_OK;
}

CFrame* Engine::CFrame::Create(const _float& fFps)
{
	CFrame*		pFrame = new CFrame;

	if(FAILED(pFrame->Init_Frame(fFps)))
	{
		MSG_BOX("CFrame Create Failed");
		Engine::Safe_Release(pFrame);
	}	
	return pFrame;
}

_ulong Engine::CFrame::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

