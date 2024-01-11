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
// �R���X�g���N�^
//========================================
_RNC_Matrix::_RNC_Matrix() {

}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Matrix::~_RNC_Matrix() {

}

//========================================
// ����������
//========================================
void _RNC_Matrix::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_Matrix::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void _RNC_Matrix::Update(void) {

}

//========================================
// �ʒu���}�g���b�N�X�ɕϊ�
//========================================
Matrix _RNC_Matrix::ConvPosToMtx(const Pos3D& pos) {

	Matrix posMtx = INITMATRIX;
	Matrix mtx    = INITMATRIX;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&posMtx, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &posMtx);

	return mtx;
}

//========================================
// �������}�g���b�N�X�ɕϊ�
//========================================
Matrix _RNC_Matrix::ConvRotToMtx(const Rot3D& rot) {

	// �����𔽉f
	Matrix rotMtx = INITMATRIX;
	D3DXMatrixRotationYawPitchRoll(&rotMtx, rot.y, rot.x, rot.z);

	return rotMtx;
}

//========================================
// �ʒu�ƌ������}�g���b�N�X�ɕϊ�
//========================================
Matrix _RNC_Matrix::ConvPosRotToMtx(const Pos3D& pos, const Rot3D& rot) {

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
// �ʒu�Ɩ@�����}�g���b�N�X�ɕϊ�
//========================================
Matrix _RNC_Matrix::ConvPosNorToMtx(const Pos3D& pos, const Normal3D& nor) {

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
// �ʒu�ƌ����Ɗg��{�����}�g���b�N�X�ɕϊ�
//========================================
Matrix _RNC_Matrix::ConvPosRotScaleToMtx(const Pos3D& pos, const Rot3D& rot, const Scale3D& scale) {

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
// �}�g���b�N�X���ʒu�ɕϊ�
//========================================
Pos3D _RNC_Matrix::ConvMtxToPos(const Matrix& mtx) {

	return Pos3D(mtx._41, mtx._42, mtx._43);
}

//========================================
// �}�g���b�N�X�������ɕϊ�
//========================================
Rot3D _RNC_Matrix::ConvMtxToRot(const Matrix& mtx) {

	float x = -asinf(mtx._32);
	float y = acosf(mtx._11);

	if (mtx._22 < 0.0f) { x = ((D3DX_PI * (fabsf(x) / x)) - x); }
	if (mtx._13 > 0.0f) { y = -y; }

	return Rot3D(x, y, 0.0f);
}

//========================================
// �}�g���b�N�X�������x�N�g���ɕϊ�
//========================================
Vector3D _RNC_Matrix::ConvMtxToRotVec(const Matrix& mtx) {
	return Vector3D(mtx._31, mtx._32, mtx._33);
}

//========================================
// �}�g���b�N�X�̊g��{���𒲂ׂ�
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
// �}�g���b�N�X���|�����킹��
//========================================
Matrix _RNC_Matrix::MultiplyMtx(const Matrix& parentMtx, const Matrix& childMtx) {

	Matrix mtx = INITMATRIX;
	D3DXMatrixMultiply(&mtx, &childMtx, &parentMtx);

	return mtx;
}