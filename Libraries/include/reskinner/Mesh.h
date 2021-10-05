#pragma once

#include<vector>

#include <glad/glad.h> // holds all OpenGL type declarations

#include <reskinner/Texture.h>
#include <reskinner/Face.h>
#include <reskinner/Vertex.h>

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex>&& vertices, std::vector<Face>&& faces, std::vector<Texture>&& textures);
	int GetVAO() const;
	Mesh Bake(const std::vector<glm::mat4>& matrices) const;

private:
	// render data 
	unsigned int VAO, VBO, EBO;

	// initializes all the buffer objects/arrays
	void setupMesh();
};