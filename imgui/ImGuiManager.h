#pragma once

#include "imgui.h"
#include "d3d12.h"
#include "wrl.h"

using Microsoft::WRL::ComPtr;

// 前方宣言
class WinApp;
class DirectXCommon;

class ImGuiManager
{
public:

	static ImGuiManager* GetInstance();
	void Initialize(WinApp* win, DirectXCommon* dxCommon);
	void Finalize();
	void Begin();
	void End();
	void Draw();

private:

	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager& obj) = delete;
	ImGuiManager& operator=(const ImGuiManager& obj) = delete;

private:

#ifdef _DEBUG
	DirectXCommon* dxCommon_ = nullptr; 
	ComPtr<ID3D12DescriptorHeap> srvHeap_;
#endif // _DEBUG


};

