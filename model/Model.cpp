#include "Model.h"

void Model::Load(const char* path)
{
	dxCommon_ = DirectXCommon::GetInstance();
	ID3D12Device* device = dxCommon_->GetDevice();

	MeshData data;
	data.LoadObj(path);

	Mesh mesh;
	mesh.Create(device, data);
	meshes_.push_back(mesh);
}

void Model::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	for (auto& mesh : meshes_) {
		mesh.Draw(cmdList);
	}
}
