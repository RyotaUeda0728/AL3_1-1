#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() 
{
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 行列を定数バッファに転送
	TransferMatrix();
}