#include "MeshData.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "cassert"

using namespace Assimp;

void MeshData::LoadObj(const char* failPath)
{
	Importer importer;

	const aiScene* scene = importer.ReadFile(failPath,
		aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

	assert(scene && scene->HasMeshes());

	aiMesh* mesh = scene->mMeshes[0];

	//頂点
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex v{};

		v.position = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};

		v.normal = {
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		};

		vertices_.push_back(v);
	}

	//インデックス
	for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
		for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			indices_.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}
