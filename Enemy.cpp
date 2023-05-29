#include "Enemy.h"
#include "Player.h"
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
		if (worldTransform_.translation_.y >= 25.0f) {
			worldTransform_.translation_.x = 10.0f;
			worldTransform_.translation_.y = 2.0f;
			worldTransform_.translation_.z = 60.0f;
			phase_ = Phase::Approach;
		}
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
		//Vector3 velocity(0, 0, kBulletSpeed);

		//初期化(一個だけのとき)
	    //bullet_ = new EnemyBullet();
	    //bullet_->Initialize(model_, worldTransform_.translation_, velocity);

		//敵キャラ→自キャラの差分ベクトルを求める
	    Vector3 distance;
	    distance.x = GetWorldPosition().x - player_->GetWorldPosition().x;
	    distance.y = GetWorldPosition().y - player_->GetWorldPosition().y;
	    distance.z = GetWorldPosition().z - player_->GetWorldPosition().z;

		//ベクトルの正規化
	    float length =
	        sqrt((distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z));
	    Vector3 dir (distance.x / length, distance.y / length, distance.z / length);

		//ベクトルの長さを、早さに合わせる
	    Vector3 velocity(dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed);

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

Vector3 Enemy::GetWorldPosition() 
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision() {}
