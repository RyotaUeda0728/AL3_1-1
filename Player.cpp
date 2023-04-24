#include "Player.h"
#include <cassert>

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

	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

void Player::Draw(ViewProjection& viewProjection) 
{ 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
