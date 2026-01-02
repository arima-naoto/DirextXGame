#include "Mesh.h"

void Mesh::Create(ID3D12Device* device, const MeshData& data)
{
	bool use32Bit = data.vertices_.size() > 65535;
	indexCount_ = (uint32_t)data.indices_.size();

	// VertexBuffer
	UINT vbSize = UINT(sizeof(Vertex) * data.vertices_.size());

	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vbDesc =
		CD3DX12_RESOURCE_DESC::Buffer(vbSize);

	device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&vbDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer_)
	);

	Vertex* vbData = nullptr;
	vertexBuffer_->Map(0, nullptr, (void**)&vbData);
	memcpy(vbData, data.vertices_.data(), vbSize);
	vertexBuffer_->Unmap(0, nullptr);

	vbView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = vbSize;
	vbView_.StrideInBytes = sizeof(Vertex);

	// IndexBuffer
	UINT ibSize = UINT(sizeof(uint32_t) * data.indices_.size());
	CD3DX12_RESOURCE_DESC ibDesc =
		CD3DX12_RESOURCE_DESC::Buffer(ibSize);

	device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&ibDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer_)
	);

	uint32_t* ibData = nullptr;
	indexBuffer_->Map(0, nullptr, (void**)&ibData);
	memcpy(ibData, data.indices_.data(), ibSize);
	indexBuffer_->Unmap(0, nullptr);

	ibView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	ibView_.SizeInBytes = ibSize;
	ibView_.Format = DXGI_FORMAT_R32_UINT;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	cmdList->IASetIndexBuffer(&ibView_);
	cmdList->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}
