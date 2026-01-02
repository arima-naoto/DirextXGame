#pragma once
#include "DirectXCommon.h"

class ModelGraphics {
public:
	void Initialize(ID3D12Device* device);
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	ID3D12RootSignature* GetRootSignature() const { return rootSignature_; }

private:
	void CreatePipeline();

	ID3D12Device* device_ = nullptr;

	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12PipelineState* pipelineState_ = nullptr;
};
