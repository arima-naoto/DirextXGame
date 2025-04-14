#include "ImGuiManager.h"

#ifdef _DEBUG
#include "DirectXCommon.h"
#include "WinApp.h"
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#endif

ImGuiManager* ImGuiManager::GetInstance()
{
    static ImGuiManager instance;
    return &instance;
}

void ImGuiManager::Initialize(WinApp* win, DirectXCommon* dxCommon)
{
#ifdef _DEBUG
    dxCommon_ = dxCommon;

    HRESULT result = S_FALSE;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    result = dxCommon_->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvHeap_));
    assert(SUCCEEDED(result));

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(win->GetHwnd());
    ImGui_ImplDX12_Init(
        dxCommon_->GetDevice(), static_cast<int>(dxCommon_->GetBackBufferCount()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, srvHeap_.Get(),
        srvHeap_->GetCPUDescriptorHandleForHeapStart(),
        srvHeap_->GetGPUDescriptorHandleForHeapStart());

#endif // _DEBUG

}

void ImGuiManager::Finalize() {
#ifdef _DEBUG
	// 後始末
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// デスクリプタヒープを解放
	srvHeap_.Reset();
#endif
}

void ImGuiManager::Begin() {
#ifdef _DEBUG
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

void ImGuiManager::End() {
#ifdef _DEBUG
	// 描画前準備
	ImGui::Render();
#endif
}

void ImGuiManager::Draw() {
#ifdef _DEBUG
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif
}
