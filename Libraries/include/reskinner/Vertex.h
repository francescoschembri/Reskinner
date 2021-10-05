#pragma once

#include<glm/glm.hpp>
#include<reskinner/VertexBoneData.h>

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    // texCoords
    glm::vec2 TexCoords = glm::vec2(0.0f, 0.0f);
    // info about which bones influences and how much the vertex, if there are any bone
    VertexBoneData BoneData;
};