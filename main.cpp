#include "WinApp.h"
#include "DirectXCommon.h"

#include "d3dcompiler.h"
#pragma comment(lib,"d3dcompiler.lib")
#include "vector"

#include "DirectXTex.h"
#pragma comment(lib,"DirectXTex.lib")
using namespace DirectX;

#include "Input.h"
#include "ImGuiManager.h"

#include "Renderer.h"
#include "Model.h"
#include "Conversion.h"

size_t AlignmentedSize(size_t size, size_t alignment) {
	return size + alignment - size % alignment;
}

// Windowアプリのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	Renderer renderer;
	Model model;

	win = WinApp::GetInstance();
	win->CreateGameWindow();

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	renderer.Initialize(dxCommon->GetDevice());
	model.Load("Resources/Box.obj");

	input = Input::GetInstance();
	input->Initialize();
	
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	Conversion conversion;

	conversion.Initialize();
	conversion.CreateConstBuffer();

	while (true) {
		if (win->ProcessMessage()) {
			break;
		}

		imguiManager->Begin();

		input->Updata();

		conversion.Transform();

		imguiManager->End();

		dxCommon->BeginDraw();

		ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

		renderer.PreDraw(cmdList);

		model.Draw();

		imguiManager->Draw();

		dxCommon->EndDraw();

		if (input->TriggerKey(DIK_ESCAPE)) {
			return -1;
		}
	}

	imguiManager->Finalize();
	win->TerminateGameWindow();
	return 0;
}