//========================================
// 
// �}�l�[�W���[����
// Author:RIKU NISHIMURA
// 
//========================================
#include "System\words\object\font-object.h"
#include "Sound/stage-sound-player.h"
#include "main.h"
#include "resource.h"
#include "stage.h"

//****************************************
// �}�N����`
//****************************************
#define LIGHT3D_MAX (3)

//****************************************
// �������
//****************************************
namespace {
	//========== [[[ �ϐ��錾 ]]]
	CMode*          m_modeObj;
	CMode::TYPE     m_reserveModeType;
	CStageObjectMgr m_blockMgr;
	CObjectMgr      m_BGMgr("BGMgr");
	CEffMgr         m_effectMgr;
	CStageEditor    m_StgEd;
	CFont           m_Font;
	CCamera*        m_camera;
	CCamera*        m_subCamera;
	CTransition     m_transition;
	CLight3D*       m_light3D[LIGHT3D_MAX];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Manager�̊֐�
//----------|---------------------------------------------------------------------
//================================================================================
CCamera* Manager::GetMainCamera(void) { return m_camera; }
CCamera* Manager::GetSubCamera(void) { return m_subCamera; }
CStageObjectMgr* Manager::StageObjectMgr(void) { return &m_blockMgr; }
CObjectMgr* Manager::BGMgr(void) { return &m_BGMgr; }
CEffMgr* Manager::EffectMgr(void) { return &m_effectMgr; }
CStageEditor* Manager::StgEd(void) { return &m_StgEd; }
CFont* Manager::Font(void) { return &m_Font; }
CTransition& Manager::Transition(void) { return m_transition; }
short& Manager::GetLightIdx(UShort lightIdx) { return m_light3D[lightIdx]->GetID(); }

//========================================
// ����������
// Author:RIKU NISHIMURA
//========================================
void Manager::Init(CMode::TYPE mode) {

	// ���\�[�X�̓ǂݍ���
	CResources::Load();

	// ���C�g3D�̐���
	for (int cnt = 0; cnt < LIGHT3D_MAX; cnt++) {
		switch (cnt) {
		case 0:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\Title.txt"); break;
		case 1:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\World1.txt"); break;
		case 2:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\World2.txt"); break;
		default:
			m_light3D[cnt] = NULL;
			assert(false);
			break;
		}
	}

	// �J�����̐���
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetLightID(m_light3D[0]->GetID());
	m_subCamera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_subCamera->SetLightID(m_light3D[0]->GetID());

	// ���[�h�ݒ�
	SetMode(mode);

	// �X�e�[�W����������
	Stage::Init();

	// �g�p����R���g���[���[�̐����w�肷��
	RNLib::Input().SetJoyPadNum(2);

	// �W���G�t�F�N�g�̗D��x�ݒ�
	RNLib::StandardEffect3D().SetPriority(PRIORITY_EFFECT);

	// �J�ڂ̏�����
	m_transition.Init();
}

//========================================
// �I������
// Author:RIKU NISHIMURA
//========================================
void Manager::Uninit(void) {

	// ���[�h�I�u�W�F�N�g�̏I������
	if (m_modeObj != NULL) {
		m_modeObj->Uninit();
		RNLib::Memory().Release(&m_modeObj);
	}

	// �t�H���g�I�u�W�F�N�g�̏I������
	CFontObject::UpdateAll();

	// �J�����̔j��
	RNLib::Memory().Release(&m_camera);
	RNLib::Memory().Release(&m_subCamera);

	// �X�e�[�W�I������
	Stage::Uninit();

	// �J�ڂ̏I������
	m_transition.Uninit();
}

//========================================
// �X�V����
// Author:RIKU NISHIMURA
//========================================
void Manager::Update(void) {

	{// [[[ �J�����̕`�� ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();

		RNLib::Polygon2D().Put(0, Pos3D(windowCenterPos.x, windowCenterPos.y, 0.0f), 0.0f, true)
			->SetTex(m_camera)
			->SetSize(windowWidth, windowHeight)
			->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
	}

	if (RNSystem::GetMode() == RNSystem::MODE::DEBUG) {
		if (RNLib::Input().GetKeyTrigger(DIK_F9)) {
			RNLib::DrawMgr().SetIsDrawPolygon2D(false, !RNLib::DrawMgr().GetIsDrawPolygon2D(false));
		}
	}

	// �\�񂳂�Ă��鎞�A�J�ڂ����[�h�ݒ�҂��Ȃ烂�[�h��ݒ肷��
	if (m_reserveModeType != CMode::TYPE::NONE) {
		if (m_transition.GetState() == CTransition::STATE::OPEN_WAIT) {
			SetMode(m_reserveModeType);
		}
	}

	// ���[�h�I�u�W�F�N�g���݂鎞�A�X�V����
	if (m_modeObj != NULL) {
		m_modeObj->Update();

		if(m_modeObj != NULL)
		m_modeObj->ProcessState(PROCESS::UPDATE);
	}

	// �t�H���g�I�u�W�F�N�g�̍X�V����
	CFontObject::UpdateAll();

	// �J�ڂ̍X�V����
	m_transition.Update();
}

//========================================
// �`�揈��
// Author:RIKU NISHIMURA
//========================================
void Manager::Draw(void) {

	CFontObject::DrawAll();
}

//========================================
// ���[�h�̐ݒ菈��
// Author:RIKU NISHIMURA
//========================================
void Manager::SetMode(CMode::TYPE newMode) {

	// �V�[�����I��
	RNSystem::EndScene();

	// ���[�h�I�u�W�F�N�g�̏I������
	if (m_modeObj != NULL) {
		m_modeObj->Uninit();
		RNLib::Memory().Release(&m_modeObj);
	}
	// ���[�h�I�u�W�F�N�g��V���ɐ���
	m_modeObj = CMode::Create(newMode);

	// �\�񃂁[�h�𖳂��ɂ���
	m_reserveModeType = CMode::TYPE::NONE;
}

//========================================
// ���[�h�̎擾����
// Author:RIKU NISHIMURA
//========================================
CMode::TYPE Manager::GetMode(void) {

	if (m_modeObj != NULL)
		return m_modeObj->GetType();

	return CMode::TYPE::NONE;
}

//========================================
// �J�ڏ���
// Author:RIKU NISHIMURA
//========================================
void Manager::Transition(CMode::TYPE newMode, CTransition::TYPE transType) {

	// ���[�h��\�񂷂�
	m_reserveModeType = newMode;

	// �J�ڐݒ�
	m_transition.Close(transType, COLOR_WHITE, 60);
}