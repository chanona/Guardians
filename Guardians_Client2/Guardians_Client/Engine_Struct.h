/*!
 * \file Engine_Struct.h
 * \date 2015/04/04 23:03
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagFrame_Derived : public _D3DXFRAME
	{
		// 나의 TransformationMatrix에 부모의 TransformationMatrix을 곱한 행렬이다.
		D3DXMATRIX				CombinedTransformationMatrix;
	}D3DXFRAME_DERIVED;

	typedef struct tagMeshContainer_Derived : public _D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTextures;	
		LPD3DXMESH					pOriMesh;
		DWORD						dwNumBones;
		D3DXMATRIX*					pOffsetFrameMatrices;
		D3DXMATRIX**				ppCombinedTransformationMatrices;
		D3DXMATRIX*					pRenderingMatrices;
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3				vPos;				
		D3DXVECTOR3				vNormal;
		D3DXVECTOR3				vTexUV;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3				vPos;				
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTexUV;
	}VTXTEX;
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;
}

#endif // Engine_Struct_h__