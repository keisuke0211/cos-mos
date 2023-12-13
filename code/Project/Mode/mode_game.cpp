//========================================
// 
// ���[�h:�Q�[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "../main.h"
#include "../Character/player.h"
#include "../System/StageEditor.h"
#include "../System/words/font-text.h"
#include "../Mode/mode_title.h"
#include "../UI/partsUI.h"
#include "../UI/coinUI.h"
#include "../System/BG-Editor.h"
#include "../Sound/stage-sound-player.h"
#include "../resource.h"
#include "../stage.h"
#include "../UI/MenuUi.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Game�N���X
//----------|---------------------------------------------------------------------
//================================================================================
CMenuUI *CMode_Game::m_MenuUI = NULL;
static const int s_PlanetMaxSummon = 8;		// �o������ʒu�̍ő吔
static const int s_StarMaxSummon = 10;		// �o������ʒu�̍ő吔

//========================================
// �R���X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CMode_Game::CMode_Game(void) {

}

//========================================
// �f�X�g���N�^
// Author:RIKU NISHIMURA
//========================================
CMode_Game::~CMode_Game(void) {
	if (m_MenuUI != NULL){
		delete m_MenuUI;
		m_MenuUI = NULL;
	}
}

//========================================
// ����������
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Init(void) {
	CMode::Init();

	// �X�e�[�W�J�n����
	Stage::StartStage();

	// �J�ڐݒ�
	RNLib::Transition().Open(CTransition::TYPE::FADE, 30);

	// ��Ԑݒ�
	SetState((int)STATE::NONE);

	// �X�e�[�W�I�u�W�F�N�g�̓ǂݍ���
	Manager::StageObjectMgr()->Load();

	// ���j���[����
	m_MenuUI = CMenuUI::Create(CMode::TYPE::GAME);
}

//========================================
// �I������
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Uninit(void) {
	CMode::Uninit();

	// �X�e�[�W�I������
	Stage::EndStage();
}

//========================================
// �X�V����
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Update(void) {
	CMode::Update();

	// �X�e�[�W�X�V����
	Stage::UpdateStage();

	// [[[ ��|�[�Y���̏��� ]]]
	if (m_state != (int)STATE::PAUSE) {

		// �|�[�Y
		if (!CPlayer::GetSwapAnim() && !CPlayer::GetDeath() && CPlayer::GetZoomUpCounter() == 0) {
			if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START)) {
				SetState((int)STATE::PAUSE);
			}
		}
	}
}

//========================================
// �X�V����(���)
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::ProcessState(const PROCESS process) {
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
	case STATE::PAUSE: {
		switch (process) {
			// [[[ �������� ]]]
		case PROCESS::INIT: {

			m_MenuUI->MenuCreate();

			// �X�e�[�W���|�[�Y��Ԃ�
			Stage::SetPause(true);
		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {

			// �X�e�[�W���|�[�Y��Ԃ�
			Stage::SetPause(false);
		}break;
			// [[[ �X�V���� ]]]
		case PROCESS::UPDATE: {

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, 0.0f), 0.0f, false)
				->SetSize(RNLib::Window().GetCenterX() * 2, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 0,0,0,120 });

			m_MenuUI->Update();

			bool MenuEnd = m_MenuUI->m_MenuEnd;

			if (MenuEnd) {
				m_MenuUI->TextRelease(CMenuUI::TEXT_ALL);
				ProcessState(PROCESS::UNINIT);

				if (RNLib::Transition().GetState() == CTransition::STATE::NONE) {
					SetState((int)STATE::NONE);
				}
			}
		}break;
		}
	}break;
	}
}