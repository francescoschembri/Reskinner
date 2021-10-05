#pragma once

#include<glad/glad.h>

#include<reskinner/VBO.h>

class VAO
{
public:
	// ID reference for the Vertex Array Object
	unsigned int ID;
	// Constructor that generates a VAO ID
	VAO();

	// Links a VBO Attribute such as a position or color to the VAO
	void LinkAttribute(const VBO& VBO, const unsigned int layout, const unsigned int numComponents, const GLenum type, const int stride, const void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};