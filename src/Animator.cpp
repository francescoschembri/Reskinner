#include  <reskinner/Animator.h>


Animator::Animator(Animation& animation) : m_CurrentAnimation(&animation)
{
	m_CurrentTime = 0.0f;

	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(const float dt)
{
	if (m_CurrentAnimation)
	{
		m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
		CalculateBoneTransforms();
	}
}

void Animator::PlayAnimation(Animation& pAnimation, bool add)
{
	m_CurrentAnimation = &pAnimation;
	m_CurrentTime = 0.0f;
}

void Animator::CalculateBoneTransforms()
{
	for (unsigned int i = 0; i < m_CurrentAnimation->transformations.size(); i++) {
		AssimpNodeData t = m_CurrentAnimation->transformations[i];
		glm::mat4 nodeTransform = m_CurrentAnimation->GetBoneTransform(t, m_CurrentTime);

		if (t.parentIndex >= 0) {
			nodeTransform = m_FinalBoneMatrices[t.parentIndex] * nodeTransform;
		}
		m_FinalBoneMatrices[i] = nodeTransform;
	}
}

const std::vector<glm::mat4>& Animator::GetFinalBoneMatrices() const
{
	return m_FinalBoneMatrices;
}
