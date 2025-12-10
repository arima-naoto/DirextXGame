#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "ObjLoader.h"
#include "Maths.h"
#include <d3d12.h>

class Model
{
public:
    Model();
    ~Model();

    bool LoadFromOBJ(const std::string& filename);

    void CreateBuffers(ID3D12Device* device);
    void Draw(ID3D12GraphicsCommandList* cmdList);

    void SetTransform(const WorldTransform& wt) { transform_ = wt; }
    const WorldTransform& GetTransform() const { return transform_; }

private:
    ObjMesh mesh_;

    ID3D12Resource* vertBuff_ = nullptr;
    ID3D12Resource* idxBuff_ = nullptr;

    D3D12_VERTEX_BUFFER_VIEW vbView_{};
    D3D12_INDEX_BUFFER_VIEW ibView_{};

    size_t vbSize_ = 0;
    size_t ibSize_ = 0;

    WorldTransform transform_;
};

