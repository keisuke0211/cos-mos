//========================================
// 
// ���P�b�g
// Author:KOMURO HIROMU
// 
//========================================
// *** rocket.cpp ***
//========================================
#include "rocket.h"
#include "../../main.h"
#include"../../Character/player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CRocket�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================
const int   CRocket::s_AnimeMax = 120;		// ���������A�j���[�V�����̍ő吔
const int   CRocket::s_RideAnimeMax = 25;	// ��荞�݃A�j���[�V�����̍ő吔
const float CRocket::s_RideAnimeMag = 1.3f;	// �傫��1.0����ɂ���������Z�����傫��	
const float CRocket::s_RideAnimeShrink = 20;// ��荞�݃A�j���[�V�����̏k�ޔ{��
const float CRocket::s_RotAdd = 0.02f;		// �����̑�����
const int   CRocket::s_RotAnimeMax = 4;		// �����݃A�j���[�V�����̍ő�
const float CRocket::s_MoveMag = 1.05f;		// �ړ��ʂ̔{��
const float CRocket::s_MoveAdd = 0.01f;		// �ړ��ʂ̑�����
const float CRocket::s_HeightDis = 40.0f;	// �����̋���
const int   CRocket::s_FadeModeCountMax = 240;	// �t�F�[�h�̃��[�h�̃J�E���g�ő�
const int   CRocket::s_Firerate = 4;		// ���̏o������
const int   CRocket::s_Smokerate = 6;		// ���̏o������
bool        CRocket::s_bReady = false;		
int         CRocket::s_nCountPlayer = 0;	// �v���C���[�̃J�E���g

//========================================
// �R���X�g���N�^
//========================================
CRocket::CRocket(void)
{
	Manager::StageObjectMgr()->AddList(this);

	ResetCounter();

	m_type = TYPE::ROCKET;
	m_width = SIZE_OF_1_SQUARE * 4;
	m_height = SIZE_OF_1_SQUARE * 7;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITD3DCOLOR;
	m_Info.scale = Scale3D(1.0f, 1.0f, 1.0f);
	m_Info.nFlyAnimeCounter = 0;
	m_Info.SmallSpeed = 0.0f;
	m_Info.fScaleMag = 1.0f;
	m_Info.Animstate = CRocket::ANIME_STATE::NONE;
	m_Info.nRideAnimeCounter = 0;
	m_Info.nEffectAnimCounter = 0;
	m_Info.bEffect = false;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Rocket_Body.x");

	m_Info.Firetex.TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Fire_002.png");
	m_Info.Smoketex.TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\Smoke.png");
	m_Info.Firetex.col = Color{ 255,100,0,255 };
	m_Info.Smoketex.col = Color{ 255,255,255,255 };
	m_Info.Firetex.pos = INITD3DXVECTOR3;
	m_Info.Smoketex.pos = INITD3DXVECTOR3;
	m_Info.Firetex.move = D3DXVECTOR3(rand() % 2 - 1 * 0.1f, -0.1f, 0.0f);
	m_Info.Smoketex.move = D3DXVECTOR3(rand() % 2 - 1 * 0.1f, -0.1f, 0.0f);
	m_Info.Firetex.rot = INITD3DXVECTOR3;
	m_Info.Smoketex.rot = INITD3DXVECTOR3;
}

//========================================
// �f�X�g���N�^
//========================================
CRocket::~CRocket()
{
	ResetCounter();
}

//========================================
// ������
//========================================
HRESULT CRocket::Init(void)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITD3DCOLOR;
	if (m_pos.y < 0)
	{// ���̐��E�ɂ���Ƃ����]������
		m_rot.z -= D3DX_PI;
	}
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_Info.Firetex.pos = m_pos;
		m_Info.Smoketex.pos = m_pos;
	}
	return S_OK;
}

//========================================
// �I��
//========================================
void CRocket::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CRocket::Update(void)
{
	//���A�j���[�V�����Ɉړ�
	if (RNLib::Input().GetKeyTrigger(DIK_R) && m_Info.Animstate == CRocket::ANIME_STATE::RIDE)
	{
		m_Info.Animstate = CRocket::ANIME_STATE::FLY;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_R) && m_Info.Animstate == CRocket::ANIME_STATE::NONE)
	{
		m_Info.Animstate = CRocket::ANIME_STATE::RIDE;
		Ride();
	}

	switch (m_Info.Animstate)
	{
	case CRocket::ANIME_STATE::NONE:
		break;
	case CRocket::ANIME_STATE::RIDE:
		m_Info.bEffect = true;
		UpdateState_Ride();		// ��яo��������Ԃ̍X�V
	
		break;
	case CRocket::ANIME_STATE::FLY:
		UpdateState_Fly();		// ��яo����Ԃ̍X�V
		
		break;
	}

	if (m_Info.bEffect == true)
	{
		m_Info.nEffectAnimCounter++;
		if (m_Info.nEffectAnimCounter % s_Smokerate == 0)
		{
			m_Info.Smoketex.move = D3DXVECTOR3(rand() % 6 - 3, rand() % 4 - 3, 0.0f);

			// ���̃G�t�F�N�g
			Manager::EffectMgr()->EffectCreate(m_Info.Smoketex.TexIdx, D3DXVECTOR3(m_pos.x - 2.0f, m_pos.y - s_HeightDis, m_pos.z), Scale3D(30.0f, 30.0f, 30.0f), m_Info.Smoketex.col, 120, D3DXVECTOR3(0.0f, 0.0f, 0.1f), m_Info.Smoketex.move, false, D3DXVECTOR3(1.05f, 1.05f, 1.00f));
		}
		if (m_Info.nEffectAnimCounter % s_Firerate == 0 && m_Info.Animstate == CRocket::ANIME_STATE::FLY)
		{
			m_Info.Firetex.move = D3DXVECTOR3(0.0f, -0.1f, 0.0f);

			// ���̃G�t�F�N�g
			Manager::EffectMgr()->EffectCreate(m_Info.Firetex.TexIdx, D3DXVECTOR3(m_pos.x - rand() % 4 - 2, m_pos.y - s_HeightDis, m_pos.z), Scale3D(30.0f, 30.0f, 30.0f), m_Info.Firetex.col, 160, D3DXVECTOR3(0.0f, 0.0f, 0.1f), m_Info.Firetex.move, false);
		}
	}
	
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, m_rot + Rot3D(0.0f, D3DX_PI, 0.0f), m_Info.scale * m_Info.fScaleMag, false)
		->SetOutLine(true);

	RNLib::Text2D().PutDebugLog(CreateText("�v���C���[�J�E���^�[�F%d", s_nCountPlayer));
}
//========================================
// ����ԍX�V
//========================================
void CRocket::UpdateState_Ride(void)
{
	m_Info.nRideAnimeCounter++;	// �A�j���[�V�����̑���

	if (m_Info.nRideAnimeCounter <= 1)
	{// �傫������
	}
	else if (m_Info.nRideAnimeCounter <= s_RideAnimeMax)
	{// ����������
		m_Info.fScaleMag -= m_Info.SmallSpeed;	// �X�P�[���{���̌��Z
		if (m_Info.fScaleMag <= 1.0f)
		{
			m_Info.fScaleMag = 1.0f;
		}
	}
	else if (m_Info.nRideAnimeCounter <= (s_RideAnimeMax + s_RideAnimeShrink) * 4)
	{// �A�j���[�V�����̈ڍs

		if (s_nCountPlayer == CPlayer::NUM_PLAYER)
		{// �v���C���[���S���������
			m_Info.Animstate = ANIME_STATE::FLY;	// ��ԏ�ԂɈڍs
		}
		m_Info.nRideAnimeCounter = 0;					// ���A�j���[�V�����J�E���^�[��������
	}
}
//========================================
// ��ԏ�ԍX�V
//========================================
void CRocket::UpdateState_Fly(void)
{
	int nCounter;

	m_Info.nFlyAnimeCounter++;	// �A�j���[�V�����̑���
	nCounter = m_Info.nFlyAnimeCounter % s_RotAnimeMax;	// �����̌v�Z

	// ���[�h�̐؂�ւ�	
	if (m_Info.nFlyAnimeCounter >= s_FadeModeCountMax)
	{
		int stage = Manager::StgEd()->GetType()->nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

	// ���������������
	if (nCounter >= s_RotAnimeMax * 0.5f)
	{
		m_rot.z += s_RotAdd;
	}
	else
	{
		m_rot.z -= s_RotAdd;
	}

	if (m_Info.nFlyAnimeCounter >= s_AnimeMax)
	{
		m_Info.move.y *= s_MoveMag;	// �ړ��ʂɔ{����������

		if (m_pos.y >= 0)
		{// ��̐��E�ɂ���Ƃ�
			m_Info.move.y += s_MoveAdd;
		}
		else
		{// ���̐��E�ɂ���Ƃ�
			m_Info.move.y -= s_MoveAdd;
		}
	}

	
	m_pos += m_Info.move;	// �ʒu�Ɉړ��ʂ̑���
}
//========================================
// ����Ă���
//========================================
void CRocket::Ride(void)
{
	s_nCountPlayer++;												// �v���C���[�̏�����l���̑���
	m_Info.fScaleMag = s_RideAnimeMag;								// �X�P�[���{���̐ݒ�
	m_Info.SmallSpeed = (m_Info.fScaleMag - 1.0f) / s_RideAnimeMax;	// �������Ȃ鑬�x�̐ݒ�
	m_Info.nRideAnimeCounter = 0;									// ���A�j���[�V�����J�E���^�[��������
	m_Info.nFlyAnimeCounter = 0;									// ��ԃA�j���[�V�����J�E���^�[��������

	m_Info.Animstate = ANIME_STATE::RIDE;		// ����ԂɈڍs

}