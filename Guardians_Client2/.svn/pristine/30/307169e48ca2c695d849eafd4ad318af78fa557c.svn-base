#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.
vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;

vector				g_vMeshDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vMeshAmbient;

struct VS_OUT
{
	vector		vPosition : POSITION;
	float		fShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
};

// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(vector vPosition : POSITION
					 , vector vNormal : NORMAL0
					 , float2 vTexUV : TEXCOORD0)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);

	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(vPosition.xyz, 1.f), matWVP);

	vector	vWorld_Normal = normalize(mul(vector(vNormal.xyz, 0.f), g_matWorld));
	vector	vWorld_LightDirInv = normalize(g_vLightDir) * -1.f;

	Out.fShade = max(dot(vWorld_Normal, vWorld_LightDirInv), 0.f);

	Out.vTexUV = vTexUV;

	return Out;
}

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(float fShade : COLOR0
			, float2 vTexUV : TEXCOORD0 )
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vColor;

	vColor = tex2D(BaseSampler, vTexUV);

	Out.vColor = vColor * fShade;

	Out.vColor.a = 1.0f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Default_Mesh
	{	
		// Pixel.a * (Pixel.rgb) + (1.f - Pixel.a) * (Draw.rgb)		
		Alphablendenable =true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};