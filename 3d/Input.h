#pragma once
#define DURECTINPUT_VERSION 0x0800;
#include "dinput.h"
#include "Xinput.h"
#include "wrl.h"
#include <cstdint>

#include "Vector2.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"xinput.lib")

using namespace Microsoft::WRL;

class Input {
public:

	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

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

	bool IsPressMouse(int32_t mouseNumber);

	bool IsTriggerMouse(int32_t mouseNumber);

	MouseMove GetMouseMove();

	int32_t GetWheel();

	const Vector2Int GetMousePosition() const;
	

private:

	Input() = default;
	~Input() = default;
	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;

	void CreateDirectInputObj();

	void SetInputData();

	void SetExclusiveControlLevel();

	

private:

	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyborad = nullptr;
	ComPtr<IDirectInputDevice8> devMouse = nullptr;

	BYTE key[256] = {};
	BYTE prevKey[256] = {};
	DIMOUSESTATE2 mouse;
	DIMOUSESTATE2 mousePre;

};