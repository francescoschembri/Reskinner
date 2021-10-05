#pragma once

/* Container for bone data */

#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <reskinner/AssimpGLMHelpers.h>

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, const int ID, const aiNodeAnim* channel, const glm::mat4& offset);
	void Update(const float animationTime);
	const glm::mat4& GetLocalTransform() const;
	const glm::mat4& GetOffset() const;
	std::string GetBoneName() const;
	int GetBoneID() const;
	int GetPositionIndex(const float animationTime) const;
	int GetRotationIndex(const float animationTime) const;
	int GetScaleIndex(const float animationTime) const;


private:

	float GetScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime) const;
	glm::mat4 InterpolatePosition(const float animationTime) const;
	glm::mat4 InterpolateRotation(const float animationTime) const;
	glm::mat4 InterpolateScaling(const float animationTime) const;

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	glm::mat4 offset;
	std::string m_Name;
	int m_ID;
};

