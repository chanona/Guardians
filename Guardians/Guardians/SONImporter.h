#pragma once

#include <iostream>
#include "SONData.h"
#include "stdafx.h"

using namespace std;

class SBXImporter
{
public:
	bool ImportSBX(const string& fileName,
		vector<CMyBoneVertex>& vertices,
		vector<USHORT>& indices,
		SkinnedData& skinInfo,
		TEX_TYPE eType);

private:
	void ReadMeshDataFromSBX(ifstream& fin, UINT numVertices,UINT numIndices, vector<CMyBoneVertex>& vertices, vector<USHORT>& indices, int meshID, TEX_TYPE eType);
	void ReadBoneHierarchyFromSBX(ifstream& fin, UINT numBones, vector<int>& boneIndexToParentIndex, int meshID);
	void ReadOffsetMatrixFromSBX(ifstream& fin, UINT numBones, vector<D3DXMATRIX>& boneOffsets,int meshID);
	void ReadAnimationClipsFromSBX(ifstream& fin, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations);
	void ReadTimeTransformFromSBX(ifstream& fin, UINT numBones,  BoneAnimation& boneAnimation);
};

