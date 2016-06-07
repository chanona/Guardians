#include "stdafx.h"
#include "SONImporter.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;


void SBXImporter::ReadMeshDataFromSBX(ifstream& fin, UINT numVertices, UINT numIndices, vector<CMyBoneVertex>& vertices, vector<USHORT>& indices, int meshID, TEX_TYPE eType)
{
	string L_DUMMY, C_DUMMY, R_DUMMY;
	string LABLE;
	
	D3DXVECTOR4 dummyV;
	// Vertex Data Import
	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY; // ###### VERTEX_DATA #######

	vertices.resize(numVertices);
	indices.resize(numIndices);

	for (UINT i = 0; i < numVertices; ++i)
	{
		if (eType == TEXTYPE_DYNAMIC)
		{
			fin >> LABLE >> vertices[i].mPos.x >> vertices[i].mPos.y >> vertices[i].mPos.z;
			fin >> LABLE >> vertices[i].mNormal.x >> vertices[i].mNormal.y >> vertices[i].mNormal.z;
			fin >> LABLE >> vertices[i].mUV.x >> vertices[i].mUV.y;
			fin >> LABLE >> vertices[i].mBoneIndices[0] >> vertices[i].mBoneIndices[1] >> vertices[i].mBoneIndices[2] >> vertices[i].mBoneIndices[3];
			fin >> LABLE >> vertices[i].mBoneWeights.x >> vertices[i].mBoneWeights.y >> vertices[i].mBoneWeights.z >> vertices[i].mBoneWeights.w;
		}
		else if (eType == TEXTYPE_STATIC)
		{
			fin >> LABLE >> vertices[i].mPos.x >> vertices[i].mPos.y >> vertices[i].mPos.z;
			fin >> LABLE >> vertices[i].mNormal.x >> vertices[i].mNormal.y >> vertices[i].mNormal.z;
			fin >> LABLE >> vertices[i].mUV.x >> vertices[i].mUV.y;
		}
	}

	// Index Data Import

	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY; // ###### INDEX_DATA  #####

	WORD index = 0;

	for (UINT i = 0; i < numIndices; ++i)
	{
		fin >> index;
		indices[i] = (WORD)index;
	}
	
}

void SBXImporter::ReadBoneHierarchyFromSBX(ifstream& fin, UINT numBones, vector<int>& boneIndexToParentIndex, int meshID)
{
	string L_DUMMY, C_DUMMY, R_DUMMY;
	string LABLE;

	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY; // ### BONE_HIERARCHY ####

	boneIndexToParentIndex.resize(numBones);

	int PARENT_INDEX = 0;

	for (UINT i = 0; i < numBones; ++i)
	{
		fin >> LABLE >> PARENT_INDEX; // ParentIndexOfBone0: -1
		boneIndexToParentIndex[i] = PARENT_INDEX;
	}
}


#define MATRIX_LOAD_CASE_1
//#define MATRIX_LOAD_CASE_2
void SBXImporter::ReadOffsetMatrixFromSBX(ifstream& fin, UINT numBones, vector<D3DXMATRIX>& boneOffsets, int meshID)
{
	string L_DUMMY, C_DUMMY, R_DUMMY;
	string LABLE;

	boneOffsets.resize(numBones);

	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY;  // ##### OFFSET_MATRIX #####
	
	for (UINT i = 0; i < numBones; ++i)
	{
#ifdef MATRIX_LOAD_CASE_1  // BoneOffSet0: 1.39743e-06 -1.01935 6.24172e-17 0 0.876104 1.20
		fin >> LABLE
			>> boneOffsets[i]._11 >> boneOffsets[i]._12 >> boneOffsets[i]._13 >> boneOffsets[i]._14
			>> boneOffsets[i]._21 >> boneOffsets[i]._22 >> boneOffsets[i]._23 >> boneOffsets[i]._24
			>> boneOffsets[i]._31 >> boneOffsets[i]._32 >> boneOffsets[i]._33 >> boneOffsets[i]._34
			>> boneOffsets[i]._41 >> boneOffsets[i]._42 >> boneOffsets[i]._43 >> boneOffsets[i]._44;
#endif 
#ifdef MATRIX_LOAD_CASE_2
		fin >> LABLE
			>> boneOffsets[i]._11 >> boneOffsets[i]._21 >> boneOffsets[i]._31 >> boneOffsets[i]._41
			>> boneOffsets[i]._12 >> boneOffsets[i]._22 >> boneOffsets[i]._32 >> boneOffsets[i]._42
			>> boneOffsets[i]._13 >> boneOffsets[i]._23 >> boneOffsets[i]._33 >> boneOffsets[i]._43
			>> boneOffsets[i]._14 >> boneOffsets[i]._24 >> boneOffsets[i]._34 >> boneOffsets[i]._44;
#endif 
	}

}

void SBXImporter::ReadAnimationClipsFromSBX(ifstream& fin, UINT numBones, UINT numAnimationClips, map<string, AnimationClip>& animations)
{
	string L_DUMMY, C_DUMMY, R_DUMMY;
	string BRACKET;
	string LABLE;

	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY; // ###### ANIMATION_CLIPS ####

	for (UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
	{
		string ANI_NAME;

		fin >> LABLE >> ANI_NAME; // AnimationClip Take_001
		fin >> BRACKET; // {

		AnimationClip clip;
		clip.m_vBoneAnimations.resize(numBones);

		for (UINT boneIndex = 0; boneIndex < numBones; ++boneIndex)
		{
			ReadTimeTransformFromSBX(fin, numBones, clip.m_vBoneAnimations[boneIndex]);
		}

		fin >> BRACKET;  // }
		animations[ANI_NAME] = clip;
	}
}

void SBXImporter::ReadTimeTransformFromSBX(ifstream& fin, UINT numBones, BoneAnimation& boneAnimation)
{
	string BRACKET;
	string LABLE;
	string COLON;

	int BONEINDEX;
	int ANI_LENGTH;

	fin >> LABLE >> BONEINDEX >> COLON >> ANI_LENGTH;  // Bone 0: 41
	fin >> BRACKET;  // {
	
	
	/*fin >> LABLE >> LABLE >> ANI_LENGTH;
	fin >> BRACKET;
*/
	boneAnimation.m_vKeyFrames.resize(ANI_LENGTH);

	if (ANI_LENGTH == 0)
	{
		fin >> BRACKET;	 // }
	}
	else
	{
		for (int i = 0; i < ANI_LENGTH; ++i)
		{
			KeyFrame FRAME;

			fin >> LABLE >> FRAME.timePos
				>> LABLE >> FRAME.translation.x >> FRAME.translation.y >> FRAME.translation.z
				>> LABLE >> FRAME.scale.x >> FRAME.scale.y >> FRAME.scale.z
				>> LABLE >> FRAME.rotation.x >> FRAME.rotation.y >> FRAME.rotation.z >> FRAME.rotation.w;

			boneAnimation.m_vKeyFrames[i] = FRAME;

		}
		fin >> BRACKET;
	}
}


bool SBXImporter::ImportSBX(const string& fileName,
	vector<CMyBoneVertex>& vertices,
	vector<USHORT>& indices,
	SkinnedData& skinInfo,
	TEX_TYPE eType)
{
	cout << "된다 0" << endl;
	cout << "파일 이름이" << fileName << "맞니 ??" << endl;
	
	ifstream fin(fileName.c_str(), ios_base::binary);

	if (!fin)
	{
		cout << "파일이 안열림" << endl;
		std::cerr << fileName << " 파일이 열리지 않습니다 " << endl;

		return false;
	}

	cout << "파일은 열린다고 읽히네??" << endl;

	string L_DUMMY, C_DUMMY, R_DUMMY;  
	string LABLE;  

	UINT numMeshCount = 0;
	UINT numVertices = 0;
	UINT numIndices = 0;
	UINT numBones = 0;
	UINT numAnimationClips = 0;

	fin >> L_DUMMY >> C_DUMMY >> R_DUMMY;	// #### SBX_META_DATA ###
	fin >> LABLE >> numMeshCount;  // MeshCount: 1	

	cout << "실험 : " << L_DUMMY << "\t" << C_DUMMY << "\t" << R_DUMMY << endl;

	for (UINT i = 0; i < numMeshCount; ++i)
	{
		cout << "된다 1" << endl;
		fin >> L_DUMMY >> C_DUMMY >> R_DUMMY; // #### MESH_DATA ####
		fin >> LABLE >> numVertices;  // VertexCount: 6922
		fin >> LABLE >> numIndices;   // IndexCount: 28056
		fin >> LABLE >> numBones;     // BoneCount: 56
		fin >> LABLE >> numAnimationClips;  // AnimationClips: 1

		if (eType == TEXTYPE_DYNAMIC)
		{
			cout << "된다 2" << endl;
			vector<D3DXMATRIX> boneOffsets;
			vector<int> boneIndexToParentIndex;
			map<std::string, AnimationClip> animations;

			ReadMeshDataFromSBX(fin, numVertices, numIndices, vertices, indices, i, eType);
			ReadBoneHierarchyFromSBX(fin, numBones, boneIndexToParentIndex, i);
			ReadOffsetMatrixFromSBX(fin, numBones, boneOffsets, i);
			ReadAnimationClipsFromSBX(fin, numBones, numAnimationClips, animations);

			skinInfo.Set(boneIndexToParentIndex, boneOffsets, animations);
		}
		else if (eType == TEXTYPE_STATIC)
		{
			cout << "된다 3" << endl;
			ReadMeshDataFromSBX(fin, numVertices, numIndices, vertices, indices, i, eType);
			//skinInfo.Set(boneIndexToParentIndex, boneOffsets, animations);
		}
	}

	fin.close();

	return true;
}