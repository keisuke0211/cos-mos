//========================================
// 
// マトリックス関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
class _RNC_Matrix {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Matrix                  ();
	~_RNC_Matrix                 ();
	Matrix   ConvPosToMtx        (const Pos3D& pos);
	Matrix   ConvRotToMtx        (const Rot3D& rot);
	Matrix   ConvPosRotToMtx     (const Pos3D& pos, const Rot3D& rot);
	Matrix   ConvPosNorToMtx     (const Pos3D& pos, const Normal3D& nor);
	Matrix   ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale);
	Pos3D    ConvMtxToPos        (const Matrix& mtx);
	Rot3D    ConvMtxToRot        (const Matrix& mtx);
	Vector3D ConvMtxToRotVec     (const Matrix& mtx);
	Scale3D  ConvMtxToScale      (const Matrix& mtx);
	Matrix   MultiplyMtx         (const Matrix& parentMtx, const Matrix& childMtx);

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_Calculation;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};