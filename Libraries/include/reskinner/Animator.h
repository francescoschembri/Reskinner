#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <reskinner/Animation.h>
#include <reskinner/Bone.h>

class Animator
{
public:
	Animator(Animation& animation);
	void UpdateAnimation(const float dt);
	void PlayAnimation(Animation& pAnimation, bool add = false);
	void CalculateBoneTransforms();
	const std::vector<glm::mat4>& GetFinalBoneMatrices() const;

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation* m_CurrentAnimation;
	float m_CurrentTime;
};

