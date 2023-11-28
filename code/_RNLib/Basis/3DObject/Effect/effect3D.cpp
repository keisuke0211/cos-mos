//========================================
// 
// �G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�G�t�F�N�g3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CEffect3D::CEffect3D() {

}

//========================================
// �f�X�g���N�^
//========================================
CEffect3D::~CEffect3D() {

}


//========================================
// ����������
//========================================
void CEffect3D::Init(void) {

}

//========================================
// �I������
//========================================
void CEffect3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CEffect3D::Update(void) {

}

//========================================
// �V�����_�[�G�t�F�N�g��������
//========================================
CEffect3D_Cylinder* CEffect3D::CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const CEase::TYPE& easeType, const CDrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack) {

	return (new CEffect3D_Cylinder(priority, pos, rot, life, easeType, alphaBlendType))
		->SetDivisionNum(divNum)
		->SetCircleFront(circleFront)
		->SetCircleBack(circleBack);
}