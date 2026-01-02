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

	conversion_ = new Conversion();
	conversion_->Initialize();
	conversion_->CreateConstBuffer();
}

void Model::Updata()
{
	conversion_->DrawDebugText();
}

void Model::Draw()
{
	conversion_->Transform();

	ID3D12GraphicsCommandList* cmdList = dxCommon_->GetCommandList();

	cmdList->SetGraphicsRootConstantBufferView(
		0,
		conversion_->GetConstBuffer()->GetGPUVirtualAddress()
	);

	for (auto& mesh : meshes_) {
		mesh.Draw(cmdList);
	}
}
