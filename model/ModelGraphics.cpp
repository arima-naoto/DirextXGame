#include "ModelGraphics.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

void ModelGraphics::Initialize(ID3D12Device* device) {
	device_ = device;
	CreatePipeline();
}

void ModelGraphics::PreDraw(ID3D12GraphicsCommandList* cmdList) {
	cmdList->SetPipelineState(pipelineState_);
	cmdList->SetGraphicsRootSignature(rootSignature_);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelGraphics::CreatePipeline(){

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	D3DCompileFromFile(L"shader/ModelVS.hlsl",nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main","vs_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,&vsBlob,&errorBlob);

	D3DCompileFromFile(L"shader/ModelPS.hlsl",nullptr,D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main","ps_5_0",D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,&psBlob,&errorBlob);

	D3D12_ROOT_PARAMETER rootParams[1]{};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[0].Descriptor.ShaderRegister = 0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc{};
	rsDesc.pParameters = rootParams;
	rsDesc.NumParameters = 1;
	rsDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* rsBlob = nullptr;
	D3D12SerializeRootSignature(&rsDesc,D3D_ROOT_SIGNATURE_VERSION_1,&rsBlob,&errorBlob);

	device_->CreateRootSignature(0,rsBlob->GetBufferPointer(),rsBlob->GetBufferSize(),IID_PPV_ARGS(&rootSignature_));

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,0, 0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"NORMAL", 0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.pRootSignature = rootSignature_;
	psoDesc.VS = { vsBlob->GetBufferPointer(), vsBlob->GetBufferSize() };
	psoDesc.PS = { psBlob->GetBufferPointer(), psBlob->GetBufferSize() };
	psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };

	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	/* ===== Rasterizer（★ここ）===== */
	auto raster = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	raster.FillMode = D3D12_FILL_MODE_SOLID; // ← ワイヤーフレーム
	raster.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState = raster;
	/* ===== Blend ===== */
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	/* ===== Depth（★ここ）===== */
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = FALSE; // ★Depth無効
	psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;

	/* ===== PSO 作成 ===== */
	device_->CreateGraphicsPipelineState(
		&psoDesc,
		IID_PPV_ARGS(&pipelineState_)
	);
}
