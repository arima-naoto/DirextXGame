#pragma once
#define DURECTINPUT_VERSION 0x0800;
#include "dinput.h"
#include "Xinput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"xinput.lib")

class Input {
public:

	/// <summary>
	/// シングルトン
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Updata();

	/// <summary>
	/// キーが押された瞬間
	/// </summary>
	/// <param name="keycode">キー番号</param>
	/// <returns></returns>
	bool TriggerKey(BYTE keycode);

	/// <summary>
	/// キーが押されている間
	/// </summary>
	/// <param name="keycode">キー番号</param>
	/// <returns></returns>
	bool PushKey(BYTE keycode);

private:

	Input() = default;
	~Input() = default;
	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;

	void CreateDirectInputObj();

	void SetInputData();

	void SetExclusiveControlLevel();

private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyborad = nullptr;

	BYTE key[256] = {};
	BYTE prevKey[256] = {};

};