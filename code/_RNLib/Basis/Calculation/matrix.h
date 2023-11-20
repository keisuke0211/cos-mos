//========================================
// 
// �}�g���b�N�X�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
class CMatrix {
public:
	//========== [[[ �֐��錾 ]]]
	static Matrix  ConvPosToMtx        (const Pos3D& pos);
	static Matrix  ConvRotToMtx        (const Rot3D& rot);
	static Matrix  ConvPosRotToMtx     (const Pos3D& pos, const Rot3D& rot);
	static Matrix  ConvPosNorToMtx     (const Pos3D& pos, const Normal3D& nor);
	static Matrix  ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale);
	static Pos3D   ConvMtxToPos        (const Matrix& mtx);
	static Rot3D   ConvMtxToRot        (const Matrix& mtx);
	static Scale3D ConvMtxToScale      (const Matrix& mtx);
	static Matrix  MultiplyMtx         (const Matrix& mtxA, const Matrix& mtxB);
	CMatrix();
	~CMatrix();
	void Init  (void);
	void Uninit(void);
	void Update(void);
};