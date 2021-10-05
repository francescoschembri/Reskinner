#pragma once

#include<glad/glad.h>
#include<vector>

#include<reskinner/Face.h>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	unsigned int ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(const std::vector<Face>& faces);

	// Binds the EBO
	void Bind() const;
	// Unbinds the EBO
	void Unbind() const;
	// Deletes the EBO
	void Delete();
};