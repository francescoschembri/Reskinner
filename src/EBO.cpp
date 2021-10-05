#include <reskinner/EBO.h>

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(const std::vector<Face>& faces)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), faces.data(), GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}