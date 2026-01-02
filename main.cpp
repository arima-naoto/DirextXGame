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

#include "ModelGraphics.h"
#include "Model.h"

// Windowアプリのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;

	win = WinApp::GetInstance();
	win->CreateGameWindow();

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

	input = Input::GetInstance();
	input->Initialize();
	
	ModelGraphics graphics;
	graphics.Initialize(dxCommon->GetDevice());

	Model model;
	model.Load("Resources/Box.obj");

	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	while (true) {
		if (win->ProcessMessage()) {
			break;
		}

		imguiManager->Begin();

		input->Updata();

		model.Updata();

		imguiManager->End();


		dxCommon->BeginDraw();

		ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

		graphics.PreDraw(cmdList);

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