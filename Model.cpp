#include "Model.h"
#include <algorithm>

Model::Model()
{
    transform_.scale = {1,1,1};
    transform_.rotate = {0,0,0};
    transform_.translate = {0,0,0};
}

Model::~Model()
{
    if (vertBuff_)  vertBuff_->Release();
    if (idxBuff_)   idxBuff_->Release();
}

bool Model::LoadFromOBJ(const std::string& filename)
{
    return ObjLoader::Load(filename, mesh_);
}

void Model::CreateBuffers(ID3D12Device* device)
{
    //------------- 頂点バッファ作成 ---------------
    vbSize_ = sizeof(ObjVertex) * mesh_.vertices.size();

    D3D12_HEAP_PROPERTIES heapProp = {};
    heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC resDesc = {};
    resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resDesc.Width = vbSize_;
    resDesc.Height = 1;
    resDesc.DepthOrArraySize = 1;
    resDesc.MipLevels = 1;
    resDesc.SampleDesc.Count = 1;
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vertBuff_)
    );

    ObjVertex* vertMap = nullptr;
    vertBuff_->Map(0, nullptr, (void**)&vertMap);
    std::copy(mesh_.vertices.begin(), mesh_.vertices.end(), vertMap);
    vertBuff_->Unmap(0, nullptr);

    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = (UINT)vbSize_;
    vbView_.StrideInBytes = sizeof(ObjVertex);

    //------------- インデックスバッファ作成 ---------------
    ibSize_ = sizeof(unsigned int) * mesh_.indices.size();
    resDesc.Width = ibSize_;

    device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&idxBuff_)
    );

    unsigned int* idxMap = nullptr;
    idxBuff_->Map(0, nullptr, (void**)&idxMap);
    std::copy(mesh_.indices.begin(), mesh_.indices.end(), idxMap);
    idxBuff_->Unmap(0, nullptr);

    ibView_.BufferLocation = idxBuff_->GetGPUVirtualAddress();
    ibView_.Format = DXGI_FORMAT_R32_UINT;
    ibView_.SizeInBytes = (UINT)ibSize_;
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList)
{
    cmdList->IASetVertexBuffers(0, 1, &vbView_);
    cmdList->IASetIndexBuffer(&ibView_);
    cmdList->DrawIndexedInstanced((UINT)mesh_.indices.size(), 1, 0, 0, 0);
}