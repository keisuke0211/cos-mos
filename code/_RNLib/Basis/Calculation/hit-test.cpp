//========================================
// 
// �����蔻��̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�����蔻��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CHitTest::CHitTest() {

}

//========================================
// �f�X�g���N�^
//========================================
CHitTest::~CHitTest() {

}

//========================================
// ����������
//========================================
void CHitTest::Init(void) {

}

//========================================
// �I������
//========================================
void CHitTest::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CHitTest::Update(void) {

}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�����蔻��N���X�̃����o�֐�(XZ����)
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]�_�����`�̏d�Ȃ蔻��
//========================================
bool CHitTest::XZ::OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale) {

	const float targetWidthHalf = targetScale.x * 0.5f;
	const float targetDepthHalf = targetScale.y * 0.5f;
	const float targetFront = targetPos.z - targetDepthHalf;
	const float targetBack  = targetPos.z + targetDepthHalf;
	const float targetLeft  = targetPos.x - targetWidthHalf;
	const float targetRight = targetPos.x + targetWidthHalf;

	if (basePos.x >= targetLeft && basePos.x <= targetRight &&
		basePos.z >= targetFront && basePos.z <= targetBack) {
		return true;
	}

	return false;
}