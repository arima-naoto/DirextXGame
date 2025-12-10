#pragma once
#include <vector>
#include <string>
#include "Vector2.h"
#include "Vector3.h"

struct ObjVertex {
    Vector3 pos;
    Vector3 normal;
    Vector2 uv;
};

class ObjMesh {
public:
    std::vector<ObjVertex> vertices;
    std::vector<unsigned int> indices;
};

class ObjLoader {
public:
    static bool Load(const std::string& filename, ObjMesh& mesh);
};
