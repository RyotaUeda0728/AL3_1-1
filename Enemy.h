#pragma once

#include <list>

#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "MathUtility.h"

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

};
