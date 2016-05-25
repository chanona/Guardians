#include "stdafx.h"
#include "SONData.h"
#include <fstream>
#include <cmath>

#define DEBUG_ANIMATE_TEST_1
//#define DEBUG_ANIMATE_TEST_2

void BoneAnimation::Interpolate(float t, D3DXMATRIX& M)  const
{

	if (t <= m_vKeyFrames.front().timePos)
	{
		float S = m_vKeyFrames.front().scale.y;
		D3DXVECTOR3 T = m_vKeyFrames.front().translation;
		D3DXQUATERNION Q = m_vKeyFrames.front().rotation;

		D3DXVECTOR3 pivot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		D3DXMatrixAffineTransformation(&M, S, &pivot, &Q, &T);
	}
	else if (t >= m_vKeyFrames.back().timePos)
	{
		float S = m_vKeyFrames.back().scale.y;
		D3DXVECTOR3 T = m_vKeyFrames.back().translation;
		D3DXQUATERNION Q = m_vKeyFrames.back().rotation;

		D3DXVECTOR3 pivot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		D3DXMatrixAffineTransformation(&M, S, &pivot, &Q, &T);
	}
	else
	{
		for (UINT i = 0; i < m_vKeyFrames.size() -1 ; ++i)
		{
			if (t >= m_vKeyFrames[i].timePos && t <= m_vKeyFrames[i + 1].timePos)
			{
				float lerpPercent = (t - m_vKeyFrames[i].timePos) / (m_vKeyFrames[i + 1].timePos - m_vKeyFrames[i].timePos);
				
				D3DXVECTOR3 s0 = m_vKeyFrames[i].scale;
				D3DXVECTOR3 s1 = m_vKeyFrames[i + 1].scale;
				
				D3DXVECTOR3 p0 = m_vKeyFrames[i].translation;
				D3DXVECTOR3 p1 = m_vKeyFrames[i + 1].translation;
				
				D3DXQUATERNION q0 = m_vKeyFrames[i].rotation;
				D3DXQUATERNION q1 = m_vKeyFrames[i + 1].rotation;

				D3DXVECTOR3 S,P;
				D3DXQUATERNION Q;
				
				D3DXVec3Lerp(&S, &s0, &s1, lerpPercent);
				D3DXVec3Lerp(&P, &p0, &p1, lerpPercent);
				D3DXQuaternionSlerp(&Q, &q0, &q1, lerpPercent);
			
				D3DXVECTOR3 pivot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				D3DXMatrixAffineTransformation(&M, S.y, &pivot, &Q, &P);

				/*cout << M._11 << ' ' << M._12 << ' ' << M._13 << ' ' << M._14 << endl;
				cout << M._21 << ' ' << M._22 << ' ' << M._23 << ' ' << M._24 << endl;
				cout << M._31 << ' ' << M._32 << ' ' << M._23 <<  ' ' << M._34 << endl;
				cout << M._41 << ' ' << M._42 << ' ' << M._33 << ' ' << M._44 << endl;
				cout << endl;
				*/
				break;
			}
		}
	}
}


float AnimationClip::GetClipStartTime() const
{
	float t = FLT_MAX;

	for (UINT i = 0; i < m_vBoneAnimations.size(); ++i)
	{
		if (m_vBoneAnimations[i].m_vKeyFrames.size() == 0) continue;

		t = fmin(t, m_vBoneAnimations[i].GetStartTime());
	}
	
	return t;


}
float AnimationClip::GetClipEndTime() const
{
	float t = 0.0f;
	
	for (UINT i = 0; i < m_vBoneAnimations.size(); ++i)
	{
		if (m_vBoneAnimations[i].m_vKeyFrames.size() == 0) continue;

		t = fmax(t, m_vBoneAnimations[i].GetEndTime());
	}

	return t;

}

void AnimationClip::Interpolate(float t, vector<D3DXMATRIX>& boneTransforms) const
{
	for (UINT i = 0; i < m_vBoneAnimations.size(); ++i)
	{
		if (m_vBoneAnimations[i].m_vKeyFrames.size() > 0)
			m_vBoneAnimations[i].Interpolate(t, boneTransforms[i]);
		else
		{
			D3DXMATRIX identityMtx; 
			D3DXMatrixIdentity(&identityMtx);
			boneTransforms[i] = identityMtx;
		}
	}
	/*cout << boneTransforms[0]._11 << ' ' << boneTransforms[0]._12 << ' ' << boneTransforms[0]._13 << ' ' << boneTransforms[0]._14 << endl;
	cout << boneTransforms[0]._21 << ' ' << boneTransforms[0]._22 << ' ' << boneTransforms[0]._23 << ' ' << boneTransforms[0]._24 << endl;
	cout << boneTransforms[0]._31 << ' ' << boneTransforms[0]._32 << ' ' << boneTransforms[0]._23 << ' ' << boneTransforms[0]._34 << endl;
	cout << boneTransforms[0]._41 << ' ' << boneTransforms[0]._42 << ' ' << boneTransforms[0]._33 << ' ' << boneTransforms[0]._44 << endl;
	cout << endl;*/

}

float SkinnedData::GetClipStartTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipStartTime();
}

float SkinnedData::GetClipEndTime(const std::string& clipName)const
{
	auto clip = mAnimations.find(clipName);
	return clip->second.GetClipEndTime();
}

UINT SkinnedData::BoneCount() const
{
	return m_vBoneHierarchy.size();
}

void SkinnedData::Set(vector<int>& boneHierarchy,
					  vector<D3DXMATRIX>& boneOffsets,
					  map<string, AnimationClip>& animations)
{
	m_vBoneHierarchy = boneHierarchy;
	m_vBoneOffsets = boneOffsets;
	mAnimations = animations;
}

void SkinnedData::GetFinalTransforms(const string& clipName, float timePos, vector<D3DXMATRIX>& finalTransforms) const
{
#ifdef DEBUG_ANIMATE_TEST_1
	UINT numBones = m_vBoneOffsets.size();
	
	vector<D3DXMATRIX> toRootTransforms(numBones);

	auto clip = mAnimations.find(clipName);
	clip->second.Interpolate(timePos, toRootTransforms);

	
	finalTransforms.resize(numBones);
	
	for (UINT i = 0; i < numBones; ++i)
	{
		D3DXMATRIX offSet = m_vBoneOffsets[i];
		D3DXMATRIX toRoot = toRootTransforms[i];
	
		finalTransforms[i] = offSet * toRoot;
	}
#endif

#ifdef DEBUG_ANIMATE_TEST_2

	static int index = 0;

	UINT numBones = m_vBoneOffsets.size();
	
	vector<D3DXMATRIX> toRootTransforms(numBones);

	auto clip = mAnimations.find(clipName);


	finalTransforms.resize(numBones);

	for (UINT i = 0; i < numBones; ++i)
	{
		if (clip->second.m_vBoneAnimations[i].m_vKeyFrames.size() > 0)
		{
			D3DXVECTOR3 P, S;
			D3DXQUATERNION Q;
			D3DXMATRIX M;

			S = clip->second.m_vBoneAnimations[i].m_vKeyFrames[index].scale;
			P = clip->second.m_vBoneAnimations[i].m_vKeyFrames[index].translation;
			Q = clip->second.m_vBoneAnimations[i].m_vKeyFrames[index].rotation;

			D3DXVECTOR3 pivot(0.0f, 0.0f, 0.0f);
			D3DXMatrixAffineTransformation(&M, 1.0, &pivot, &Q, &P);

			toRootTransforms[i] = M;
		}
		else
		{
			D3DXMATRIX mtx; 
			D3DXMatrixIdentity(&mtx);
			toRootTransforms[i] = mtx;

		}
	}

	for (UINT i = 0; i < numBones; ++i)
	{
		D3DXMATRIX offSet = m_vBoneOffsets[i];
		D3DXMATRIX toRoot = toRootTransforms[i];

		finalTransforms[i] = offSet * toRoot;
	}

	index++;

	if (index > clip->second.m_vBoneAnimations[0].m_vKeyFrames.size() - 1 )
		index = 0;



#endif

	//cout << finalTransforms[0]._11 << ' ' << finalTransforms[0]._12 << ' ' << finalTransforms[0]._13 << endl;
}