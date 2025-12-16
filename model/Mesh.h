#pragma once
#include "MeshData.h"
#include "d3d12.h"
#include "d3dx12.h"

class Mesh{
public:
	void Create(ID3D12Device* device, const MeshData& data);
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	ID3D12Resource* vertexBuffer_ = nullptr;
	ID3D12Resource* indexBuffer_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView_{};
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	uint32_t indexCount_ = 0;
};

