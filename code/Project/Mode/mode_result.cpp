//========================================
// 
// ���[�h:���U���g����
// Author:KEISUKE OTONO
// 
//========================================
#include "../main.h"
#include "mode_result.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Result�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
// Author:KEISUKE OTONO
//========================================
CMode_Result::CMode_Result(void) {

}

//========================================
// �f�X�g���N�^
// Author:KEISUKE OTONO
//========================================
CMode_Result::~CMode_Result(void) {

}

//========================================
// ����������
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Init(void) {
	CMode::Init();

	// �J�ڐݒ�
	Manager::Transition().Open(CTransition::TYPE::NUI, 60);

	// �J�����̎��_/�����_��ݒ�
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ��Ԑݒ�
	SetState((int)STATE::NONE);
}

//========================================
// �I������
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Uninit(void) {
	CMode::Uninit();

}

//========================================
// �X�V����
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Update(void) {
	CMode::Update();

	if (RNLib::Input().GetKeyTrigger(DIK_SPACE) && Manager::Transition().GetState() == CTransition::STATE::NONE)
		Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::NUI);

}

//========================================
// �X�V����(���)
// Author:KEISUKE OTONO
//========================================
void CMode_Result::ProcessState(const PROCESS process) {
	switch ((STATE)m_state) {
		//----------------------------------------
		// ����
		//----------------------------------------
	case STATE::NONE: {
		switch (process) {
			// [[[ �������� ]]]
		case PROCESS::INIT: {

		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {

		}break;
			// [[[ �X�V���� ]]]
		case PROCESS::UPDATE: {

		}break;
		}
	}break;
	}
}