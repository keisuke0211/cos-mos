//========================================
// 
// �f���v���C���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �f���v���C���[�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
CDemoPlayer* CDemoPlayer::ms_control = NULL;
bool CDemoPlayer::ms_isCameraClipping = false;

//========================================
// �R���X�g���N�^
//========================================
CDemoPlayer::CDemoPlayer(const Pos3D& pos, const Rot3D& rot, const Color& col, const Color& cameraCol) {

	// ���X�g�ɒǉ�
	RNDemo::Get().GetDemoObjMgr().AddList(this);

	m_pos    = pos;
	m_rot    = rot;
	m_col    = col;
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight() * 0.5f));
	m_camera->SetBGCol(cameraCol);
	m_clippingCameraID = NONEDATA;
}

//========================================
// �f�X�g���N�^
//========================================
CDemoPlayer::~CDemoPlayer() {

	// ���X�g����폜
	RNDemo::Get().GetDemoObjMgr().SubList(this);

	// �J������j��
	CMemory::Release(&m_camera);
}

//========================================
// �X�V����
//========================================
void CDemoPlayer::Update(void) {

	// ����
	if (ms_control == this) {

		float moveForce = 0.0f;

		// �O�i
		if (RNLib::Input().GetKeyPress(DIK_UP))
			moveForce = 0.5f;

		// ��i
		if (RNLib::Input().GetKeyPress(DIK_DOWN))
			moveForce = -0.5f;
		
		// ����]
		if (RNLib::Input().GetKeyPress(DIK_LEFT))
			m_rot.y -= 0.025f;

		// �E��]
		if (RNLib::Input().GetKeyPress(DIK_RIGHT))
			m_rot.y += 0.025f;

		// �ړ�
		m_pos.x += sinf(m_rot.y) * moveForce;
		m_pos.z += cosf(m_rot.y) * moveForce;
	}

	{// �J�����̐ݒ�
		Pos3D posV = m_pos + Pos3D(0.0f, 10.0f, 0.0f);
		Pos3D posR = posV;
		posR.x += sinf(m_rot.y);
		posR.z += cosf(m_rot.y);
		m_camera->SetPosVAndPosR(posV, posR);
		m_camera->SetClipping(ms_isCameraClipping);
	}

	// ���f���z�u
	RNLib::Model().Put(m_pos, m_rot, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::OBSERVER))
		->SetCol(m_col)
		->SetOutLine(true)
		->SetClippingCamera(m_clippingCameraID);
}