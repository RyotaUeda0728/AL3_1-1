#include "EnemyBullet.h"
#include <cassert>

void EnemyBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("white1x1.png");

	world_.Initialize();
	world_.translation_ = pos;

	velocity_ = velocity;
}

void EnemyBullet::Update() {

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	world_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewPrijection) {
	model_->Draw(world_, viewPrijection, texturehandle_);
}
