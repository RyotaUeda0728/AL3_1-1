#include "Player.h"
#include <cassert>
#include "ImGui.h"
#include "Player.h"

Player::~Player() {
	//bullet_ の解放
	//if (bullet_) {
	//	delete bullet_;
	//}

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 playerPosition) {
	// NULLポインタチェック
	assert(model);

	//受け渡し
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド初期化
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//座標を移動
	worldTransform_.translation_.x = playerPosition.x;
	worldTransform_.translation_.y = playerPosition.y;
	worldTransform_.translation_.z = playerPosition.z;
}

void Player::Update()
{ 
	// キャラクターの移動ベクトル
	Vector3 move = {0,0,0};
	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.4f;
	// 押した方向で移動ベクトルを変更(上下左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} 
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//旋回
	const float matRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= matRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += matRotSpeed;
	}

	//攻撃
	Attack();

	//弾更新
	//if (bullet_) {
	//	bullet_->Update();
	//}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	const float kRotLimitX = 20.0f;
	//ImGui::DragFloat3("cameraTranslation", &worldTransform_.translation_.x, 0.01f);const float kRotLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);

	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotLimitX);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotLimitX);

	

	//座標移動(ベクトルの加算)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

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

	//if (bullet_) {
	//	bullet_->Draw(viewProjection); 
	//}
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack() 
{ 
	if (input_->TriggerKey(DIK_SPACE)) {

		//自キャラの座標をコピー
		//DirectX::XMFLOAT3 position = worldTransform_.translation_;

		//弾があれば解放する
		//if (bullet_) {
		//	delete bullet_;
		//	bullet_ = nullptr;
		//}

	//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

	//速度ベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		//bullet_ = newBullet;

		// 弾を登録する
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() 
{ 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; 
}

void Player::OnCollision() 
{

}

void Player::SetParent(const WorldTransform* parent) 
{
	//親子関係を結ぶ
	worldTransform_.parent_ = parent;
}
