#pragma once
#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
#include <memory>

using namespace std;

//struct CMyVertex
//{
//	D3DXVECTOR3								mPos;		// 12
//	D3DXVECTOR3								mNormal;	// 12
//	D3DXVECTOR2								mUV;		// 8
//
//	CMyVertex()
//		: mPos(D3DXVECTOR3(0, 0, 0)), mNormal(D3DXVECTOR3(0, 0, 0)), mUV(D3DXVECTOR2(0, 0))
//	{}
//	CMyVertex(const D3DXVECTOR3& pos, const D3DXVECTOR3& normal, const D3DXVECTOR2& uv)
//		: mPos(pos), mNormal(normal), mUV(uv)
//	{}
//	CMyVertex(const CMyVertex& otherData)
//	{
//		mPos = otherData.mPos;
//		mNormal = otherData.mNormal;
//		mUV = otherData.mUV;
//	}
//	~CMyVertex(){};
//};

struct CMyBoneVertex
{
	D3DXVECTOR3								mPos;		// 12
	D3DXVECTOR3								mNormal;	// 12
	D3DXVECTOR2								mUV;		// 8
	D3DXVECTOR4								mBoneWeights;		 //16
	UINT									mBoneIndices[4];	// 16

	CMyBoneVertex()
		:mPos(0,0,0),
		mNormal(0, 0, 0),
		mUV(0, 0),
		mBoneWeights(0, 0, 0, 0)
	{
		for (int i = 0; i < 4; ++i) mBoneIndices[i] = 0;
	}
	CMyBoneVertex(const D3DXVECTOR3& pos, const D3DXVECTOR3& normal, const D3DXVECTOR2& uv, const D3DXVECTOR4& boneWeights, UINT* boneIndices)
		:mPos(pos),
		mNormal(normal),
		mUV(uv),
		mBoneWeights(boneWeights)
	{
		for (int i = 0; i < 4; ++i) mBoneIndices[i] = boneIndices[i];
	}
	CMyBoneVertex(const CMyBoneVertex& otherData)
	{
		mPos = otherData.mPos;
		mNormal = otherData.mNormal;
		mUV = otherData.mUV;
		mBoneWeights = otherData.mBoneWeights;
		for (int i = 0; i < 4; ++i) mBoneIndices[i] = otherData.mBoneIndices[i];
	}
	~CMyBoneVertex()
	{}

};
//struct CMyBoneVertex : public CMyVertex
//{
//	D3DXVECTOR4								mBoneWeights;		 //16
//	UINT									mBoneIndices[4];	// 16
//
//	CMyBoneVertex()
//		:CMyVertex(),
//		 mBoneWeights(0, 0, 0, 0)
//	{
//		for (int i = 0; i < 4; ++i) mBoneIndices[i] = 0;
//	}
//	CMyBoneVertex(const D3DXVECTOR3& pos, const D3DXVECTOR3& normal, const D3DXVECTOR2& uv, const D3DXVECTOR4& boneWeights , UINT* boneIndices )
//		:CMyVertex(pos, normal, uv),
//		mBoneWeights(boneWeights)
//	{
//		for (int i = 0; i < 4; ++i) mBoneIndices[i] = boneIndices[i];
//	}
//	CMyBoneVertex(const CMyBoneVertex& otherData)
//		:CMyVertex(CMyVertex(otherData.mPos, otherData.mNormal, otherData.mUV))
//	{
//		for (int i = 0; i < 4; ++i) mBoneIndices[i] = otherData.mBoneIndices[i];
//		mBoneWeights = otherData.mBoneWeights;
//	}
//	~CMyBoneVertex()
//	{}
//
//};

struct KeyFrame
{
	float timePos;

	D3DXVECTOR3 translation;
	D3DXVECTOR3	scale;
	D3DXQUATERNION rotation;

	KeyFrame()
		:timePos(0),
		 translation(0, 0, 0),
		 scale(0, 0, 0),
		 rotation(0, 0, 0, 0)
	{}
	~KeyFrame(){};
};

struct BoneAnimation
{
	vector<KeyFrame>					  m_vKeyFrames;

	float								  GetStartTime() const { return m_vKeyFrames.front().timePos; }
	float								  GetEndTime() const { return m_vKeyFrames.back().timePos; }

	void								  Interpolate(float t, D3DXMATRIX& M) const;
};

struct AnimationClip
{
	vector<BoneAnimation>				  m_vBoneAnimations;	// ���뺰 �ִϸ��̼ǵ�

	
	float								  GetClipStartTime() const;	// �ִϸ��̼� Ŭ���� ���� �� ���� �̸� ���� �ð��� ��ȯ
	float								  GetClipEndTime() const;	// �ִϸ��̼� Ŭ���� ���� �� ���� ���� ���� �ð��� ��ȯ

	void								  Interpolate(float t, vector<D3DXMATRIX>& boneTransforms) const;	// �ִϸ��̼��� ����
};

class SkinnedData
{
public:
	UINT BoneCount() const;

	float GetClipStartTime(const string& clipName) const;
	float GetClipEndTime(const string& clipName) const;

	void Set(
		vector<int>& boneHierarchy,
		vector<D3DXMATRIX>& boneOffsets,
		map<string, AnimationClip>& animations);

	void GetFinalTransforms(const string& clipName, float timePos, vector<D3DXMATRIX>& finalTransforms, TEX_TYPE eType) const;
	
private:
	// i�� ������ �θ��� ����(parentIndex)�� ��´�.
	// i�� ����� �ִϸ��̼� Ŭ���� i��° BoneAnimation �ν��Ͻ��� �����ȴ�.
	vector<int> m_vBoneHierarchy;

	// i�� ������ ������ ��ȯ
	vector<D3DXMATRIX> m_vBoneOffsets;

	map<string, AnimationClip> mAnimations;

	bool	m_bStatic;
};



