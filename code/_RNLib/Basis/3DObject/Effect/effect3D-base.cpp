//========================================
// 
// �G�t�F�N�g3D���N���X�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�G�t�F�N�g3D���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CEffect3D_Base::CEffect3D_Base(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const CEase::TYPE& easeType, const CDrawState::ALPHA_BLEND_MODE& alphaBlendType) {

	// ���X�g�ɒǉ�
	RNSystem::GetEffet3DMgr().AddList(this);

	m_priority       = priority;
	m_pos            = pos;
	m_rot            = rot;
	m_life           = 
	m_lifeMax        = life;
	m_easeType       = easeType;
	m_alphaBlendMode = alphaBlendType;
}

//========================================
// �f�X�g���N�^
//========================================
CEffect3D_Base::~CEffect3D_Base() {

	// ���X�g����폜
	RNSystem::GetEffet3DMgr().SubList(this);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [�����J]�G�t�F�N�g3D���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ���C�t�J�E���g����
//========================================
float CEffect3D_Base::CountLife(void) {

	if (m_life != NONEDATA) {
		if (--m_life <= 0) {
			Delete();
		}

		return CEase::Easing(m_easeType, m_lifeMax - m_life, m_lifeMax);
	}

	return 0.0f;
}