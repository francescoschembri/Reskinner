#pragma once

constexpr int MAX_BONE_INFLUENCE = 4;

struct VertexBoneData {
    // number of bones affecting the vertex
    int numBones = 0;
    // bone indexes which will influence the vertex
    int BoneIDs[MAX_BONE_INFLUENCE] = {-1};
    // weights from each bone
    float Weights[MAX_BONE_INFLUENCE] = {0.0f};
};