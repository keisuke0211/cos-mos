//========================================
// 
// �W���G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�W���G�t�F�N�g3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CStandardEffect3D::CStandardEffect3D() {

	m_priority = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CStandardEffect3D::~CStandardEffect3D() {

}


//========================================
// ����������
//========================================
void CStandardEffect3D::Init(void) {

}

//========================================
// �I������
//========================================
void CStandardEffect3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CStandardEffect3D::Update(void) {

}

//========================================
// ���n���̓y����������
//========================================
void CStandardEffect3D::CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force) {

	CEffect3D_Cylinder::Circle circleFront = {};
	circleFront.startRadius = 0.0f;
	circleFront.endRadius   = force * 1.25f;
	circleFront.startDepth  = -force / 2.5f;
	circleFront.endDepth    = -force / 1.5f;
	circleFront.col         = col;

	CEffect3D_Cylinder::Circle circleBack = {};
	circleBack.startRadius = 0.0f;
	circleBack.endRadius   = force;
	circleBack.startDepth  = 0.0f;
	circleBack.endDepth    = 0.0f;
	circleBack.col         = col;

	RNLib::Effect3D().CreateCylinder(m_priority, pos, rot + Rot3D(D3DX_PI_HALF, 0.0f, fRand() * D3DX_PI_DOUBLE), 30, CEase::TYPE::OUT_SINE, CDrawState::ALPHA_BLEND_MODE::NORMAL, 8, circleFront, circleBack)
		->SetSpinZ(-0.01f + fRand() * 0.02f)
		->SetTexIdx(RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::SMOKE))
		->SetTexXNum(4);
}