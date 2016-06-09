#include "stdafx.h"
#include "VIBuffer.h"

USING(Engine)

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
: CResources(pGraphicDev)
{

}

Engine::CVIBuffer::~CVIBuffer(void)
{

}

void Engine::CVIBuffer::Get_VtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, &pOriVtx, 0);
	memcpy(pVtxInfo, pOriVtx, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::Set_VtxInfo(void* pVtxInfo)
{
	void* pOriVtx = NULL;

	m_pVB->Lock(0, 0, &pOriVtx, 0);
	memcpy(pOriVtx, pVtxInfo, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::Set_IdxInfo(void* pInxInfo, const DWORD* pTriCnt)
{
	void*	pOriIndex = NULL;

	m_dwTriCnt = *pTriCnt;

	m_pIB->Lock(0, 0, &pOriIndex, 0);
	memcpy(pOriIndex, pInxInfo, m_dwIdxSize * m_dwTriCnt);
	m_pIB->Unlock();
}

HRESULT Engine::CVIBuffer::Create_Buffer(void)
{
	HRESULT		hr = NULL;
	hr = m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"VertexBuffer Create Failed");

	hr = m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"IndexBuffer Create Failed");
	return S_OK;
}

void Engine::CVIBuffer::Render(const D3DXMATRIX* pmatWorld)
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

_ulong Engine::CVIBuffer::Release(void)
{
	_ulong dwRefCnt = Engine::CResources::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{
			Engine::Safe_Release(m_pVB);
			Engine::Safe_Release(m_pIB);				
		}
		
	}	

	return dwRefCnt;
}