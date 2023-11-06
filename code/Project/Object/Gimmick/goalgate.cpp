//========================================
// 
// �g�����|�����̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#include "goalgate.h"
#include "../../main.h"
#include "../../../_RNLib/Basis/input.h"
#include "../../Character/player.h"

//�}�N����`
#define MAX_COUNT		(60)	//�ő�J�E���g��

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CGoalGate�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CGoalGate::CGoalGate(void) {
	Manager::BlockMgr()->AddList(this);

	//�������
	m_scale = Scale3D(3.0f,3.0f,6.0f);
	m_nCnt = MAX_COUNT;
	m_state = STATE::SCALE_DOWN;
	m_type = TYPE::GOALGATE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 2.0f;
	m_bGoal = false;
	m_modelIdx = RNLib::Model().Load("data\\MODEL\\1P.x");
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");
}

//========================================
// �f�X�g���N�^
//========================================
CGoalGate::~CGoalGate(void) {

}

//========================================
// ����������
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Init(void) {

}

//========================================
// �I������
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Uninit(void) {

}

//========================================
// �X�V����
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Update(void) {

	m_rot.z += 0.05f;

	if (m_state == STATE::SCALE_DOWN)
	{
		m_nCnt--;

		if (m_nCnt < MAX_COUNT * 0.8)
		{
			m_state = STATE::SCALE_UP;
		}
	}
	if (m_state == STATE::SCALE_UP)
	{
		m_nCnt++;

		if (m_nCnt > MAX_COUNT)
		{
			m_state = STATE::SCALE_DOWN;
		}
	}

	//�����v�Z
	float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);

	RNLib::Model().Put(m_pos, m_rot,Scale3D(m_scale.x * fCountRate, m_scale.y * fCountRate,m_scale.z), m_modelIdx, false)
		->SetOutLine(true);
}
//========================================
// �`�揈��
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Draw(void) 
{

}