#include<reskinner/Mesh.h>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<Face>&& faces, std::vector<Texture>&& textures)
{
	this->vertices = std::move(vertices);
	this->faces = std::move(faces);
	this->textures = std::move(textures);

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
	setupMesh();
}

int Mesh::GetVAO() const
{
    return VAO;
}

Mesh Mesh::Bake(const std::vector<glm::mat4>& matrices) const
{
    std::vector<Vertex> v = vertices;
    std::vector<Texture> t = textures;
    std::vector<Face> f = faces;
    return Mesh(std::move(v), std::move(f), std::move(t));
}


void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(Face), &faces[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex tangent
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    // vertex texture coords
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // num of bones
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 1, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneData.numBones));
    // bone ids
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BoneData.BoneIDs));
    // weights
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BoneData.Weights));
    glBindVertexArray(0);
}


