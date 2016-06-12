// 전역변수 (Constant Table)
matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

// VertexShading의 입력값으로 사용될 정점의 정보
struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal	: NORMAL;
	float3		vUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float3		vUV : TEXCOORD0;
};


// VertexShading : 정점의 변환 (영역의 변환)
VS_OUT/*정점*/ VS_MAIN(VS_IN In/*정점*/)
{
	VS_OUT		Out = (VS_OUT)0;

	// matWV : 월드 * 뷰
	// matWVP : 월드 * 뷰 * 투영
	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);

	matWVP = mul(matWV, g_matProj);	

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vUV = In.vUV;

	return Out;
}

// COLOR, TEXCOORD
struct PS_IN 
{
	float3		vUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = texCUBE(BaseSampler, In.vUV);

	Out.vColor.a = 0.f;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Skybox
	{	
		CullMode = none;
		
		Zenable = false;
		Zwriteenable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};