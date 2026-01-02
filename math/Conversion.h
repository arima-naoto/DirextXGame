#pragma once
#include "structure.h"
#include "Matrix4x4.h"
#include "Maths.h"

#include "DirectXCommon.h"

struct CbTransform {
	Matrix4x4 World;
	Matrix4x4 WVP;
};

/// <summary>
/// 座標変換用クラス
/// </summary>
class Conversion
{
public:

	void Initialize();

	void CreateConstBuffer();

	void Transform();

	void RotateCamera();

	void ZoomCamera();

	void DrawDebugText();

	ID3D12Resource* GetConstBuffer() { return constBuffer; }


private:

	WorldTransform local_;

	WorldTransform camera_;

	DirectXCommon* dxCommon_ = nullptr;

	CbTransform* mapMatrix_ = nullptr;

	ID3D12Resource* constBuffer = nullptr;

};



