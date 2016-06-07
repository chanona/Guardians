#define MAX_LIGHTS									4
#define POINT_LIGHT									1.0f
#define SPOT_LIGHT									2.0f
#define DIRECTIONAL_LIGHT							3.0f
#define MAX_BONE_MATRICES							96

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES


// ������ ���� ����ü ����
struct MATERIAL
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular; // a = power
	float4 m_cEmissive;
};
// ������ ���� ����ü ����
struct LIGHT
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
	float3 m_vPosition;
	float m_fRange;
	float3 m_vDirection;
	float m_nType;
	float3 m_vAttenuation;
	float m_fFalloff;
	float m_fTheta;
	float m_fPhi;
	float m_bEnable;
	float padding;

};

struct LIGHTEDCOLOR
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
};

// ���̴� ������ ����ü�� ������ �� �ִ�. ����ü�� ��� ������ ���������� ���ȴ�.
// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ���̴� ������ �����Ѵ� ( ���� 0�� ��� )

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
}

// ���� ��ȯ ��������� ���̴� ������ �����Ѵ�( ���� 1�� ��� ) 
cbuffer cbWorldMatrix : register(b1)
{
	// register(b0)�� b�� �������Ͱ� ��� ���۸� ���� ��� �Ǵ� ���� �ǹ�
	// 0�� �������� ��ȣ �̸� ���� ���α׷����� ��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ҷ��� 
	// ���� ��ȣ�� ��ġ�ϵ��� �ؾ��Ѵ�. 
	// ex ) pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 
	// 1, &m_pd3dcbWorldMatrix);
	matrix gmtxWorld : packoffset(c0);
}
// ������ ���� �������
cbuffer cbLight : register(b0)
{
	LIGHT gLights[MAX_LIGHTS];
	float4 gcLightGlobalAmbient;
	float4 gvCameraPosition;
}

// ������ ���� �������
cbuffer cbMaterial : register(b1)
{
	MATERIAL gMaterial;
}
cbuffer cbSkinned : register(b2)
{
	float4x4 gBoneTransforms[MAX_BONE_MATRICES];
};

// 

struct VS_DIFFUSED_COLOR_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR0;
};
// ������ ������� �ʴ� ��� ���� ���̴��� ����� ���� ����ü
struct VS_DIFFUSED_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// ������ ����ϴ� ��� ���� ���̴��� �Է��� ���� ����ü
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

//������ ����ϴ� ��� ���� ���̴��� ����� ���� ����ü�̴�.
struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	// ���� ��ǥ�迡�� ������ ��ġ�� ���� ���͸� ��Ÿ����
	float3 positionW: POSITION;
	float3 normalW: NORMAL;

};

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 tex2dcoord : TEXCOORD0;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex2dcoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};

struct VS_TEXTURED_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};

struct VS_SKINNED_INPUT
{
	float3 posL     : POSITION;
	float3 normal      : NORMAL;
	float2 tex2dcoord  : TEXCOORD0;
	float4 boneWeights : BONEWEIGHTS;
	uint4 boneIndices : BONEINDICES;
};
struct VS_SKINNED_OUTPUT
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW	   : NORMAL;
	float2 tex2dcoord  : TEXCOORD0;
};
// ������ ������� �ʴ� ��� ���� ���̴��� �Է��� ���� ����ü

/********************************* Directional Light ***********************************/
// ���⼺ ������ ȿ���� ����ϴ� �Լ� . 
// ���⼺ ������ ��������� �Ÿ��� ���� ������ ���� �������ʴ´�
LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = -gLights[i].m_vDirection;
		float fDiffuseFactor = dot(vToLight, vNormal);
	//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
	if (fDiffuseFactor > 0.0f)
	{
		//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
		if (gMaterial.m_cSpecular.a != 0.0f)
		{
#ifdef _WITH_REFLECT
			float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
			float3 vHalf = normalize(vToCamera + vToLight);
#else
			float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
			output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
		}
		output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
	}
	output.m_cAmbient = gMaterial.m_cAmbient * gLights[i].m_cAmbient;
	return(output);
}

/*********************************** Point Light ***************************************/
// �� ���� ȿ���� ����ϴ� �Լ��̴�
LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//��������� �Ÿ��� ������ ��ȿ�Ÿ����� ���� ���� ������ ������ ����Ѵ�.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
		if (fDiffuseFactor > 0.0f)
		{
			//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
		//��������� �Ÿ��� ���� ������ ������ ����Ѵ�.
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return(output);
}

/************************************* Spot Light ***************************************/
LIGHTEDCOLOR SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//��������� �Ÿ��� ������ ��ȿ�Ÿ����� ���� ���� ������ ������ ����Ѵ�.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
		if (fDiffuseFactor > 0.0f)
		{
			//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[i].m_fPhi) / (gLights[i].m_fTheta - gLights[i].m_fPhi)), 0.0f), gLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}

// ������ ������ ����ϱ� ���� �Լ�
float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
		float3 vToCamera = normalize(vCameraPosition - vPosition);
		LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (i = 0; i < MAX_LIGHTS; i++)
		{
		//Ȱ��ȭ�� ���� ���Ͽ� ������ ������ ����Ѵ�.
		if (gLights[i].m_bEnable == 1.0f)
		{
			//������ ������ ���� ������ ������ ����Ѵ�.
			if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
			{
				LightedColor = DirectionalLight(i, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == POINT_LIGHT)
			{
				LightedColor = PointLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
			if (gLights[i].m_nType == SPOT_LIGHT)
			{
				LightedColor = SpotLight(i, vPosition, vNormal, vToCamera);
				cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
			}
		}
		}
	//�۷ι� �ֺ� ������ ������ ���� ���� ���Ѵ�.
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);
	//���� ������ ���İ��� ������ ��ǻ�� ������ ���İ����� �����Ѵ�.
	cColor.a = gMaterial.m_cDiffuse.a;
	return(cColor);
}

VS_DIFFUSED_COLOR_OUTPUT VSDiffusedColor(VS_DIFFUSED_COLOR_INPUT input)
{
	VS_DIFFUSED_COLOR_OUTPUT output = (VS_DIFFUSED_COLOR_OUTPUT)0;
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	
	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	output.color = input.color;
	return (output);
}

//������ ������ ����ϴ� ����� ���� ���̴� �Լ��̴�.
VS_LIGHTING_OUTPUT VSLightingColor(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT)0;
	//������ ������ ����ϱ� ���Ͽ� ������ǥ�迡�� ������ ��ġ�� ���� ���͸� ���Ѵ�.
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(input.position, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	return(output);
}
VS_TEXTURED_OUTPUT VSTexturedColor(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT)0;
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	output.tex2dcoord = input.tex2dcoord;

	return(output);
}

VS_TEXTURED_LIGHTING_OUTPUT VSTexturedLighting(VS_TEXTURED_LIGHTING_INPUT input)
{
	VS_TEXTURED_LIGHTING_OUTPUT output = (VS_TEXTURED_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(input.position, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	output.position = mul(float4(input.position, 1.0f), mtxWorldViewProjection);
	output.tex2dcoord = input.tex2dcoord;

	return(output);
}

//#define RENDER_VERTEX				
#define RENDER_SKINNED
VS_SKINNED_OUTPUT VSSkinned(VS_SKINNED_INPUT input)
{
	VS_SKINNED_OUTPUT output;
	
	// �迭 �ʱ�ȭ

#ifdef RENDER_SKINNED
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.boneWeights.x;
	weights[1] = input.boneWeights.y;
	weights[2] = input.boneWeights.z;
	weights[3] = input.boneWeights.w;


	// ���� ������ �����Ѵ�.
	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		posL += weights[i] * mul(float4(input.posL,1.0f), gBoneTransforms[input.boneIndices[i]]).xyz;
		normalL += weights[i] * mul(float4(input.normal, 1.0f), gBoneTransforms[input.boneIndices[i]]).xyz;
	}
#endif

	/*float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	posL = input.posL;
	normalL = input.normal;
*/
	// ���� �������� ��ȯ�Ѵ�.
	output.posW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
	output.normalW = mul(normalL, (float3x3)gmtxWorld);
	
	// ���� ���� �������� ��ȯ�Ѵ�.
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);

	output.posH = mul(float4(posL, 1.0f), mtxWorldViewProjection);

	output.tex2dcoord = input.tex2dcoord;

	return output;

}


// Color�� �����ϴ� ���̴� : �ȼ� ���̴�
float4 PSDiffusedColor(VS_DIFFUSED_COLOR_OUTPUT input) : SV_TARGET
{
	/*�ȼ�-���̴��� �׻� ������ ��Ÿ���� 4���� �Ǽ�(float4)�� ��ȯ�ؾ� �Ѵ�. 
	�Է� �Ķ���Ϳ� ������� ����� ������ ��ȯ�Ѵ�. 
	�׷��Ƿ� �������� ����� �Ǵ� ��� �ȼ��� ������ ������� �ȴ�(����� = ������ + ���).*/
	return input.color;
}

//������ ������ ����ϴ� ����� �ȼ� ���̴� �Լ��̴�.
float4 PSLightingColor(VS_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return(cIllumination);
}


// �ȼ� ���̴� �Լ��� ���� �ؽ��� ������ ���÷� ���� ������ ������ ���� ����.
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PSTexturedColor(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord);
	return(cColor);
}

float4 PSTexturedLighting(VS_TEXTURED_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord) * cIllumination;

	return(cColor);
}

float4 PSSkinned(VS_SKINNED_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.posW, input.normalW);
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord) * cIllumination;

	return(cColor);
}
