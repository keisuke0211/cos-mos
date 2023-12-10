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
//==========| [���J]�J�ڃN���X
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
	m_nTexIdx = 0;
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
	m_nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\Load.png");
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
//==========| [����J]�J�ڃN���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ��ʖ��ߏ���
//========================================
void CTransition::FillScreen(const float& rate) {

	const float screenWidth = RNLib::Window().GetWidth();
	const float screenHeight = RNLib::Window().GetHeight();
	const float screenHeightHalf = screenHeight * 0.5f;

	{// [[[ �� ]]]
		float right1 = (screenWidth + screenHeightHalf) * rate;
		float right2 = right1 - screenHeightHalf;

		// ����
		RNLib::Polygon2D().Put(1, true)
			->SetCol(m_col)
			->SetVtxPos(Pos2D(0.0f, 0.0f), Pos2D(right2, 0.0f), Pos2D(0.0f, screenHeightHalf), Pos2D(right2, screenHeightHalf));

		// ��
		RNLib::Polygon2D().Put(1, true)
			->SetCol(m_col)
			->SetTex(m_nTexIdx)
			->SetTexMirrorX(true)
			->SetVtxPos(Pos2D(right2 - 0.1f, 0.0f), Pos2D(right1, 0.0f), Pos2D(right2 - 0.1f, screenHeightHalf), Pos2D(right1, screenHeightHalf));
	}

	{// [[[ �� ]]]
		float left1 = screenWidth - ((screenWidth + screenHeightHalf) * rate);
		float left2 = left1 + screenHeightHalf;

		// ����
		RNLib::Polygon2D().Put(1, true)
			->SetCol(m_col)
			->SetVtxPos(Pos2D(left2, screenHeightHalf), Pos2D(screenWidth, screenHeightHalf), Pos2D(left2, screenHeight), Pos2D(screenWidth, screenHeight));

		// ��
		RNLib::Polygon2D().Put(1, true)
			->SetCol(m_col)
			->SetTex(m_nTexIdx)
			->SetVtxPos(Pos2D(left1, screenHeightHalf), Pos2D(left2 + 0.1f, screenHeightHalf), Pos2D(left1, screenHeight), Pos2D(left2 + 0.1f, screenHeight));
	}
}