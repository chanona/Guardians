#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.
vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;

vector				g_vTerrainDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vTerrainAmbient;

texture				g_FloorTexture;

vector				g_vTargetPos;	// In WorldSpace
float				g_fTargetRange = 1.f;

texture				g_TileTexture;
texture				g_TileTexture2;
texture				g_TileTexture3;
texture				g_TileTexture4;
texture				g_ColorHeightTexture;

//sampler FloorSampler = sampler_state
//{
//	texture = g_FloorTexture;
//	minfilter = linear;
//	magfilter = linear;
//	mipfilter = linear;
//};
//
//sampler TileSampler = sampler_state
//{
//	texture = g_TileTexture;
//	minfilter = linear;
//	magfilter = linear;
//	mipfilter = linear;
//};

sampler TileSampler = sampler_state
{
	texture = g_TileTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler TileSampler2 = sampler_state
{
	texture = g_TileTexture2;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler TileSampler3 = sampler_state
{
	texture = g_TileTexture3;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler TileSampler4 = sampler_state
{
	texture = g_TileTexture4;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

sampler ColorHeightSampler = sampler_state
{
	texture = g_ColorHeightTexture;
	minfilter = point;
	magfilter = point;
	mipfilter = point;
};


struct VS_OUT
{
	vector			vPosition : POSITION;
	vector			vColor : COLOR0;
	float2			vTexUV : TEXCOORD0;
	vector			vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(vector vPosition : POSITION
		, vector vNormal : NORMAL
		, float2 vTexUV : TEXCOORD0)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = vTexUV;	

	vector	vWorld_Normal = normalize(mul(vector(vNormal.xyz, 0.f), g_matWorld));
	vector	vWorld_LightDirInv = normalize(g_vLightDir) * -1.f;

	Out.vColor = max(dot(vWorld_Normal, vWorld_LightDirInv), 0.f);
	Out.vWorldPos = mul(vector(vPosition.xyz, 1.f), g_matWorld);

	return Out;
}

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(vector vColor : COLOR0
			   , float2 vTexUV : TEXCOORD0
			   , vector	vWorldPos : TEXCOORD1)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vResultColor[5];

	vResultColor[0] = tex2D(TileSampler, vTexUV * 20.f);
	vResultColor[1] = tex2D(TileSampler2, vTexUV * 20.f);
	vResultColor[2] = tex2D(TileSampler3, vTexUV * 20.f);
	vResultColor[3] = tex2D(TileSampler4, vTexUV * 20.f);
	vResultColor[4] = tex2D(ColorHeightSampler, vTexUV);
	//vResultColor[0] = tex2D(BaseSampler, vTexUV * 20.0f);
	//vResultColor[1] = (vector)0;
	//vResultColor[2] = tex2D(TileSampler, vTexUV * 20.0f);
	//vResultColor[3] = tex2D(ColorHeightSampler, vTexUV);

	/*if(g_vTargetPos.x - g_fTargetRange < vWorldPos.x && vWorldPos.x < g_vTargetPos.x + g_fTargetRange
		&& g_vTargetPos.z - g_fTargetRange < vWorldPos.z && vWorldPos.z < g_vTargetPos.z + g_fTargetRange)
	{
		float2			vFloorTexUV;

		vFloorTexUV.x = (vWorldPos.x - (g_vTargetPos.x - g_fTargetRange)) / (g_fTargetRange * 2.f);
		vFloorTexUV.y = -(vWorldPos.z - (g_vTargetPos.z + g_fTargetRange)) / (g_fTargetRange * 2.f);

		vResultColor[1] = tex2D(FloorSampler, vFloorTexUV);	
	}*/

	/*Out.vColor = (vResultColor[0] * vResultColor[4].r
		+ vResultColor[1] * vResultColor[4].g
		+ vResultColor[2] * vResultColor[4].b
		+ vResultColor[3] * vResultColor[4].a) * g_vLightDiffuse * g_vTerrainDiffuse;*/

	Out.vColor = (vResultColor[0] * vResultColor[4].a
		+ vResultColor[1] * vResultColor[4].r
		+ vResultColor[2] * vResultColor[4].g
		+ vResultColor[3] * vResultColor[4].b)  * (g_vLightDiffuse * g_vTerrainDiffuse) * (vColor + (g_vLightAmbient * g_vTerrainAmbient));

	return Out;
}


technique Default
{
	pass Default_Terrain
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};