#include <reskinner/Animation.h>


Animation::Animation(const std::string& animationPath, Model& m) : model(m)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	m_Duration = animation->mDuration;
	m_TicksPerSecond = animation->mTicksPerSecond;
	aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
	globalTransformation = globalTransformation.Inverse();
	ReadHeirarchyData(scene->mRootNode, -1);
	ReadBonesData(animation, model);
}

glm::mat4 Animation::GetBoneTransform(const AssimpNodeData& node, float currentTime)
{
	auto iter = std::find_if(std::begin(m_Bones), std::end(m_Bones), [&node](const auto& bone) {
			return bone.GetBoneName() == node.name;
		});
	if (iter == std::end(m_Bones)) return node.transformation;
	iter->Update(currentTime);
	return iter->GetLocalTransform() * iter->GetOffset();
}

float Animation::GetTicksPerSecond() const { return m_TicksPerSecond; }
float Animation::GetDuration() const { return m_Duration; }

void Animation::ReadHeirarchyData(const aiNode* src, const int parentIndex)
{
	assert(src);

	AssimpNodeData dest;
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.parentIndex = parentIndex;
	transformations.push_back(dest);

	int index = transformations.size() - 1;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		ReadHeirarchyData(src->mChildren[i], index);
	}
}

void Animation::ReadBonesData(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		
		const BoneInfo& info = model.AddBoneInfo(std::move(boneName), std::move(glm::mat4(1.0f)));

		Bone b = Bone(channel->mNodeName.data, info.id, channel, info.offset);
		m_Bones.push_back(b);
	}
}
