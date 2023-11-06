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
	m_type = TYPE::TRAMPOLINE;
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLand = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_House.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHip.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHead.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[4] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_nCntExtend = 0;
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// ����������
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Init(void) {

}

//========================================
// �I������
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// �X�V����
// Author:RYUKI FUJIWARA
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
			m_nCntShrink = 0;	// ������

			m_nCntExtend++;		// ����

								//�����v�Z
			float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntExtend, MAX_COUNT);


			if (fCountRate >= 1.0f)
			{
				fCountRate = 1.0f;
			}

			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
				->SetOutLine(true);
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y + (m_fcurrenty - fCountRate * (m_fcurrenty)), m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
				->SetOutLine(true);
		}

	}
	else if (m_state == STATE::NONE)
	{//�L�т錢���쓮���Ă��Ȃ�

		m_nCntExtend = 0;	// ������

		m_nCntShrink++;		// ����

							//�����v�Z
		float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntShrink, MAX_COUNT);

		if (fCountRate >= 1.0f)
		{
			fCountRate = 1.0f;
		}

		//y���W�̍X�V
		m_fcurrenty = m_pos.y + (CORRECT_HEIGHT + fCountRate * (CORRECT_HEIGHT * 2));

		RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
			->SetOutLine(true);
		RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_fcurrenty, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
			->SetOutLine(true);
	}

	//�����蔻��
	Collision();
}
//========================================
// �`�揈��
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Draw(void) {

}
//========================================
// �����蔻�菈��
// Author:RYUKI FUJIWARA
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
		&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height)
	{//�y��͈͓̔��ɒ��n�œ�����

		if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - height
			&& p2->side == CPlayer::WORLD_SIDE::BEHIND)
		{//2p������Ă��邩


		}
		else if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + height
			&& p2->side == CPlayer::WORLD_SIDE::FACE)
		{

		}

		if (p1->side == CPlayer::WORLD_SIDE::BEHIND)
		{//�L�т錢���쓮���Ă��Ȃ�

			m_state = STATE::DOWN_LAND;
		}


	}
	//**************************************
	//2p�L�т錢�����蔻��
	//**************************************
	if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height)
	{//�y��͈͓̔��ɒ��n�œ�����

		if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + height
			&& p1->side == CPlayer::WORLD_SIDE::FACE)
		{//1p������Ă��邩


		}
		else if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - height
			&& p1->side == CPlayer::WORLD_SIDE::BEHIND)
		{

		}

		if (p2->side == CPlayer::WORLD_SIDE::BEHIND)
		{//�L�т錢���쓮���Ă��Ȃ�

			m_state = STATE::DOWN_LAND;
		}


	}
}