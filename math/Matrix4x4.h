#pragma once
#include "Vector3.h"

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

//スケーリング行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 回転行列X軸
Matrix4x4 MakeRotationXMatrix(float theta);
// 回転行列Y軸
Matrix4x4 MakeRotationYMatrix(float theta);
// 回転行列Z軸
Matrix4x4 MakeRotationZMatrix(float theta);

//平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 trans);

//アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& trans);

//代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

//2項演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);