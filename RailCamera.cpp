#include "RailCamera.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) 
{
	worldTransform_.Initialize();
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() 
{ 
	Vector3 move = {0.f, 0.f, 0.f};
	Vector3 rot = {0.f, 0.001f, 0.f};

	//ワールドトランスフォームの座標の数値を加算したりする（移動）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//ワールドトランスフォームの角度の数値を加算したりする（回転）
	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	viewProjection_.TransferMatrix();


	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	//スライダーでカメラのtranslationを表示
	ImGui::DragFloat3("cameraTranslation", &worldTransform_.translation_.x, 0.01f);
	//スライダーでカメラのrotationを表示
	ImGui::DragFloat3("cameraRotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}
