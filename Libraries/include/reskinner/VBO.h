#pragma once

#include<glad/glad.h>
#include<vector>

#include<reskinner/Vertex.h>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	unsigned int ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(const std::vector<Vertex>& vertices);
	// Binds the VBO
	void Bind() const;
	// Unbinds the VBO
	void Unbind() const;
	// Deletes the VBO
	void Delete();
};