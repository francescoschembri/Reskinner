#include <reskinner/Model.h>

Model::Model(const std::string& path, TextureManager& texMan)
{
	loadModel(path, texMan);
}

Model::Model(std::vector<Mesh>&& meshes)
{
	this->meshes = std::move(meshes);
}

Model Model::Bake(const std::vector<glm::mat4>& matrices) const
{
	std::vector<Mesh> meshes;
	meshes.reserve(this->meshes.size());
	for (const Mesh& m : this->meshes) {
		meshes.push_back(m.Bake(matrices));
	}
	return Model(std::move(meshes));
}

void Model::loadModel(const std::string& path, TextureManager& texMan) {
	meshes.clear();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// retrieve the directory path of the filepath
	std::string directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	processNode(scene->mRootNode, scene, directory, texMan);
}

void Model::processNode(aiNode* node, const aiScene* scene, const std::string& dir, TextureManager& texMan) {
	meshes.reserve(scene->mNumMeshes);

	// process each mesh located at the current node
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		meshes.push_back(processMesh(scene->mMeshes[i], scene, texMan, dir));
	}
}

Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene, TextureManager& texMan, const std::string& dir) {
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
		vertex.Tangent = AssimpGLMHelpers::GetGLMVec(mesh->mTangents[i]);
		vertex.Bitangent = AssimpGLMHelpers::GetGLMVec(mesh->mBitangents[i]);

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		Face f;
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			f.indices[j] = face.mIndices[j];
		}
		faces.push_back(f);
	}
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<Texture> diffuseMaps = texMan.loadMaterialTextures(*material, aiTextureType_DIFFUSE, "texture_diffuse", dir);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = texMan.loadMaterialTextures(*material, aiTextureType_SPECULAR, "texture_specular", dir);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	std::vector<Texture> normalMaps = texMan.loadMaterialTextures(*material, aiTextureType_HEIGHT, "texture_height", dir);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	std::vector<Texture> heightMaps = texMan.loadMaterialTextures(*material, aiTextureType_NORMALS, "texture_normal", dir);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	if (mesh->HasBones())
		ExtractBoneWeightForVertices(vertices, mesh, scene);

	return Mesh(std::move(vertices), std::move(faces), std::move(textures));
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = numBones;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = numBones++;
		}
		else
		{
			boneID = boneInfoMap[boneName].id;
		}
		assert(boneID != -1);
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;
		
		for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
		{
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

void Model::SetVertexBoneData(Vertex& vertex, const int boneID, const float weight) {
	if (vertex.BoneData.numBones >= MAX_BONE_INFLUENCE)
		return;
	vertex.BoneData.BoneIDs[vertex.BoneData.numBones] = boneID;
	vertex.BoneData.Weights[vertex.BoneData.numBones++] = weight;
}

const BoneInfo& Model::AddBoneInfo(std::string&& name, glm::mat4&& offset)
{
	if (boneInfoMap.find(name) == boneInfoMap.end())
	{
		BoneInfo info;
		info.id = numBones++;
		info.offset = std::move(offset);
		boneInfoMap[std::move(name)] = info;
		return info;
	}
	else {
		return boneInfoMap[name];
	}
}