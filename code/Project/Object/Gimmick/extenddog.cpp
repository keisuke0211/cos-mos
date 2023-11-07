//========================================
// 
// �L�т錢�̏���
// Author:KOMURO HIROMU
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
#include "../../../_RNLib/Basis/input.h"
#include "../../Character/player.h"


#define MAX_COUNT		(40)	//�ő�J�E���g��
#define RADIUS_WIDTH	(0.5f)	//�����a
#define RADIUS_HEIGHT	(0.5f)	//�c���a
#define CORRECT_HEIGHT	(15.0f)	//�����␳
#define HIP_POS			(7.0f)	//�K�̈ʒu

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampoline�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CExtenddog::CExtenddog(void) {
	Manager::BlockMgr()->AddList(this);

	//�������
	m_type = TYPE::EXTENDDOG;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 2.0f;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLand = false;
	m_bShrink = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_House.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHip.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHead.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[4] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_modelIdx[5] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Body.x");
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
	m_fHeadposx = 0.0f;
	m_fHipposx = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// ����������
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Init(void) {

}

//========================================
// �I������
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// �X�V����
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Update(void) {

	//�y�䃂�f��
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);

	if (m_state != STATE::NONE)
	{//�L�т錢���쓮���Ă���

		if (m_state == STATE::DOWN_LAND)
		{
			if (m_bShrink = false)
			{
				// �k�ރJ�E���g
				m_nCntShrink--;
				if (m_nCntShrink <= 0)
					m_nCntShrink = 0;
			}
			else
			{
				// �k�ރJ�E���g
				m_nCntShrink++;
				if (m_nCntShrink >= MAX_COUNT)
					m_nCntShrink = MAX_COUNT;
			}
		}
	}
	else if (m_state == STATE::NONE)
	{//�L�т錢���쓮���Ă��Ȃ�

		if (m_bShrink = false)
		{
			// �k�ރJ�E���g
			m_nCntShrink++;
			if (m_nCntShrink >= MAX_COUNT)
				m_nCntShrink = MAX_COUNT;
		}
		else
		{
			// �k�ރJ�E���g
			m_nCntShrink--;
			if (m_nCntShrink <= 0)
				m_nCntShrink = 0;
		}
	}

	// �����v�Z 
	float fCountRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);

	//y���W�̍X�V
	float fDowncurrenty = m_pos.y + (CORRECT_HEIGHT * 3 - (fCountRate * (CORRECT_HEIGHT * 2)));

	RNLib::Model().Put(D3DXVECTOR3(m_fHipposx, m_pos.y - HIP_POS, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
		->SetOutLine(true);
	RNLib::Model().Put(D3DXVECTOR3(m_fHeadposx, fDowncurrenty, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
		->SetOutLine(true);
	RNLib::Model().Put(D3DXVECTOR3(m_fHeadposx, m_pos.y + 10.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), Scale3D(1.0f, fDowncurrenty + CORRECT_HEIGHT * 3, 1.0f), m_modelIdx[5], false)
		->SetOutLine(true);
	//�����蔻��
	Collision();
}
//========================================
// �`�揈��
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Draw(void) {

}
//========================================
// �����蔻�菈��
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Collision(void) {

	// ��Ԃ̐ݒ�
	m_state = STATE::NONE;

	//�v���C���[���擾
	CPlayer::Info *p1, *p2;
	CPlayer *pPlayer = CMode_Game::GetPlayer();
	if (pPlayer == NULL)
		return;
	pPlayer->GetInfo(p1, p2);

	float width, height;

	width = m_width * RADIUS_WIDTH;
	height = m_height * RADIUS_HEIGHT;

	//**************************************
	//1p�L�т錢�����蔻��
	//**************************************
	if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width && p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height
		&& p1->side == CPlayer::WORLD_SIDE::BEHIND)
	{
		p1->pos.x = p1->posOLd.x;
	}
	if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width && p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width)
	{
		m_state = STATE::DOWN_LAND;

	}
	if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width && p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height
		&& p1->side == CPlayer::WORLD_SIDE::BEHIND)
	{//�y��͈͓̔��ɒ��n�œ�����


	}
	//**************************************
	//2p�L�т錢�����蔻��
	//**************************************
	if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width && p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height
		&& p2->side == CPlayer::WORLD_SIDE::BEHIND)
	{
		p2->pos.x = p2->posOLd.x;
	}
	if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height
		&& p2->side == CPlayer::WORLD_SIDE::BEHIND)
	{//�y��͈͓̔��ɒ��n�œ�����

	

		m_state = STATE::DOWN_LAND;



	}
}