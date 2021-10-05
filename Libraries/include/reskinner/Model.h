#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <reskinner/Mesh.h>
#include <reskinner/Shader.h>
#include <reskinner/TextureManager.h>
#include <reskinner/AssimpGLMHelpers.h>
#include <reskinner/BoneInfo.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class Model
{
public:
	// model data 
	std::vector<Mesh> meshes;

	// constructor, expects a filepath to a 3D model.
	Model(const std::string& path, TextureManager& texMan);
	// constructor, expects a vector of Meshes.
	Model(std::vector<Mesh>&& meshes);
	// bake the model in the current pos
	Model Bake(const std::vector<glm::mat4>& matrices) const;

	const BoneInfo& AddBoneInfo(std::string&& name, glm::mat4&& offset);

private:
	std::map<std::string, BoneInfo> boneInfoMap;
	int numBones = 0;

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(const std::string& path, TextureManager& texMan);
	// processes a node. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene, const std::string& dir, TextureManager& texMan);
	// processes a mesh.
	Mesh processMesh(const aiMesh* mesh, const aiScene* scene, TextureManager& texMan, const std::string& dir);
	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(Vertex& vertex, const int boneID, const float weight);
};