#include "Input.h"
#include "WinApp.h"
#include "cassert"

Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

void Input::Initialize()
{
    CreateDirectInputObj();

    SetInputData();

    SetExclusiveControlLevel();
}

void Input::Updata()
{
    // キーボード情報の取得開始
    keyborad->Acquire();
    // 前回の入力情報を取得
    memcpy(prevKey, key, sizeof(key));
    // 全キーの入力状態を取得
    keyborad->GetDeviceState(sizeof(key), key);
}

bool Input::TriggerKey(BYTE keycode)
{
    return (key[keycode] & 0x80) && !(prevKey[keycode] & 0x80);
}

bool Input::PushKey(BYTE keycode)
{
    return (key[keycode] & 0x80);
}

void Input::CreateDirectInputObj()
{
    HRESULT result = S_FALSE;
    WinApp* win = WinApp::GetInstance();

    result = DirectInput8Create(win->GetWC().hInstance,
        DIRECTINPUT_VERSION, IID_IDirectInput8,(void**)&directInput, nullptr);
    assert(SUCCEEDED(result));

    result = directInput->CreateDevice(GUID_SysKeyboard, &keyborad, NULL);
    assert(SUCCEEDED(result));
}

void Input::SetInputData()
{
    HRESULT result = S_FALSE;

    result = keyborad->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(result));
}

void Input::SetExclusiveControlLevel()
{
    HRESULT result = S_FALSE;
    WinApp* win = WinApp::GetInstance();

    result = keyborad->SetCooperativeLevel(
        win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

    assert(SUCCEEDED(result));
}
