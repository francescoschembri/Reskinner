#pragma once

#include <iostream>
#include <vector>

#include <assimp/scene.h>
#include <glad/glad.h> 
#include <stb/stb_image.h>

#include <reskinner/Texture.h>

class TextureManager {
public:
	std::vector<Texture> loadMaterialTextures(const aiMaterial& mat, const aiTextureType& type, const std::string& typeName, const std::string& dir);
	std::vector<Texture> GetTextures();
	void Clear();

private:
	std::vector<Texture> textures_loaded;

	unsigned int TextureFromFile(const char* filename, const bool gamma = false) const;
};