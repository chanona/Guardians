#include "stdafx.h"
#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
: CVIBuffer(pGraphicDev)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

Engine::CResources* Engine::CTerrainTex::Clone_Resource(void)
{
	++(*m_pwRefCnt);

	return new CTerrainTex(*this);
}

HRESULT Engine::CTerrainTex::Create_Buffer(const _ushort& wCntX, const _ushort& wCntZ, const _ushort& wItv)
{
	HANDLE		hFile;
	_ulong		dwByte;

	hFile = CreateFile(L"../Resource/Texture/StageScene/Terrain/Height.bmp", GENERIC_READ, 0
		, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	BITMAPFILEHEADER		fh;
	BITMAPINFOHEADER		ih;

	ReadFile(hFile, &fh, sizeof(fh), &dwByte, NULL);
	ReadFile(hFile, &ih, sizeof(ih), &dwByte, NULL);

	_ulong*	pdwPixel = new _ulong[ih.biWidth * ih.biHeight];

	ReadFile(hFile, pdwPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, NULL);
	CloseHandle(hFile);
	
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = wCntX * wCntZ ;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;

	FAILED_CHECK(CVIBuffer::Create_Buffer());

	VTXTEX*		pVtxTex = NULL;
	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	int		iIndex = NULL;
	for(int z = 0; z < wCntZ; ++z)
	{
		for(int x = 0; x < wCntX; ++x)
		{
			iIndex = z * wCntX + x;

			pVtxTex[iIndex].vPos = D3DXVECTOR3(
				float(x) * wItv , 
				(pdwPixel[iIndex] & 0x000000ff) / 5.f , 
				float(z) * wItv);
			pVtxTex[iIndex].vNormal = D3DXVECTOR3(0.f, 0.f, 0.f);
			pVtxTex[iIndex].vTexUV = D3DXVECTOR2( x / (wCntX - 1.f), z / (wCntZ - 1.f) );
		}
	}

	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	for(int z = 0; z < wCntZ - 1; ++z)
	{
		for(int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;
	
			pIndex[iTriCnt]._1 = iIndex + wCntX;			//1-2
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;		// \|
			pIndex[iTriCnt]._3 = iIndex + 1;				//	3

			D3DXVECTOR3		vDest, vSour, vNormal;
			vDest = pVtxTex[ pIndex[iTriCnt]._2 ].vPos - pVtxTex[ pIndex[iTriCnt]._1 ].vPos;
			vSour = pVtxTex[ pIndex[iTriCnt]._3 ].vPos - pVtxTex[ pIndex[iTriCnt]._2 ].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[ pIndex[iTriCnt]._1 ].vNormal += vNormal;
			pVtxTex[ pIndex[iTriCnt]._2 ].vNormal += vNormal;
			pVtxTex[ pIndex[iTriCnt]._3 ].vNormal += vNormal;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;

			vDest = pVtxTex[ pIndex[iTriCnt]._2 ].vPos - pVtxTex[ pIndex[iTriCnt]._1 ].vPos;
			vSour = pVtxTex[ pIndex[iTriCnt]._3 ].vPos - pVtxTex[ pIndex[iTriCnt]._2 ].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[ pIndex[iTriCnt]._1 ].vNormal += vNormal;
			pVtxTex[ pIndex[iTriCnt]._2 ].vNormal += vNormal;
			pVtxTex[ pIndex[iTriCnt]._3 ].vNormal += vNormal;
			++iTriCnt;
		}
	}

	for(_ushort i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	Engine::Safe_Delete_Array(pdwPixel);
	return S_OK;
}


Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice
												 , const _ushort& wCntX, const _ushort& wCntZ, const _ushort& wItv)
{
	CTerrainTex*	pBuffer = new CTerrainTex(pDevice);
	if(FAILED(pBuffer->Create_Buffer(wCntX, wCntZ, wItv)))
		Engine::Safe_Release(pBuffer);

	return pBuffer;
}

_ulong Engine::CTerrainTex::Release(void)
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