//========================================
// 
// マトリックス関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]マトリックスクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Matrix::_RNC_Matrix() {

}

//========================================
// デストラクタ
//========================================
_RNC_Matrix::~_RNC_Matrix() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Matrix::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Matrix::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Matrix::Update(void) {

}

//========================================
// 位置をマトリックスに変換
//========================================
Matrix _RNC_Matrix::ConvPosToMtx(const Pos3D& pos) {

	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// 位置を反映
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// 向きをマトリックスに変換
//========================================
Matrix _RNC_Matrix::ConvRotToMtx(const Rot3D& rot) {

	// 向きを反映
	Matrix rotMtx = INITMATRIX;
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);

	return rotMtx;
}

//========================================
// 位置と向きをマトリックスに変換
//========================================
Matrix _RNC_Matrix::ConvPosRotToMtx(const Pos3D& pos, const Rot3D& rot) {

	Matrix rotMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	// 位置を反映
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// 位置と法線をマトリックスに変換
//========================================
Matrix _RNC_Matrix::ConvPosNorToMtx(const Pos3D& pos, const Normal3D& nor) {

	Matrix rotMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// 法線を反映
	mtx._31 = nor.x;
	mtx._32 = nor.y;
	mtx._33 = nor.z;

	// 位置を反映
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// 位置と向きと拡大倍率をマトリックスに変換
//========================================
Matrix _RNC_Matrix::ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale) {

	Matrix rotMtx = INITMATRIX;
	Matrix scaleMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx = INITMATRIX;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	// 拡大倍率を反映
	D3DXMatrixScaling(&scaleMtx, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&mtx, &mtx, &scaleMtx);

	// 位置を反映
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// マトリックスを位置に変換
//========================================
Pos3D _RNC_Matrix::ConvMtxToPos(const Matrix& mtx) {

	return Pos3D(mtx._41, mtx._42, mtx._43);
}

//========================================
// マトリックスを向きに変換
//========================================
Rot3D _RNC_Matrix::ConvMtxToRot(const Matrix& mtx) {

	float x = -asinf(mtx._32);
	float y = acosf(mtx._11);

	if (mtx._22 < 0.0f) { x = ((D3DX_PI * (fabsf(x) / x)) - x); }
	if (mtx._13 > 0.0f) { y = -y; }

	return Rot3D(x, y, 0.0f);
}

//========================================
// マトリックスを向きベクトルに変換
//========================================
Vector3D _RNC_Matrix::ConvMtxToRotVec(const Matrix& mtx) {
	return Vector3D(mtx._31, mtx._32, mtx._33);
}

//========================================
// マトリックスの拡大倍率を調べる
//========================================
Scale3D _RNC_Matrix::ConvMtxToScale(const Matrix& mtx) {

	const Vector3D vectorX = Vector3D(mtx._11, mtx._12, mtx._13);
	const Vector3D vectorY = Vector3D(mtx._21, mtx._22, mtx._23);
	const Vector3D vectorZ = Vector3D(mtx._31, mtx._32, mtx._33);

	return Scale3D(
		D3DXVec3Length(&vectorX),
		D3DXVec3Length(&vectorY),
		D3DXVec3Length(&vectorZ));
}

//========================================
// マトリックスを掛け合わせる
//========================================
Matrix _RNC_Matrix::MultiplyMtx(const Matrix& parentMtx, const Matrix& childMtx) {

	Matrix mtx = INITMATRIX;
	D3DXMatrixMultiply(&mtx, &childMtx, &parentMtx);

	return mtx;
}