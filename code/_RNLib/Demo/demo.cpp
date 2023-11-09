//========================================
// 
// �f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �f���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CDemo::CDemo() {

	for (int cntPlayer = 0; cntPlayer < (int)PLAYER::MAX; m_players[cntPlayer] = NULL, cntPlayer++);
	m_controlPlayerCount = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CDemo::~CDemo() {

}

//========================================
// ����������
//========================================
void CDemo::Init(void) {

	// �v���C���[�𐶐�
	m_players[(int)PLAYER::RED] = new CDemoPlayer(Pos3D(0.0f, 0.0f, -40.0f), Rot3D(0.0f, 0.0f, 0.0f), Color{ 255,0,0,255 }, Color{ 255,128,128,255 });
	m_players[(int)PLAYER::BLUE] = new CDemoPlayer(Pos3D(0.0f, 0.0f, 40.0f), Rot3D(0.0f, D3DX_PI, 0.0f), Color{ 0,0,255,255 }, Color{ 128,128,255,255 });

	// �v���C���[�̃N���b�s���O�J����ID��ݒ�
	m_players[(int)PLAYER::RED]->SetClippingCameraID(m_players[(int)PLAYER::BLUE]->GetCamera().GetID());
	m_players[(int)PLAYER::BLUE]->SetClippingCameraID(m_players[(int)PLAYER::RED]->GetCamera().GetID());

	// ����ݒ�
	CDemoPlayer::SetControl(m_players[m_controlPlayerCount]);
}

//========================================
// �I������
//========================================
void CDemo::Uninit(void) {

	for (int cntPlayer = 0; cntPlayer < (int)PLAYER::MAX; cntPlayer++)
		CMemory::Release(&m_players[cntPlayer]);
}

//========================================
// �X�V����
//========================================
void CDemo::Update(void) {

	//----------------------------------------
	// ����
	//----------------------------------------
	// ����v���C���[�؂�ւ�
	if (RNLib::Input().GetKeyTrigger(DIK_1)) {
		m_controlPlayerCount = (m_controlPlayerCount + 1) % (int)PLAYER::MAX;
		CDemoPlayer::SetControl(m_players[m_controlPlayerCount]);
	}

	// �J�����̃N���b�s���O�I���I�t�؂�ւ�
	if (RNLib::Input().GetKeyTrigger(DIK_2)) {
		CDemoPlayer::SetCameraClipping(!CDemoPlayer::GetCameraClipping());
	}

	//----------------------------------------
	// �`��
	//----------------------------------------
	{// [[[ �v���C���[�̎��_�`�� ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();
		const float windowHeightHalf  = windowHeight * 0.5f;
		const float windowHeightHalf2 = windowHeightHalf * 0.5f;

		// ��(��)
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y - windowHeightHalf2, 0.0f), 0.0f, true)
			->SetTex_Camera(&m_players[(int)PLAYER::RED]->GetCamera())
			->SetSize(windowWidth, windowHeightHalf);

		// ��(��)
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y + windowHeightHalf2, 0.0f), 0.0f, true)
			->SetTex_Camera(&m_players[(int)PLAYER::BLUE]->GetCamera())
			->SetSize(windowWidth, windowHeightHalf);
	}

	//----------------------------------------
	// �`��I�u�W�F�N�g
	// �� (�I�u�W�F�N�g��)
	//----------------------------------------
	{// �� �N���b�s���O�X�t�B�A
		Pos3D basePos = INITPOS3D;

		// ��
		RNLib::Model().Put(basePos + Pos3D(10.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 255, 0, 0, 255 })
			->SetOutLine(true)
			->SetClippingCamera(m_players[(int)PLAYER::BLUE]->GetCamera());	// ���v���C���[�̃J�����ɂ̂ݕ`��

		// ��
		RNLib::Model().Put(basePos + Pos3D(0.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 255, 255, 255, 255 })
			->SetOutLine(true);

		// ��
		RNLib::Model().Put(basePos + Pos3D(-10.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 0, 0, 255, 255 })
			->SetOutLine(true)
			->SetClippingCamera(m_players[(int)PLAYER::RED]->GetCamera());	// ���ԃv���C���[�̃J�����ɂ̂ݕ`��

		// ��
		RNLib::Polygon3D().Put(basePos + Pos3D(0.0f, -5.0f, 0.0f), Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(100.0f, 100.0f)
			->SetCol(Color{ 128, 128, 128, 255 });
	}
}