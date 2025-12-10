#include "ObjLoader.h"
#include <fstream>
#include <sstream>

bool ObjLoader::Load(const std::string& filename, ObjMesh& mesh) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;

    std::string line;
    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            Vector3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        }
        else if (prefix == "vt") {
            Vector2 uv;
            ss >> uv.x >> uv.y;
            uv.y = 1.0f - uv.y;    // DirectX 補正
            uvs.push_back(uv);
        }
        else if (prefix == "vn") {
            Vector3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (prefix == "f") {

            // f v/t/n v/t/n v/t/n
            for (int i = 0; i < 3; i++) {
                std::string vertStr;
                ss >> vertStr;

                std::stringstream vs(vertStr);
                std::string idx;
                int vIdx = 0, uvIdx = 0, nIdx = 0;

                std::getline(vs, idx, '/');
                vIdx = std::stoi(idx) - 1;

                std::getline(vs, idx, '/');
                uvIdx = std::stoi(idx) - 1;

                std::getline(vs, idx, '/');
                nIdx = std::stoi(idx) - 1;

                ObjVertex vert;
                vert.pos = positions[vIdx];
                vert.uv = uvs[uvIdx];
                vert.normal = normals[nIdx];

                mesh.vertices.push_back(vert);
                mesh.indices.push_back((unsigned int)mesh.indices.size());
            }
        }
    }

    return true;
}