//========================================
// 
// �g�����|�����̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#include "trampoline.h"
#include "../../main.h"
#include "../../Character/player.h"


#define PLAYER_FLAME	(6)		//�v���C���[�̍������B�J�E���g
#define RADIUS_WIDTH	(0.5f)	//�����a
#define RADIUS_HEIGHT	(0.5f)	//�c���a
#define CORRECT_WIDTH	(8.0f)	//�����␳
#define CORRECT_HEIGHT	(6.0f)	//�����␳

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampoline�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CTrampoline::CTrampoline(void) {
	Manager::BlockMgr()->AddList(this);

	//�������
	m_type = TYPE::TRAMPOLINE;
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\Spring_Body.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\Spring_Footing.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\Spring_Spring.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\Spring_Eye.x");
	m_fJamp = 8.0f;
	m_nCnt = 1;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_SpringPos[nCnt] = INITD3DXVECTOR3;
	}
	m_fSpringForce = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CTrampoline::~CTrampoline(void) {

}

//========================================
// ����������
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Init(void) {
	m_SpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
	m_SpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
}

//========================================
// �I������
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Uninit(void) {

}

//========================================
// �X�V����
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Update(void) {

	//�y�䃂�f��
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);

	//�ڋʃ��f��
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x + CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x - CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);

	if (m_nCnt > 0) 
	{
		m_nCnt--;
		if (m_nCnt == 0) {
			m_state = STATE::NONE;
			m_SpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
			m_SpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
		}
	}

	if (m_state != STATE::NONE)
	{//�g�����|�������쓮���Ă���

		//�����v�Z
		float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		
		if (m_state == STATE::UP_LAND)
		{
			//�L�m�R
			RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
				->SetOutLine(true);

			m_SpringPos[1].y = m_pos.y - CORRECT_HEIGHT - m_fJamp * fCountRate;
			RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[1], false)
				->SetOutLine(true);

			//�΂�
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - m_fJamp * fCountRate, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[2], false)
				->SetOutLine(true);
		}
		else if (m_state == STATE::DOWN_LAND)
		{
			//�L�m�R
			m_SpringPos[0].y = m_pos.y + CORRECT_HEIGHT + m_fJamp * fCountRate;
			RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
				->SetOutLine(true);

			RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f,D3DX_PI), m_modelIdx[1], false)
				->SetOutLine(true);

			//�΂�
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y + m_fJamp * fCountRate, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[2], false)
				->SetOutLine(true);
		}
	}
	else if (m_state == STATE::NONE)
	{//�g�����|�������쓮���Ă��Ȃ�
		RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
			->SetOutLine(true);
		RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[1], false)
			->SetOutLine(true);
	}

	//�����蔻��
	Collision();
}
//========================================
// �`�揈��
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Draw(void) {

}
//========================================
// �����蔻�菈��
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Collision(void) {

	//�v���C���[���擾
	CPlayer::Info *Player[2];
	CPlayer *pPlayer = CMode_Game::GetPlayer();
	if (pPlayer == NULL)
		return;
	pPlayer->GetInfo(Player[0], Player[1]);

	float widthHalf, height;

	widthHalf = m_width * RADIUS_WIDTH;

	return;
	//**************************************
	// �g�����|���������蔻��
	// Arrange:RIKU NISHIMURA
	//**************************************
	for (int nCnt = 0; nCnt < 2; nCnt++) {
		// ���ϊ�
		float&      fP_PosX    = Player[nCnt]->pos.x;
		float&      fP_PosY    = Player[nCnt]->pos.y;
		const float fP_Top     = fP_PosY + CPlayer::SIZE_HEIGHT;
		const float fP_Bottom  = fP_PosY - CPlayer::SIZE_HEIGHT;
		const float fP_Left    = fP_PosX - CPlayer::SIZE_WIDTH;
		const float fP_Right   = fP_PosX + CPlayer::SIZE_WIDTH;
		float&      fS0_PosX   = m_SpringPos[0].x;
		float&      fS0_PosY   = m_SpringPos[0].y;
		const float fS0_Top    = fS0_PosY + m_height;
		const float fS0_Bottom = fS0_PosY - m_height;
		const float fS0_Left   = fS0_PosX - widthHalf;
		const float fS0_Right  = fS0_PosX + widthHalf;
		float&      fS1_PosX   = m_SpringPos[1].x;
		float&      fS1_PosY   = m_SpringPos[1].y;
		const float fS1_Top    = fS0_PosY + m_height;
		const float fS1_Bottom = fS0_PosY - m_height;
		const float fS1_Left   = fS0_PosX - widthHalf;
		const float fS1_Right  = fS0_PosX + widthHalf;

		// ( ���n�̏u�Ԃ��A�g�����|�����̕����ݒ�ς� )�łȂ���ΐ܂�Ԃ�
		if (!(Player[nCnt]->bGround == true && Player[nCnt]->bGroundOld == false && Player[nCnt]->TramColliRot == CPlayer::COLLI_ROT::NONE))
			continue;

		// �o�l�̏㉺�ǂ���Ƃ��ڐG���Ă��Ȃ���ΐ܂�Ԃ�
		const bool isColl0 = (fP_Right >= fS0_Left && fP_Left <= fS0_Right && fP_Bottom <= fS0_Top && fP_Top >= fS0_Bottom);
		const bool isColl1 = (fP_Right >= fS1_Left && fP_Left <= fS1_Right && fP_Bottom <= fS1_Top && fP_Top >= fS1_Bottom);
		if (!isColl0 && !isColl1)
			continue;

		// 0(��)�̃o�l�ɐڐG���Ă��鎞�A
		// �������ɓ������Ă��āA���㐢�E�ł���΁A
		if (isColl0 && Player[nCnt]->TramColliRot == CPlayer::COLLI_ROT::UNDER && Player[nCnt]->side == CPlayer::WORLD_SIDE::BEHIND) {
			pPlayer->SetTrampolineJump(Player[nCnt], Player[nCnt]->fMaxHeight);
		}
		// 1(��)�̃o�l�ɐڐG���Ă��鎞�A
		// ������ɓ������Ă��āA�������E�ł���΁A
		else if (isColl0 && Player[nCnt]->TramColliRot == CPlayer::COLLI_ROT::OVER && Player[nCnt]->side == CPlayer::WORLD_SIDE::BEHIND) {
			pPlayer->SetTrampolineJump(Player[nCnt], Player[nCnt]->fMaxHeight);
		}

		if (m_state == STATE::NONE
			&& Player[nCnt]->side == CPlayer::WORLD_SIDE::BEHIND)
		{
			m_state = STATE::DOWN_LAND;
			m_nCnt = MAX_COUNT;
		}
		else if (m_state == STATE::NONE
			&& Player[nCnt]->side == CPlayer::WORLD_SIDE::FACE)
		{// �g�����|�������쓮���Ă��Ȃ�

			m_state = STATE::UP_LAND;
			m_nCnt = MAX_COUNT;
		}
	}
}