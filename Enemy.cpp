#include "Enemy.h"
#include <cassert>

Enemy::~Enemy() 
{ 
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);

	// 受け渡し
	model_ = model;

	// ワールド初期化
	worldTransform_.Initialize();

	// 敵の初期座標
	Vector3 pos = {10, 2, 60};
	worldTransform_.translation_ = pos;

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	//接近フェーズ初期化
	ApproachInitialize();

}

void Enemy::Update() 
{
	// 敵の移動速さ
	const float kEnemyApproachSpeed = 0.2f;
	const float kEnemyLeaveSpeed = 0.3f;
	//敵の移動
	switch (phase_) {
		case Phase::Approach:
	default:
		//移動(ベクトルを加算)
		worldTransform_.translation_.z -= kEnemyApproachSpeed;
		//既定の位置に用達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		//弾発射
		ApproachUpdate(); 
		break;
	case Phase::Leave:
		//移動(ベクトルを加算)
		worldTransform_.translation_.x -= kEnemyLeaveSpeed;
		worldTransform_.translation_.y += kEnemyLeaveSpeed;
		break;
	}

	//弾(一個の時)
	//if (bullet_) {
	//	bullet_->Update();
	//}


	//弾(複数の時)
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection)
{
	//弾(一個のとき)
	//if (bullet_) {
	//	bullet_->Draw(viewProjection);
	//}

	//弾(複数の時)
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}

void Enemy::Fire() {
	//if (input_->TriggerKey(DIK_SPACE)) {

		// 自キャラの座標をコピー
		// DirectX::XMFLOAT3 position = worldTransform_.translation_;

		// 弾があれば解放する
		// if (bullet_) {
		//	delete bullet_;
		//	bullet_ = nullptr;
		// }

		// 弾の速度
		const float kBulletSpeed = -2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//初期化(一個だけのとき)
	    //bullet_ = new EnemyBullet();
	    //bullet_->Initialize(model_, worldTransform_.translation_, velocity);

		// 速度ベクトルを自機の向きに合わせて回転する（どっちでも）
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化（複数のとき）
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		// bullet_ = newBullet;

		// 弾を登録する
		bullets_.push_back(newBullet);
	//}
}

void Enemy::ApproachInitialize() 
{
	//発射タイマーを初期化
	FireTimer = kFireInterval;
}

void Enemy::ApproachUpdate() 
{
	//発射タイマーカウントダウン
	--FireTimer;
	//指定時間に達した
	if (FireTimer <= 0) {
	//弾を発射
		Fire();
		//発射タイマーを初期化;
		FireTimer = kFireInterval;
	}
}
