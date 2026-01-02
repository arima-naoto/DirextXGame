#include "MeshData.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "cassert"

using namespace Assimp;

void MeshData::LoadObj(const char* filePath)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |
        aiProcess_GenNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_ConvertToLeftHanded
    );

    assert(scene && scene->HasMeshes());

    aiMesh* mesh = scene->mMeshes[0];

    // 頂点
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        Vertex v{};

        v.position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        if (mesh->HasNormals()) {
            v.normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };
        }
        else {
            v.normal = { 0.0f, 1.0f, 0.0f };
        }

        vertices_.push_back(v);
    }

    // インデックス
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);

        indices_.push_back(face.mIndices[0]);
        indices_.push_back(face.mIndices[1]);
        indices_.push_back(face.mIndices[2]);
    }
}
