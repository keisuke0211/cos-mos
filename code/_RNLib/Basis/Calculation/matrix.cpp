//========================================
// 
// �}�g���b�N�X�֘A�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�}�g���b�N�X�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]�ʒu���}�g���b�N�X�ɕϊ�
//========================================
Matrix CMatrix::ConvPosToMtx(const Pos3D& pos) {

	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// [�ÓI]�������}�g���b�N�X�ɕϊ�
//========================================
Matrix CMatrix::ConvRotToMtx(const Rot3D& rot) {

	Matrix rotMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	return mtx;
}

//========================================
// [�ÓI]�ʒu�ƌ������}�g���b�N�X�ɕϊ�
//========================================
Matrix CMatrix::ConvPosRotToMtx(const Pos3D& pos, const Rot3D& rot) {

	Matrix rotMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// [�ÓI]�ʒu�Ɩ@�����}�g���b�N�X�ɕϊ�
//========================================
Matrix CMatrix::ConvPosNorToMtx(const Pos3D& pos, const Normal3D& nor) {

	Matrix rotMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// �@���𔽉f
	mtx._31 = nor.x;
	mtx._32 = nor.y;
	mtx._33 = nor.z;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// [�ÓI]�ʒu�ƌ����Ɗg��{�����}�g���b�N�X�ɕϊ�
//========================================
Matrix CMatrix::ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale) {

	Matrix rotMtx = INITMATRIX;
	Matrix scaleMtx = INITMATRIX;
	Matrix posMtx = INITMATRIX;
	Matrix mtx = INITMATRIX;

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtx, &mtx, &rotMtx);

	// �g��{���𔽉f
	D3DXMatrixScaling(&scaleMtx, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&mtx, &mtx, &scaleMtx);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// [�ÓI]�}�g���b�N�X���ʒu�ɕϊ�
//========================================
Pos3D CMatrix::ConvMtxToPos(const Matrix& mtx) {

	return Pos3D(mtx._41, mtx._42, mtx._43);
}

//========================================
// [�ÓI]�}�g���b�N�X�������ɕϊ�
//========================================
Rot3D CMatrix::ConvMtxToRot(const Matrix& mtx) {

	return Rot3D(
		atan2f(mtx._32, mtx._33),
		atan2f(-mtx._31, sqrtf(mtx._32 * mtx._32 + mtx._33 * mtx._33)),
		atan2f(mtx._21, mtx._11));
}

//========================================
// [�ÓI]�}�g���b�N�X�̊g��{���𒲂ׂ�
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
// [�ÓI]�}�g���b�N�X���|�����킹��
//========================================
Matrix CMatrix::MultiplyMtx(const Matrix& mtxA, const Matrix& mtxB) {

	Matrix mtx = INITMATRIX;
	D3DXMatrixMultiply(&mtx, &mtxA, &mtxB);

	return mtx;
}

//========================================
// �R���X�g���N�^
//========================================
CMatrix::CMatrix() {

}

//========================================
// �f�X�g���N�^
//========================================
CMatrix::~CMatrix() {

}

//========================================
// ����������
//========================================
void CMatrix::Init(void) {

}

//========================================
// �I������
//========================================
void CMatrix::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CMatrix::Update(void) {

}