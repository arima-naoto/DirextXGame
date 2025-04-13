#include "WinApp.h"

// ウィンドウプロシージャの定義
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

// シングルトンの定義
WinApp* WinApp::GetInstance()
{
	// TODO: return ステートメントをここに挿入します
	static WinApp instance;
	return &instance;
}

// ゲーム画面生成の定義
void WinApp::CreateGameWindow(const std::wstring& title, int windowWidth, int windowHeight) {

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.lpszClassName = (L"DirectXGame");
	wc.hInstance = GetModuleHandle(nullptr);

	RegisterClassEx(&wc);

	RECT wrc = { 0,0,windowWidth,windowHeight };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(wc.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left,
		wrc.bottom - wrc.top, nullptr, nullptr, wc.hInstance, nullptr);

	ShowWindow(hwnd, SW_SHOW);

}

// メッセージプロセスの定義
bool WinApp::ProcessMessage()
{

	MSG msg = {};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

// 登録解除
void WinApp::TerminateGameWindow() {
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

WNDCLASSEX& WinApp::GetWC() {
	return wc;
}

HWND& WinApp::GetHwnd()
{
	// TODO: return ステートメントをここに挿入します
	return hwnd;
}
