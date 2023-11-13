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
	m_width = SIZE_OF_1_SQUARE * 2.0f;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\Spring_Body.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\Spring_Footing.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\Spring_Spring.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\Spring_Eye.x");
	m_fJamp = 8.0f;
	m_nCnt = 1;
	m_pSpringPos = NULL;
	m_fSpringForce = 0.0f;
}

//========================================
// �f�X�g���N�^
//========================================
CTrampoline::~CTrampoline(void) {

	//�΂ˈʒu�i�[�ϐ��J��
	if (m_pSpringPos != NULL) {
		delete[] m_pSpringPos;
		m_pSpringPos = NULL;
	}
}

//========================================
// ����������
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Init(void) {
	//�΂˂̈ʒu��ۑ�����ϐ����m��
	if (m_pSpringPos == NULL)
		m_pSpringPos = new D3DXVECTOR3[(int)STATE::MAX];

	for (int nCnt = 0; nCnt < (int)STATE::MAX; nCnt++)
	{
		m_pSpringPos[nCnt] = INITD3DXVECTOR3;
	}

	m_pSpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
	m_pSpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
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

	if (m_nCnt > 0)
	{
		m_nCnt--;
		if (m_nCnt == 0) {
			m_state = STATE::NONE;
			m_pSpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
			m_pSpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
		}
	}

	//�����v�Z
	const float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);

	if (m_state == STATE::UP_LAND)
	{
		m_pSpringPos[1].y = m_pos.y - CORRECT_HEIGHT - m_fJamp * fCountRate;
	}
	else if (m_state == STATE::DOWN_LAND)
	{
		m_pSpringPos[0].y = m_pos.y + CORRECT_HEIGHT + m_fJamp * fCountRate;
	}

	//���f���z�u
	PutModel();
}

//========================================
// ���f���z�u����
// Author:HIRASAWA SHION
//========================================
void CTrampoline::PutModel(void)
{
	//�y�䃂�f��
	RNLib::Model().Put(m_pos, INITD3DXVECTOR3, m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);

	//�L�m�R
	RNLib::Model().Put(m_pSpringPos[0], INITD3DXVECTOR3, m_modelIdx[1], false)
		->SetOutLine(true);
	RNLib::Model().Put(m_pSpringPos[1], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[1], false)
		->SetOutLine(true);

	//�΂�
	RNLib::Model().Put(m_pSpringPos[0], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[2], false)
		->SetOutLine(true);
	//�΂�
	RNLib::Model().Put(m_pSpringPos[1], INITD3DXVECTOR3, m_modelIdx[2], false)
		->SetOutLine(true);

	//�ڋʃ��f��
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x + CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), INITD3DXVECTOR3, m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x - CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), INITD3DXVECTOR3, m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);
}

//========================================
// �΂˂̈ʒu�擾
// Author:HIRASAWA SHION
//========================================
D3DXVECTOR3 CTrampoline::GetSpringPos(int nIdx)
{
	if (m_pSpringPos == NULL || nIdx >= (int)STATE::MAX) return INITD3DXVECTOR3;
	return m_pSpringPos[nIdx];
}