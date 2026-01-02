#include "DirectXCommon.h"
#include "cassert"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

const uint32_t kLinearRTVStart = 2;

// シングルトン
DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;
	return &instance;
}

// 初期化
void DirectXCommon::Initialize(WinApp* win, int32_t backBufferWidth, int32_t backBufferHeight) {

	// NULLポインタチェック
	assert(win && "WinAppクラスが見当たりません");

	win_ = win;

	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	InitializeDXGIDevice();
	InitializeCommand();
	CreateSwapChain();
	CreateFinalRenderTargets();
	CreateDepthBuffer();
	CreateFence();
}

// 描画前処理
void DirectXCommon::BeginDraw() {

	auto bbIdx = swapchain_->GetCurrentBackBufferIndex();
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIdx],
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	cmdList_->ResourceBarrier(1, &barrier);
	
	SetRenderTargets(true);

	ClearRenderTarget();

	ClearDepthBuffer();

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport =
		CD3DX12_VIEWPORT(0.0f, 0.0f, float(backBufferWidth_), float(backBufferHeight_));
	cmdList_->RSSetViewports(1, &viewport);
	// シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, backBufferWidth_, backBufferHeight_);
	cmdList_->RSSetScissorRects(1, &rect);
}

// 描画後処理
void DirectXCommon::EndDraw() {

	auto bbIdx = swapchain_->GetCurrentBackBufferIndex();
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(backBuffers_[bbIdx],
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	cmdList_->ResourceBarrier(1, &barrier);

	cmdList_->Close();

	ID3D12CommandList* cmdLists[] = { cmdList_.Get() };
	cmdQueue_->ExecuteCommandLists(1, cmdLists);

	cmdQueue_->Signal(fence.Get(), ++fenceVal);

	if (fence->GetCompletedValue() != fenceVal) {
		auto event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator_->Reset();
	cmdList_->Reset(cmdAllocator_.Get(), nullptr);
	swapchain_->Present(1, 0);
}

// デバイスのGetter
ID3D12Device* DirectXCommon::GetDevice() { return dev_.Get(); }

// グラフィックスコマンドリストのGetter
ID3D12GraphicsCommandList* DirectXCommon::GetCommandList() { return cmdList_.Get(); }

// Direct3D初期化
void DirectXCommon::InitializeDXGIDevice() {

	HRESULT result = S_FALSE;

#ifdef _DEBUG

	EnableDebugLayer();

#endif

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// Direct3Dデバイスの初期化
	D3D_FEATURE_LEVEL featureLevel;

	for (auto lv : levels) {
		if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&dev_)) == S_OK) {
			featureLevel = lv;
			break;
		}
	}

	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<IDXGIAdapter*>adapters;
	IDXGIAdapter* tmpAdapter = nullptr;

	for (int i = 0; dxgiFactory_->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
		adapters.push_back(tmpAdapter);
	}

	for (auto adpt : adapters) {
		DXGI_ADAPTER_DESC adesc = {};
		adpt->GetDesc(&adesc);

		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::string::npos) {
			tmpAdapter = adpt;
			break;
		}
	}

}

// コマンド初期化
void DirectXCommon::InitializeCommand() {

	HRESULT result = S_FALSE;

	result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator_));
	assert(SUCCEEDED(result));

	result = dev_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator_.Get(), nullptr, IID_PPV_ARGS(&cmdList_));
	assert(SUCCEEDED(result));

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = dev_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));
}

// スワップチェーン生成
void DirectXCommon::CreateSwapChain() {

	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = backBufferWidth_;
	swapchainDesc.Height = backBufferHeight_;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;

	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	result = dxgiFactory_->CreateSwapChainForHwnd(cmdQueue_.Get(), win_->GetHwnd(), &swapchainDesc,
		nullptr, nullptr, (IDXGISwapChain1**)swapchain_.ReleaseAndGetAddressOf());
	assert(SUCCEEDED(result));

}

void DirectXCommon::CreateFinalRenderTargets() {
	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = 2;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	result = dev_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps_));
	assert(SUCCEEDED(result));

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain_->GetDesc(&swcDesc);

	// ★ SRGBで固定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	backBuffers_.resize(swcDesc.BufferCount);
	D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps_->GetCPUDescriptorHandleForHeapStart();

	for (int idx = 0; idx < backBuffers_.size(); idx++) {

		result = swapchain_->GetBuffer(idx, IID_PPV_ARGS(&backBuffers_[idx]));
		assert(SUCCEEDED(result));

		// ✅ ここでSRGBのままRTV作成
		dev_->CreateRenderTargetView(backBuffers_[idx], &rtvDesc, handle);

		handle.ptr += dev_->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
}

void DirectXCommon::EnableDebugLayer() {
	ID3D12Debug* debugLayer = nullptr;
	auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

	debugLayer->EnableDebugLayer();
	debugLayer->Release();

}

void DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, backBufferWidth_, backBufferHeight_, 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = dev_->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&depthBuffer_));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = dev_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev_->CreateDepthStencilView(
		depthBuffer_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	result = dev_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(result));

}

void DirectXCommon::SetRenderTargets(bool sRGB)
{
	auto bbIndex = swapchain_->GetCurrentBackBufferIndex();
	auto rtvIndex = sRGB ? bbIndex : bbIndex + kLinearRTVStart;

	auto rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), rtvIndex,
		dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	auto dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());

	cmdList_->OMSetRenderTargets(1, &rtvH, false, &dsvH);
}

void DirectXCommon::ClearRenderTarget(){

	auto bbIndex = swapchain_->GetCurrentBackBufferIndex();
	auto rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		rtvHeaps_->GetCPUDescriptorHandleForHeapStart(), bbIndex,
		dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };
	cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer(){

	auto dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());

	cmdList_->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

size_t DirectXCommon::GetBackBufferCount() { return backBuffers_.size(); }
