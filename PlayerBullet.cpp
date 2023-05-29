#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& pos,const Vector3& velocity) { 
	assert(model);
	model_ = model;

	texturehandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	velocity_ = velocity;

}

void PlayerBullet::Update() 
{ 

	//座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;


	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewPrijection)
{ 
	model_->Draw(worldTransform_, viewPrijection, texturehandle_); 
}

Vector3 PlayerBullet::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::OnCollision()
{ 
	isDead_ = true;
}