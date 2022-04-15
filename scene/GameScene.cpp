#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	// 3Dモデルの生成
	model_ = Model::Create();

	// X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f, 5.0f, 5.0f};

	// X,Y,Z方向の回転角を設定
	worldTransform_.rotation_ = {XM_PI / 4.0f, XM_PI / 4.0f, 0.0f};

	// X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f, 10.0f, 10.0f};

	//ワールドビュートランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void GameScene::Update() {

	//値を含んだ文字列
	//平行移動
	std::string strDebugTranslation =std::string("translation:(") +
									 std::to_string(worldTransform_.translation_.x) + std::string(",") +
									 std::to_string(worldTransform_.translation_.y) + std::string(",") +
									 std::to_string(worldTransform_.translation_.z) + std::string(")");

	//回転角
	std::string strDebugRotation = std::string("rotation:(") +
	                               std::to_string(worldTransform_.rotation_.x) + std::string(",") +
	                               std::to_string(worldTransform_.rotation_.y) + std::string(",") +
	                               std::to_string(worldTransform_.rotation_.z) + std::string(")");

	//スケーリング
	std::string strDebugScale = std::string("scale:(") +
								std::to_string(worldTransform_.scale_.x) + std::string(",") +
								std::to_string(worldTransform_.scale_.y) + std::string(",") +
								std::to_string(worldTransform_.scale_.z) + std::string(")");

	//デバッグテキストの表示
	debugText_->Print(strDebugTranslation, 50, 50, 1.0f);
	debugText_->Print(strDebugRotation, 50, 70, 1.0f);
	debugText_->Print(strDebugScale, 50, 90, 1.0f);
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
	/// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}