//========================================
// 
// �W���G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�W���G�t�F�N�g3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_StandardEffect3D::_RNC_StandardEffect3D() {

	m_priority = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_StandardEffect3D::~_RNC_StandardEffect3D() {

}

//========================================
// ���n���̓y����������
//========================================
void _RNC_StandardEffect3D::CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force) {

	CEffect3D_Cylinder::Circle circleFront = {};
	circleFront.startRadius = 0.0f;
	circleFront.endRadius   = force * 1.25f;
	circleFront.startDepth  = -force / 1.5f;
	circleFront.endDepth    = 0.0f;
	circleFront.col         = col;

	CEffect3D_Cylinder::Circle circleBack = {};
	circleBack.startRadius = 0.0f;
	circleBack.endRadius   = force;
	circleBack.startDepth  = 0.0f;
	circleBack.endDepth    = 0.0f;
	circleBack.col         = col;

	RNLib::Effect3D().CreateCylinder(m_priority, pos, rot + Rot3D(D3DX_PI_HALF, 0.0f, RNLib::Number().GetRandomFloat(1.0f) * D3DX_PI_DOUBLE), 30, _RNC_Ease::TYPE::OUT_SINE, _RNC_DrawState::ALPHA_BLEND_MODE::NORMAL, 8, circleFront, circleBack)
		->SetSpinZ(-0.01f + RNLib::Number().GetRandomFloat(1.0f) * 0.02f)
		->SetTexIdx(RNLib::DefaultData().GetTextureIdx(_RNC_DefaultData::TEXTURE::SMOKE))
		->SetTexXNum(4);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�W���G�t�F�N�g3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ����������
//========================================
void _RNC_StandardEffect3D::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_StandardEffect3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void _RNC_StandardEffect3D::Update(void) {

}