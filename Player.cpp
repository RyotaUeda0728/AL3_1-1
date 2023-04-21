#include "Player.h"
#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	// �󂯓n��
	model_ = model;
	textureHandle_ = textureHandle;

	// ���[���h������
	worldTransform_.Initialize();
}

void Player::Update() {
	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
