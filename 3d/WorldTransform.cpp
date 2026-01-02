#include "WorldTransform.h"

void World::Initialize(){

#pragma region 関数の呼び出し

	CreateConstBuffer();
	Map();

#pragma endregion 

}

void World::CreateConstBuffer()
{
	HRESULT result = S_FALSE;

	dxCommon_ = DirectXCommon::GetInstance();
	ID3D12Device* dev = dxCommon_->GetDevice();

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(CbTransform) + 0xff) & ~0xff);

	dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuffer_));
}

void World::Map()
{
	HRESULT result = S_FALSE;

	result = constBuffer_->Map(0, nullptr, (void**)&MapMatrix_);
	MapMatrix_->World = Maths::IdentityMatrix();
	MapMatrix_->WVP = Maths::IdentityMatrix();
}
