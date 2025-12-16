#pragma once
#include "structure.h"
#include "Matrix4x4.h"
#include "Maths.h"

#include "DirectXCommon.h"

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

	ID3D12Resource* GetConstBuffer() { return constBuffer; }


private:

	WorldTransform local_;

	WorldTransform camera_;

	DirectXCommon* dxCommon_ = nullptr;

	Matrix4x4* mapMatrix_ = nullptr;

	ID3D12Resource* constBuffer = nullptr;

};

