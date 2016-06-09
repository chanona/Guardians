#include "stdafx.h"
#include "RcTex.h"

USING(Engine)

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTex::~CRcTex(void)
{

}

HRESULT Engine::CRcTex::Create_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 2;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::Create_Buffer()))
		return E_FAIL;

	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	pVtxTex[0].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVtxTex[1].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.f);
	pVtxTex[1].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVtxTex[2].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.f);
	pVtxTex[2].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVtxTex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);
	pVtxTex[3].vNormal = D3DXVECTOR3(0.f, 0.f, -1.f);
	pVtxTex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CResources* Engine::CRcTex::Clone_Resource(void)
{
	CResources* pResource = new CRcTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*		pRcTex = new CRcTex(pGraphicDev);

	if(FAILED(pRcTex->Create_Buffer()))	
		Engine::Safe_Release(pRcTex);

	return pRcTex;
}


_ulong Engine::CRcTex::Release(void)
{
	_ulong dwRefCnt = Engine::CVIBuffer::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{

		}
		delete this;
	}

	return dwRefCnt;
}