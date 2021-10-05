#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <reskinner/Bone.h>
#include <reskinner/BoneInfo.h>
#include <reskinner/Model.h>

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int parentIndex;
};

class Animation
{
public:
	std::vector<AssimpNodeData> transformations;

	Animation(const std::string& animationPath, Model& model);

	glm::mat4 GetBoneTransform(const AssimpNodeData& node, float currentTime);

	float GetTicksPerSecond() const;
	float GetDuration() const;

private:
	void ReadHeirarchyData(const aiNode* src, const int parentIndex);
	void ReadBonesData(const aiAnimation* animation, Model& model);

	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	Model& model;
};