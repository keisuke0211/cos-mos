//========================================
// 
// �g�����|�����̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#include "goalgate.h"
#include "../../main.h"
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
	Manager::StageObjectMgr()->AddList(this);

	//�������
	m_scale = Scale3D(3.0f,3.0f,6.0f);
	m_nCnt = MAX_COUNT;
	m_state = STATE::NONE;
	m_type = TYPE::GOALGATE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 2.0f;
	m_modelIdx = RNLib::Model().Load("data\\MODEL\\GoalGate.x");
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

	m_state = STATE::SMALL;
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

	if (RNLib::Input().GetKeyTrigger(DIK_U))
	{
		m_state = STATE::MAX;
	}

	if (m_state == STATE::SMALL)
	{
		m_nCnt--;

		if (m_nCnt < MAX_COUNT * 0.8)
		{
			m_state = STATE::GROW;
		}
	}
	else if (m_state == STATE::GROW)
	{
		m_nCnt++;

		if (m_nCnt > MAX_COUNT)
		{
			m_state = STATE::SMALL;
		}
	}
	else if (m_state == STATE::MAX)
	{
		if (m_nCnt > 0)
		{
			m_nCnt--;
		}
		else
		{
			m_state = STATE::NONE;

			for (int ParCnt = 0; ParCnt < 16; ParCnt++)
			{
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[0], m_pos, INIT_EFFECT_SCALE, INITCOLOR);
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[1], m_pos, INIT_EFFECT_SCALE, INITCOLOR);
			}	
		}
	}

	//�����v�Z
	float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);

	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx, m_pos, m_rot, Scale3D(m_scale.x * fCountRate, m_scale.y * fCountRate, m_scale.z), false);
}
//========================================
// �`�揈��
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Draw(void) 
{

}