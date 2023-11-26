//========================================
// 
// �}�l�[�W���[����
// Author:RIKU NISHIMURA
// 
//========================================
#include "System\words\object\font-object.h"
#include "Sound/ambient-sound-player.h"
#include "main.h"

//****************************************
// ���O���
//****************************************
namespace {
	//========== [[[ �ϐ��錾 ]]]
	CMode*          m_modeObj;
	CMode::TYPE     m_reserveModeType;
	CStageObjectMgr m_blockMgr;
	CObjectMgr      m_BGMgr;
	CEffMgr         m_effectMgr;
	CStageEditor    m_StgEd;
	CFont           m_Font;
	CCamera*        m_camera;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Manager�̊֐�
//----------|---------------------------------------------------------------------
//================================================================================
CCamera* Manager::GetMainCamera(void) { return m_camera; }
CStageObjectMgr* Manager::StageObjectMgr(void) { return &m_blockMgr; }
CObjectMgr* Manager::BGMgr(void) { return &m_BGMgr; }
CEffMgr* Manager::EffectMgr(void) { return &m_effectMgr; }
CStageEditor* Manager::StgEd(void) { return &m_StgEd; }
CFont* Manager::Font(void) { return &m_Font; }

//========================================
// ����������
// Author:RIKU NISHIMURA
//========================================
void Manager::Init(CMode::TYPE mode) {

	// �J�����̐���
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));

	// ���[�h�ݒ�
	SetMode(mode);

	// �g�p����R���g���[���[�̐����w�肷��
	RNLib::Input().SetJoyPadNum(2);

	// �u���b�N�̓ǂݍ��ݏ���(����Ƀ��f���Ȃ�)
	CBlock::Load();

	// �����v���C���[�̏���������
	AmbientSoundPlayer::Init();

	// �W���G�t�F�N�g�̗D��x�ݒ�
	RNLib::StandardEffect3D().SetPriority(PRIORITY_EFFECT);
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

	// �����v���C���[�̏I������
	AmbientSoundPlayer::Uninit();
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
			->SetTexUV(m_camera)
			->SetSize(windowWidth, windowHeight);
	}

	// �\�񂳂�Ă��鎞�A�J�ڂ����[�h�ݒ�҂��Ȃ烂�[�h��ݒ肷��
	if (m_reserveModeType != CMode::TYPE::NONE) {
		if (RNLib::Transition().GetState() == CTransition::STATE::OPEN_WAIT) {
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
}

//========================================
// �`�揈��
// Author:RIKU NISHIMURA
//========================================
void Manager::Draw(void) {

	CFontObject::DrawAll();
}

//========================================
// �ǂݍ��ݏ���
// Author:RIKU NISHIMURA
//========================================
void Manager::Load(void) {
	
}

//========================================
// �������ݏ���
// Author:RIKU NISHIMURA
//========================================
void Manager::Save(void) {
	
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
	RNLib::Transition().Close(transType, Color{ 0,0,0,255 }, 30);
}