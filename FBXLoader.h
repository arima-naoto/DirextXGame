#pragma once
#include "vector"
#include "Vector3.h"


struct Vertex {
	Vector3 pos;
};

class FBXLoader
{
public:

	static void Load(const char* fileName, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

};

