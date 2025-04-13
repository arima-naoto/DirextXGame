#pragma once
#include "WinApp.h"
#include "d3d12.h"
#include "dxgi1_6.h"
#include "wrl.h"

#include "vector"

using Microsoft::WRL::ComPtr;

class DirectXCommon
{
public:

	static DirectXCommon* GetInstance();

	void Initialize(WinApp* win, int32_t backBufferWidth = WinApp::window_width, int32_t backBufferHeight = WinApp::window_height);

	void BeginDraw();

	void EndDraw();

	ID3D12Device* GetDevice();

	ID3D12GraphicsCommandList* GetCommandList();

private:

	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon& obj) = delete;
	DirectXCommon& operator=(const DirectXCommon& obj) = delete;

	void InitializeDXGIDevice();
	void InitializeCommand();
	void CreateSwapChain();
	void CreateFinalRenderTargets();
	void EnableDebugLayer();
	void CreateFence();

private:

	WinApp* win_ = nullptr;

	ComPtr<ID3D12Device> dev_ = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory_ = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList_ = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue_ = nullptr;
	ComPtr<IDXGISwapChain4> swapchain_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps = nullptr;
	std::vector<ID3D12Resource*> backBuffers_;
	ComPtr<ID3D12Fence> fence = nullptr;

	int32_t backBufferWidth_ = 0;
	int32_t backBufferHeight_ = 0;
	UINT64 fenceVal = 0;


};

