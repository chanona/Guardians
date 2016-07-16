#include "Shader_Base.hpp"

vector				g_vLightDir; // 방향성광원의 방향벡터를 얻어오겠다.
vector				g_vLightDiffuse; // 광원의 난반사 색상을 얻어오자.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vMeshDiffuse; // 지형객체의 난반사 색상을 얻어온다.
vector				g_vMeshAmbient;
vector				g_vMeshSpecular;
vector				g_vCameraPos; // 카메라 포즈 데스

								  // ★ 쉐이더 시멘틱 순서 : 포지션, 칼라, 노말, 텍스쿠드 //

								  //************ VertexShading *************** 
struct VS_OUT
{
	vector		vPosition : POSITION;		// 위치
	vector		vColor : COLOR0;			// 텍스처 기본컬러 난반사광(디퓨즈)
	float2		vTexUV : TEXCOORD0;			// 텍스처 좌표
	vector		vReflection : TEXCOORD1;	// 정반사광
	vector		vViewDir : TEXCOORD2;		// 버텍스가 카메라 바라보는 방향벡터
};


VS_OUT VS_MAIN(vector vfactor_Pos : POSITION
	, vector vfactor_Normal : NORMAL0
	, float2 vfactor_TexUV : TEXCOORD0)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView); // 월드 * 뷰 -> 뷰공간으로 이동
	matWVP = mul(matWV, g_matProj);		// 뷰 * 투영 -> 투영공간으로 이동

										// 노말은 값계산만 하면되서 월드까지만 보내준다.
	vector	vWorld_Normal = normalize(mul(vector(vfactor_Normal.xyz, 0.f), g_matWorld));
	vector	vWorld_LightDirInv = normalize(g_vLightDir) * -1.f;
	vector	vPos = mul(vfactor_Pos.xyz, g_matWorld);

	// 출력데이터_포지션 에 벡터 * 투영 한걸 넣어준다
	Out.vPosition = mul(vector(vfactor_Pos.xyz, 1.f), matWVP);

	// 출력데이터_UV좌표 에 유브이 좌표도 넣어준다 
	// 이건 투영에 안옮겨도 된다 걍 숫자 유브이좌표
	Out.vTexUV = vfactor_TexUV;

	// 디퓨즈값 버텍스 노말벡터랑 빛의역방향 벡터를 내적해서, 0이랑 이값중 큰걸 넣어준다.
	Out.vColor = max(dot(vWorld_Normal, vWorld_LightDirInv), 0.f);

	// 버텍스 바라보는 방향벡터에 월드(카메라를 바라보는 포즈)
	Out.vViewDir = vector((vPos - g_vCameraPos).xyz, 0.f);

	// 정반사광에 리플렉트한값 넣어준다.
	Out.vReflection = reflect(g_vLightDir, vWorld_Normal);


	return Out;
}


//************ PixelShading *************** 
struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT PS_MAIN(float vfactor_Color : COLOR0
	, float2 vfactor_TexUV : TEXCOORD0
	, vector vfactor_Reflection : TEXCOORD1
	, vector vfactor_ViewDir : TEXCOORD2)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vColor;
	vector		vViewDir = normalize(vfactor_ViewDir);
	vector		vReflection = normalize(vfactor_Reflection);
	vector		vSpecular = (vector)0;

	if (vfactor_Color.x > 0)
	{
		vSpecular = saturate(dot(vReflection, -vViewDir));
		vSpecular = pow(vSpecular, 10.0f);
	}

	vColor = tex2D(BaseSampler, vfactor_TexUV);

	Out.vColor = (g_vLightDiffuse * g_vMeshDiffuse) * (vColor + (g_vLightAmbient * g_vMeshAmbient) + (g_vLightSpecular * g_vMeshSpecular) * vSpecular);

	Out.vColor.a = vColor.a;

	return Out;
}

// technique : 장치의 지원여부에따라 다른 셰이딩을 할 수 있도록 한다.
technique Default
{
	// pass : 기능의 캡슐화
	pass Default_Mesh
{
	//CullMode = none;
	// Pixel.a * (Pixel.rgb) + (1.f - Pixel.a) * (Draw.rgb)	
	Alphablendenable = true;
srcblend = srcalpha;
destblend = invsrcalpha;

//Alphablendenable = true;
//srcblend = srcalpha;
//destblend = one/*invsrcalpha*/;

/*AlphaTestenable = true;
AlphaRef = 0x88;
AlphaFunc = greater;*/

VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}
};