#pragma once
/// <summary>
/// 自キャラ
/// </summary>

#include <list>

#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "Input.h"
#include "PlayerBullet.h"
#include "MathUtility.h"
#include "Sprite.h"
#include "WinApp.h"

class Player 
{
public:
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle, Vector3 playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	//当たり判定のための半径を設定
	const float GetRadius() { return radius_; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 3Dレティクルの座標を持ってくる
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldTransform3DReticle();

	bool IsDead() const { return isDead_; }

	bool IsTitle() const { return isTitle_; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//uint32_t textureGameover_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	PlayerBullet* bullet_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	//当たり判定のための半径
	const float radius_ = 1.8f;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	// 2D画像
	Sprite* spriteTitle_ = nullptr;
	Sprite* spriteGameOver_ = nullptr;
	Sprite* spriteGameClear_ = nullptr;

	// タイトル表示フラグ
	bool isTitle_ = true;

	// クリアタイム
	uint32_t clearTime_ = 4950;

	// デスフラグ
	bool isDead_ = false;

};
