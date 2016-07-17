#include "Shader_Base.hpp"

vector				g_vLightDir; // ���⼺������ ���⺤�͸� �����ڴ�.
vector				g_vLightDiffuse; // ������ ���ݻ� ������ ������.
vector				g_vLightAmbient;
vector				g_vLightSpecular;

vector				g_vMeshDiffuse; // ������ü�� ���ݻ� ������ ���´�.
vector				g_vMeshAmbient;
vector				g_vMeshSpecular;
vector				g_vCameraPos; // ī�޶� ���� ����

								  // �� ���̴� �ø�ƽ ���� : ������, Į��, �븻, �ؽ���� //

								  //************ VertexShading *************** 
struct VS_OUT
{
	vector		vPosition : POSITION;		// ��ġ
	vector		vColor : COLOR0;			// �ؽ�ó �⺻�÷� ���ݻ籤(��ǻ��)
	float2		vTexUV : TEXCOORD0;			// �ؽ�ó ��ǥ
	vector		vReflection : TEXCOORD1;	// ���ݻ籤
	vector		vViewDir : TEXCOORD2;		// ���ؽ��� ī�޶� �ٶ󺸴� ���⺤��
};


VS_OUT VS_MAIN(vector vfactor_Pos : POSITION
	, vector vfactor_Normal : NORMAL0
	, float2 vfactor_TexUV : TEXCOORD0)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView); // ���� * �� -> ��������� �̵�
	matWVP = mul(matWV, g_matProj);		// �� * ���� -> ������������ �̵�

										// �븻�� ����길 �ϸ�Ǽ� ��������� �����ش�.
	vector	vWorld_Normal = normalize(mul(vector(vfactor_Normal.xyz, 0.f), g_matWorld));
	vector	vWorld_LightDirInv = normalize(g_vLightDir) * -1.f;
	vector	vPos = mul(vfactor_Pos.xyz, g_matWorld);

	// ��µ�����_������ �� ���� * ���� �Ѱ� �־��ش�
	Out.vPosition = mul(vector(vfactor_Pos.xyz, 1.f), matWVP);

	// ��µ�����_UV��ǥ �� ������ ��ǥ�� �־��ش� 
	// �̰� ������ �ȿŰܵ� �ȴ� �� ���� ��������ǥ
	Out.vTexUV = vfactor_TexUV;

	// ��ǻ� ���ؽ� �븻���Ͷ� ���ǿ����� ���͸� �����ؼ�, 0�̶� �̰��� ū�� �־��ش�.
	Out.vColor = max(dot(vWorld_Normal, vWorld_LightDirInv), 0.f);

	// ���ؽ� �ٶ󺸴� ���⺤�Ϳ� ����(ī�޶� �ٶ󺸴� ����)
	Out.vViewDir = vector((vPos - g_vCameraPos).xyz, 0.f);

	// ���ݻ籤�� ���÷�Ʈ�Ѱ� �־��ش�.
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

// technique : ��ġ�� �������ο����� �ٸ� ���̵��� �� �� �ֵ��� �Ѵ�.
technique Default
{
	// pass : ����� ĸ��ȭ
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