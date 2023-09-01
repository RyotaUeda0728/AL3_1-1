#pragma once

#include <list>

#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "MathUtility.h"

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言（苦肉の策）
class GameScene;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	void Initialize(Model* model,Vector3 pos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="view"></param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 発射間隔
	/// </summary>
	static const int kFireInterval = 60;

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInitialize();

	/// <summary>
	/// 接近フェーズ更新
	/// </summary>
	void ApproachUpdate();

	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 自キャラの情報を持ってくる
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 敵キャラにゲームシーンの情報を持ってくる
	/// </summary>
	/// <param name="scene"></param>
	void SetGameScene(GameScene* scene) { gameScene = scene; }

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
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	// 当たり判定のための半径を設定
	const float GetRadius() { return radius_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// キーボード入力
	//Input* input_ = nullptr;

	//行動フェーズ
	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	EnemyBullet* bullet_ = nullptr;

	// 弾
	//std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t FireTimer = kFireInterval;

	//自キャラ
	Player* player_ = nullptr;

	// 当たり判定のための半径
	const float radius_ = 1.0f;

	// ゲームシーン
	GameScene* gameScene = nullptr;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//死亡フラグ
	bool isDead_ = false;
	
};
