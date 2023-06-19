#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::~GameScene() { 
	delete model_; 
	delete player_;
	delete enemy_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("cube/cube.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	//レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize({0.f, 0.f, -10.f}, {0.f, 0.f, 0.f});

	//ワールドトランスフォーム初期化
	//worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->SetParent(&railCamera_->GetWorldTransform());

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_);
	enemy_->SetPlayer(player_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth,WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//自機をカメラからずらす
	Vector3 playerPosition(0, -10.0f, 50.0f);
	player_->Initialize(model_, textureHandle_,playerPosition);
	
}

void GameScene::Update() 
{ 
	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	if (enemy_) {//nullかどうかをチェック
		enemy_->Update();
	}

	//スカイドームの更新
	skydome_->Update();

	//ロールカメラ
	railCamera_->Update();

	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_BACKSPACE)) {
		isDebugCameraActive_ = true;
	}
	#endif
	
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else if(!isDebugCameraActive_){
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	//当たり判定
	CheckAllCollision();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	if (enemy_) { // nullかどうかをチェック
		enemy_->Draw(viewProjection_);
	}

	//スカイドームの描画
	skydome_->Draw(viewProjection_);
	
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision() 
{
	//衝突判定AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region //自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	//敵弾の座標
		posB = bullet->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
						 (posB.y - posA.y) * (posB.y - posA.y) +
						 (posB.z - posA.z) * (posB.z - posA.z);

		//球と球の交差判定
		if (distance <= powf(player_->GetRadius() + bullet->GetRadius(),2.0f)) {
		//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		
		}
	}
	#pragma endregion

	#pragma region //自弾と敵キャラの当たり判定
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();
	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posB = bullet->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);

		// 球と球の交差判定
		if (distance <= powf(enemy_->GetRadius() + bullet->GetRadius(),2.0f)) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region //自弾と敵弾の当たり判定
	// 自弾全て
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();
		// 敵弾全て
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posB = enemyBullet->GetWorldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
							 (posB.y - posA.y) * (posB.y - posA.y) +
							 (posB.z - posA.z) * (posB.z - posA.z);

			// 球と球の交差判定
			if (distance <= powf(playerBullet->GetRadius() + enemyBullet->GetRadius(), 2.0f)) {
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}
