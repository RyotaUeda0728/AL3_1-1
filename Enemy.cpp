#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	// 受け渡し
	model_ = model;

	// ワールド初期化
	worldTransform_.Initialize();

	// 敵の初期座標
	Vector3 pos = {0, 2, 50};
	worldTransform_.translation_ = pos;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");
	
}

void Enemy::Update() 
{
	// 敵の移動速さ
	const float kEnemyApproachSpeed = 0.4f;
	const float kEnemyLeaveSpeed = 0.6f;
	//敵の移動
	//worldTransform_.translation_.z -= kEnemySpeed;
	switch (phase_) {
		case Phase::Approach:
	default:
		//移動(ベクトルを加算)
		worldTransform_.translation_.z -= kEnemyApproachSpeed;
		//既定の位置に用達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		//移動(ベクトルを加算)
		worldTransform_.translation_.x -= kEnemyLeaveSpeed;
		worldTransform_.translation_.y += kEnemyLeaveSpeed;
		break;
	}

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}
