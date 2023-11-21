//========================================
// 
// �J�ڂ̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//****************************************
// �}�N����`
//****************************************
#define BLACK_TIME     (10)
#define HOLE_SCALE_MAX (6.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�J�ڃN���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CTransition::CTransition() {

	m_type = TYPE::NONE;
	m_state = STATE::NONE;
	m_fMove = 0.0f;
	m_stateCounter = 0;
	m_time = 0;
	m_col = INITCOLOR;
}

//========================================
// �f�X�g���N�^
//========================================
CTransition::~CTransition() {

}

//========================================
// ����������
//========================================
void CTransition::Init(void) {

}

//========================================
// �I������
//========================================
void CTransition::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CTransition::Update(void) {

	float rate = 0.0f;	// �����Ȃ� 0 ~ 1 ���S�ɖ��܂�

	switch (m_state) {
		// [[[ ���� ]]]
	case STATE::NONE: {

	}break;
		// [[[ �I�[�v�� ]]]
	case STATE::OPEN: {
		rate = 1.0f - ((float)m_stateCounter / m_time);

		// ��Ԃ𖳂���
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::NONE;
		}
	}break;
		// [[[ �I�[�v���҂� ]]]
	case STATE::OPEN_WAIT: {
		rate = 1.0f;
	}break;
		// [[[ �N���[�Y ]]]
	case STATE::CLOSE: {
		rate = ((float)m_stateCounter / m_time);

		// ��Ԃ��I�[�v���҂���
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::OPEN_WAIT;
		}
	}break;
	}

	// ������0�ł���ΏI��
	if (rate == 0.0f)
		return;

	// ��ʖ��ߏ���
	FillScreen(rate);
}

//========================================
// �I�[�v������
//========================================
bool CTransition::Open(const TYPE& type, const UShort& time) {

	// ��ԃI�[�v���҂��Ŗ������A
	if (m_state != STATE::OPEN_WAIT)
		return false;

	m_stateCounter = 0;
	m_state = STATE::OPEN;
	m_time = time;

	return true;
}

//========================================
// �N���[�Y����
//========================================
bool CTransition::Close(const TYPE& type, const Color& col, const UShort& time) {

	// ��Ԃ������Ŗ������A
	if (m_state != STATE::NONE)
		return false;

	m_stateCounter = 0;
	m_state = STATE::CLOSE;
	m_time = time;
	m_col = col;

	return true;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�J�ڃN���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ��ʖ��ߏ���
//========================================
void CTransition::FillScreen(const float& rate) {

	// �|���S��2D�̐ݒu
	RNLib::Polygon2D().Put(1, RNLib::Window().GetCenterPos(), 0.0f, false)
		->SetZTest(false)
		->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	//// �|���S��2D�̐ݒu
	//RNLib::Polygon2D().Put(1, D3DXVECTOR3(0.0f + m_fMove, RNLib::Window().GetHeight() / 3, 0.0f), 0.0f, true)
	//	->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
	//	->SetSize(m_fMove, RNLib::Window().GetHeight() / 2);

	//// �|���S��2D�̐ݒu
	//RNLib::Polygon2D().Put(1, D3DXVECTOR3(RNLib::Window().GetHeight() - m_fMove, RNLib::Window().GetHeight() / 3 - RNLib::Window().GetHeight() / 2, 0.0f), 0.0f, true)
	//	->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
	//	->SetSize(m_fMove, RNLib::Window().GetHeight() / 2);

	m_fMove++;

}