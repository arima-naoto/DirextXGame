#pragma once
#include <vector>
#include "Vector3.h"

struct Vertex {
	Vector3 position;
	Vector3 normal;
};

class MeshData{
public:

	void LoadObj(const char* filePath);

	std::vector<Vertex> vertices_;
	std::vector<uint32_t> indices_;

};

