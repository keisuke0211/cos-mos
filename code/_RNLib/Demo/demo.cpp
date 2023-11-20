//========================================
// 
// �f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_player.h"
#include "demo_zone.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �萔��`
//========================================
const char* CDemo::PLAYER_NAMES[(int)PLAYER::MAX] = { "RED", "BLUE"};
const char* CDemo::SCREEN_TYPE_NAMES[(int)SCREEN_TYPE::MAX] = { "NORMAL", "DIVISION"};

//========================================
// �R���X�g���N�^
//========================================
CDemo::CDemo() {

	for (int cntPlayer = 0; cntPlayer < (int)PLAYER::MAX; m_players[cntPlayer] = NULL, cntPlayer++);
	m_controlPlayerCount = 0;
	m_screenTypeCount = 0;
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

	// �]�[������
	for (int cntZone = 0; cntZone < (int)CDemoZone::TYPE2::MAX; cntZone++)
		new CDemoZone((CDemoZone::TYPE2)cntZone);
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

	RNLib::Text2D().PutDebugLog(CreateText("FPS             :%d", RNSystem::GetFPS()));
	RNLib::Text2D().PutDebugLog(CreateText("DrawFPS         :%d", RNSystem::GetDrawFPS()));
	RNLib::Text2D().PutDebugLog(CreateText("DrawWaitTime    :%d", RNLib::DrawMgr().GetWaitMilliseconds()));
	RNLib::Text2D().PutDebugLog(CreateText("ControlPlayer[Q]:%s", PLAYER_NAMES[m_controlPlayerCount]));
	RNLib::Text2D().PutDebugLog(CreateText("ScreenType   [E]:%s", SCREEN_TYPE_NAMES[m_screenTypeCount]));

	{// [[[ ���� ]]]
		// ����v���C���[�؂�ւ�
		if (RNLib::Input().GetKeyTrigger(DIK_Q)) {
			m_controlPlayerCount = (m_controlPlayerCount + 1) % (int)PLAYER::MAX;
			CDemoPlayer::SetControl(m_players[m_controlPlayerCount]);
		}

		// �X�N���[���^�C�v�؂�ւ�
		if (RNLib::Input().GetKeyTrigger(DIK_E)) {
			m_screenTypeCount = (m_screenTypeCount + 1) % (int)SCREEN_TYPE::MAX;
		}
	}

	{// [[[ �v���C���[�̎��_�`�� ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();
		const float windowHeightHalf  = windowHeight * 0.5f;
		const float windowHeightHalf2 = windowHeightHalf * 0.5f;

		switch ((SCREEN_TYPE)m_screenTypeCount) {
		case SCREEN_TYPE::NORMAL: {

			RNLib::Polygon2D().Put(0, Pos3D(windowCenterPos.x, windowCenterPos.y, 0.0f), 0.0f, true)
				->SetTex(&CDemoPlayer::GetControl()->GetCamera())
				->SetSize(windowWidth, windowHeight);

		}break;
		case SCREEN_TYPE::DIVISION: {

			// ��(��)
			RNLib::Polygon2D().Put(0, Pos3D(windowCenterPos.x, windowCenterPos.y - windowHeightHalf2, 0.0f), 0.0f, true)
				->SetTexUV(
					&m_players[(int)PLAYER::RED]->GetCamera(),
					Pos2D(0.0f, 0.25f),
					Pos2D(1.0f, 0.25f),
					Pos2D(0.0f, 0.75f),
					Pos2D(1.0f, 0.75f))
				->SetCol(CDemoPlayer::GetControl() == m_players[(int)PLAYER::RED] ? Color{ 255,255,255,255 } : Color{ 128,128,128,255 })
				->SetSize(windowWidth, windowHeightHalf);

			// ��(��)
			RNLib::Polygon2D().Put(0, Pos3D(windowCenterPos.x, windowCenterPos.y + windowHeightHalf2, 0.0f), 0.0f, true)
				->SetTexUV(
					&m_players[(int)PLAYER::BLUE]->GetCamera(),
					Pos2D(0.0f, 0.25f),
					Pos2D(1.0f, 0.25f),
					Pos2D(0.0f, 0.75f),
					Pos2D(1.0f, 0.75f))
				->SetCol(CDemoPlayer::GetControl() == m_players[(int)PLAYER::BLUE] ? Color{ 255,255,255,255 } : Color{ 128,128,128,255 })
				->SetSize(windowWidth, windowHeightHalf);

		}break;
		}
	}
}