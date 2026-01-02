#pragma once
#include "Mesh.h"
#include "vector"
#include "DirectXCommon.h"
#include "Matrix4x4.h"

#include "Conversion.h"

class Model
{
public:
	void Load(const char* path);

	void Updata();

	void Draw();

private:

	DirectXCommon* dxCommon_ = nullptr;

	std::vector<Mesh> meshes_;

	Conversion* conversion_ = nullptr;
};

