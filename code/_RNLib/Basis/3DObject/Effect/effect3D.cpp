//========================================
// 
// �G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�G�t�F�N�g3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Effect3D::_RNC_Effect3D() {

}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Effect3D::~_RNC_Effect3D() {

}


//========================================
// ����������
//========================================
void _RNC_Effect3D::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_Effect3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void _RNC_Effect3D::Update(void) {

}

//========================================
// �V�����_�[�G�t�F�N�g��������
//========================================
CEffect3D_Cylinder* _RNC_Effect3D::CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const _RNC_Ease::TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack) {

	return (new CEffect3D_Cylinder(priority, pos, rot, life, easeType, alphaBlendType))
		->SetDivisionNum(divNum)
		->SetCircleFront(circleFront)
		->SetCircleBack(circleBack);
}