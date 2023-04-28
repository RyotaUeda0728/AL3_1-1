#include "Player.h"
#include <cassert>
#include "ImGui.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);

	//受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update()
{ 
	// キャラクターの移動ベクトル
	Vector3 move = {0,0,0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.4f;
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);



	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

#ifdef _DEBUG
	//InGui
	ImGui::SetNextWindowPos({0,0});
	ImGui::SetNextWindowSize({300,100});

	//座標の設定
	ImGui::Begin("Player");
	float sliderValue[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y,worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG
}

void Player::Draw(ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
