#pragma once
#include "Mesh.h"
#include "vector"
#include "DirectXCommon.h"

class Model
{
public:
	void Load(const char* path);
	void Draw();

private:

	DirectXCommon* dxCommon_ = nullptr;

	std::vector<Mesh> meshes_;

};

