#define MAX_LIGHTS									4
#define POINT_LIGHT									1.0f
#define SPOT_LIGHT									2.0f
#define DIRECTIONAL_LIGHT							3.0f
#define MAX_BONE_MATRICES							96

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES


// 재질을 위한 구조체 선언
struct MATERIAL
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular; // a = power
	float4 m_cEmissive;
};
// 조명을 위한 구조체 선언
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

// 쉐이더 변수는 구조체로 선언할 수 있다. 구조체의 멤버 변수는 전역변수로 사용된다.
// 카메라 변환 행렬과 투영 변환 행렬을 위한 쉐이더 변수를 선언한다 ( 슬롯 0을 사용 )

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView : packoffset(c0);
	matrix gmtxProjection : packoffset(c4);
}

// 월드 변환 행렬을위한 쉐이더 변수를 선언한다( 슬롯 1을 사용 ) 
cbuffer cbWorldMatrix : register(b1)
{
	// register(b0)의 b는 레지스터가 상수 버퍼를 위해 사용 되는 것을 의미
	// 0은 레지스터 번호 이며 응용 프로그램에서 상수 버퍼를 디바이스 컨텍스트에 연결할때의 
	// 슬롯 번호와 일치하도록 해야한다. 
	// ex ) pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_WORLD_MATRIX, 
	// 1, &m_pd3dcbWorldMatrix);
	matrix gmtxWorld : packoffset(c0);
}
// 조명을 위한 상수버퍼
cbuffer cbLight : register(b0)
{
	LIGHT gLights[MAX_LIGHTS];
	float4 gcLightGlobalAmbient;
	float4 gvCameraPosition;
}

// 재질을 위한 상수버퍼
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
// 조명을 사용하지 않는 경우 정점 쉐이더의 출력을 위한 구조체
struct VS_DIFFUSED_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 조명을 사용하는 경우 정점 쉐이더의 입력을 위한 구조체
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

//조명을 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	// 월드 좌표계에서 정점의 위치와 법선 벡터를 나타낸다
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
// 조명을 사용하지 않는 경우 정점 쉐이더의 입력을 위한 구조체

/********************************* Directional Light ***********************************/
// 방향성 조명의 효과를 계산하는 함수 . 
// 방향성 조명은 조명까지의 거리에 따라 조명의 양이 변하지않는다
LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = -gLights[i].m_vDirection;
		float fDiffuseFactor = dot(vToLight, vNormal);
	//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
	if (fDiffuseFactor > 0.0f)
	{
		//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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
// 점 조명 효과를 계산하는 함수이다
LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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
		//조명까지의 거리에 따라 조명의 영향을 계산한다.
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
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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

// 조명의 영향을 계산하기 위한 함수
float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
		float3 vToCamera = normalize(vCameraPosition - vPosition);
		LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (i = 0; i < MAX_LIGHTS; i++)
		{
		//활성화된 조명에 대하여 조명의 영향을 계산한다.
		if (gLights[i].m_bEnable == 1.0f)
		{
			//조명의 유형에 따라 조명의 영향을 계산한다.
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
	//글로벌 주변 조명의 영향을 최종 색상에 더한다.
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);
	//최종 색상의 알파값은 물질의 디퓨즈 색상의 알파값으로 설정한다.
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

//조명의 영향을 계산하는 경우의 정점 쉐이더 함수이다.
VS_LIGHTING_OUTPUT VSLightingColor(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT)0;
	//조명의 영향을 계산하기 위하여 월드좌표계에서 정점의 위치와 법선 벡터를 구한다.
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
	
	// 배열 초기화

#ifdef RENDER_SKINNED
	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = input.boneWeights.x;
	weights[1] = input.boneWeights.y;
	weights[2] = input.boneWeights.z;
	weights[3] = input.boneWeights.w;


	// 정점 블렌딩을 수행한다.
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
	// 월드 공간으로 변환한다.
	output.posW = mul(float4(posL, 1.0f), gmtxWorld).xyz;
	output.normalW = mul(normalL, (float3x3)gmtxWorld);
	
	// 동차 절단 공간으로 변환한다.
	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);

	output.posH = mul(float4(posL, 1.0f), mtxWorldViewProjection);

	output.tex2dcoord = input.tex2dcoord;

	return output;

}


// Color를 결정하는 쉐이더 : 픽셀 쉐이더
float4 PSDiffusedColor(VS_DIFFUSED_COLOR_OUTPUT input) : SV_TARGET
{
	/*픽셀-쉐이더는 항상 색상을 나타내는 4개의 실수(float4)를 반환해야 한다. 
	입력 파라메터에 관계없이 노란색 색상을 반환한다. 
	그러므로 렌더링의 대상이 되는 모든 픽셀의 색상은 노란색이 된다(노란색 = 빨간색 + 녹색).*/
	return input.color;
}

//조명의 영향을 계산하는 경우의 픽셀 쉐이더 함수이다.
float4 PSLightingColor(VS_LIGHTING_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	return(cIllumination);
}


// 픽셀 쉐이더 함수를 위한 텍스쳐 변수와 샘플러 상태 변수를 다음과 같이 정의.
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
