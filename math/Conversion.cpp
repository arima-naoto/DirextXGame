#define NOMINMAX
#include "Conversion.h"
#include "WinApp.h"
#include "Input.h"
#include "algorithm"

#include "imgui.h"

void Conversion::Initialize()
{
	local_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	camera_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f,},{0.0f,0.0f,-5.0f} };

}

void Conversion::CreateConstBuffer()
{
	HRESULT result = S_FALSE;

	dxCommon_ = DirectXCommon::GetInstance();
	ID3D12Device* dev = dxCommon_->GetDevice();


	CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resdesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(CbTransform) + 0xff) & ~0xff);

	dev->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuffer));

	result = constBuffer->Map(0, nullptr, (void**)&mapMatrix_);
	mapMatrix_->World = Maths::IdentityMatrix();
	mapMatrix_->WVP = Maths::IdentityMatrix();

}

void Conversion::Transform()
{
	Matrix4x4 worldMatrix = Maths::AffineMatrix(local_);
	Matrix4x4 cameraMatrix = Maths::STRAffineMatrix(camera_);
	Matrix4x4 viewMatrix = Maths::Inverse(cameraMatrix);

	float aspect =
		float(WinApp::window_width) / float(WinApp::window_height);

	Matrix4x4 projMatrix =
		Maths::MakePerspectiveFovMatrix(float(M_PI_2), aspect, 0.1f, 100.0f);

	mapMatrix_->World = worldMatrix; 
	mapMatrix_->WVP = worldMatrix * viewMatrix * projMatrix; 	
}

void Conversion::RotateCamera()
{
	static Vector2 prevMouse = { 0, 0 };
	static Vector2 mouse = { 0, 0 };

	Input* input = Input::GetInstance();
	
	// Inputクラスからマウス位置を取得（Vector2Int -> Vector2に変換）
	Vector2Int mousePosInt = input->GetMousePosition();
	mouse = { static_cast<float>(mousePosInt.x), static_cast<float>(mousePosInt.y) };

	int mouseClickLeft = 0;

	if (input->IsPressMouse(mouseClickLeft)) {
		Vector2 delta = mouse - prevMouse;
		camera_.rotate.x += delta.y * 0.0025f;
		camera_.rotate.y += delta.x * 0.0025f;
	}

	prevMouse = mouse;
}

void Conversion::ZoomCamera()
{
	Input* input = Input::GetInstance();

	int32_t wheelScroll = input->GetWheel();
	//スクロール量の最小値と最大値
	float scroll[2] = { 0.5f,5.0f };

	if (wheelScroll != 0) {
		camera_.translate.z -= (wheelScroll / (1024.f * 2));
		camera_.translate.z = std::min(std::max(camera_.translate.z, scroll[0]), scroll[1]);;
	}
}

void Conversion::DrawDebugText()
{
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Scale", &camera_.scale.x, 0.01f);
	ImGui::DragFloat3("Rotate", &camera_.rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &camera_.translate.x, 0.01f);
	ImGui::End();

}
