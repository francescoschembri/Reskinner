#include <reskinner/TextureManager.h>

unsigned int TextureManager::TextureFromFile(const char* filename, const bool gamma) const
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 3)
		{
			if (gamma)
				format = GL_SRGB;
			else
				format = GL_RGB;
		}
		else if (nrComponents == 4) {
			if (gamma)
				format = GL_SRGB_ALPHA;
			else
				format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename << "\n";
		stbi_image_free(data);
	}

	return textureID;
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> TextureManager::loadMaterialTextures(const aiMaterial& mat, const aiTextureType& type, const std::string& typeName, const std::string& dir)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat.GetTextureCount(type); i++)
	{
		aiString str;
		mat.GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0 && std::strcmp(textures_loaded[j].dir.data(), dir.c_str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.path = std::string(str.C_Str());
			std::string filename = dir + "/" + texture.path;
			texture.dir = dir;
			texture.id = TextureFromFile(filename.c_str());
			textures_loaded.push_back(texture);
			textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void TextureManager::Clear() {
	textures_loaded.clear();
}

std::vector<Texture> TextureManager::GetTextures() {
	return textures_loaded;
}