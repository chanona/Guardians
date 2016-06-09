#include "stdafx.h"
#include "CubeTex.h"

USING(Engine)

Engine::CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CCubeTex::~CCubeTex(void)
{
	
}

Engine::CResources* Engine::CCubeTex::Clone_Resource(void)
{
	CResources*		pResource = new CCubeTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

HRESULT Engine::CCubeTex::Create_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_CUBE;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 12;

	FAILED_CHECK(CVIBuffer::Create_Buffer());

	VTXCUBE*		pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxCube[0].vTexUV = pVtxCube[0].vPos;

	pVtxCube[1].vPos = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxCube[1].vTexUV = pVtxCube[1].vPos;

	pVtxCube[2].vPos = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxCube[2].vTexUV = pVtxCube[2].vPos;

	pVtxCube[3].vPos = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxCube[3].vTexUV = pVtxCube[3].vPos;


	pVtxCube[4].vPos = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxCube[4].vTexUV = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxCube[5].vTexUV = pVtxCube[5].vPos;

	pVtxCube[6].vPos = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxCube[6].vTexUV = pVtxCube[6].vPos;

	pVtxCube[7].vPos = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxCube[7].vTexUV = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	//+x
	pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;

	//-x										   
	pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;

	//+y										   
	pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;

	//-y										   
	pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;

	//+z										   
	pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;

	//-z
	pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CCubeTex* Engine::CCubeTex::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeTex*		pCube = new CCubeTex(pDevice);
	if(FAILED(pCube->Create_Buffer()))
		Engine::Safe_Release(pCube);

	return pCube;
}

_ulong Engine::CCubeTex::Release(void)
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
