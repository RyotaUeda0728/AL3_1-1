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
	void Initialize(Model* model);

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

	/// <summary>
	/// 自キャラの情報を持ってくる
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

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
	std::list<EnemyBullet*> bullets_;

	//発射タイマー
	int32_t FireTimer = kFireInterval;

	//自キャラ
	Player* player_ = nullptr;

};
