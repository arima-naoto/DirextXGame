#pragma once
#include "Windows.h"
#include "string"

/// <summary>
/// ウィンドウアプリケーションクラス
/// </summary>
class WinApp {
public:

	static const int window_width = 1280;
	static const int window_height = 720;

public:// メンバ関数

	static WinApp* GetInstance();
	void CreateGameWindow(const std::wstring& title = L"DirectXGame", int windowWidth = window_width, int windowHeight = window_height);
	bool ProcessMessage();
	void TerminateGameWindow();

	WNDCLASSEX& GetWC();

	HWND& GetHwnd();

private:

	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp& obj) = delete;
	WinApp& operator=(const WinApp& obj) = delete;

private:

	WNDCLASSEX wc = {};

	HWND hwnd = {};

};

