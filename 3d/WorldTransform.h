#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "DirectXCommon.h"
#include "Maths.h"


struct CbTransform final{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 NormalMatrix;
};

/// <summary>
/// ワールド座標
/// </summary>
class World{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファ作成
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// マッピング
	/// </summary>
	void Map();

public:

	Vector3 scale_ = { 1,1,1 };
	Vector3 rotation_ = { 0,0,0 };
	Vector3 translation_ = { 0,0,0 };

	DirectXCommon* dxCommon_ = nullptr;

	CbTransform* MapMatrix_ = nullptr;

	ID3D12Resource* constBuffer_ = nullptr;

};

