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
// [静的]位置をマトリックスに変換
//========================================
Matrix CMatrix::ConvPosToMtx(const Pos3D& pos) {

	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// 位置を反映
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// [静的]向きをマトリックスに変換
//========================================
Matrix CMatrix::ConvRotToMtx(const Rot3D& rot) {

	Matrix rotMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	return mtx;
}

//========================================
// [静的]位置と向きをマトリックスに変換
//========================================
Matrix CMatrix::ConvPosRotToMtx(const Pos3D& pos, const Rot3D& rot) {

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
// [静的]位置と法線をマトリックスに変換
//========================================
Matrix CMatrix::ConvPosNorToMtx(const Pos3D& pos, const Normal3D& nor) {

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
// [静的]位置と向きと拡大倍率をマトリックスに変換
//========================================
Matrix CMatrix::ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale) {

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
// [静的]マトリックスを位置に変換
//========================================
Pos3D CMatrix::ConvMtxToPos(const Matrix& mtx) {

	return Pos3D(mtx._41, mtx._42, mtx._43);
}

//========================================
// [静的]マトリックスを向きに変換
//========================================
Rot3D CMatrix::ConvMtxToRot(const Matrix& mtx) {

	return Rot3D(
		atan2f(mtx._32, mtx._33),
		atan2f(-mtx._31, sqrtf(mtx._32 * mtx._32 + mtx._33 * mtx._33)),
		atan2f(mtx._21, mtx._11));
}

//========================================
// [静的]マトリックスの拡大倍率を調べる
//========================================
Scale3D CMatrix::ConvMtxToScale(const Matrix& mtx) {

	const Vector3D vectorX = Vector3D(mtx._11, mtx._12, mtx._13);
	const Vector3D vectorY = Vector3D(mtx._21, mtx._22, mtx._23);
	const Vector3D vectorZ = Vector3D(mtx._31, mtx._32, mtx._33);

	return Scale3D(
		D3DXVec3Length(&vectorX),
		D3DXVec3Length(&vectorY),
		D3DXVec3Length(&vectorZ));
}

//========================================
// [静的]マトリックスを掛け合わせる
//========================================
Matrix CMatrix::MultiplyMtx(const Matrix& mtxA, const Matrix& mtxB) {

	Matrix mtx = INITMATRIX;
	D3DXMatrixMultiply(&mtx, &mtxA, &mtxB);

	return mtx;
}

//========================================
// コンストラクタ
//========================================
CMatrix::CMatrix() {

}

//========================================
// デストラクタ
//========================================
CMatrix::~CMatrix() {

}

//========================================
// 初期化処理
//========================================
void CMatrix::Init(void) {

}

//========================================
// 終了処理
//========================================
void CMatrix::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CMatrix::Update(void) {

}