//========================================
// 
// 覐΂̏���
// Author:KOMURO HIROMU  Hirasawa Shion
// 
//========================================
#include "meteor.h"
#include "../../main.h"
#include "../../Effect/eff-manager.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampoline�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================
const D3DXVECTOR3 CMeteor::METEOR_ADDROT = { 0.10f,0.02f,0.01f };    // 覐΂̉�]�̈ړ���
const D3DXVECTOR3 CMeteor::METEOR_MINPOS = { -150.0f,-150.0f,0.0f }; // �ŏ��̈ʒu
const D3DXVECTOR3 CMeteor::METEOR_MAXPOS = { 150.0f,150.0f,0.0f };   // �ő�̈ʒu
const int		  CMeteor::METEOR_ANIME_MAG = 2;                     // �_�ŃA�j���[�V�����̔{��
const int		  CMeteor::METEOR_BLINK_MAX = 10 * METEOR_ANIME_MAG; // �_�ŃA�j���[�V�����̍ő吔
const float		  CMeteor::METEOR_BLINK_MIN = 0.0f;                  // �_�ł̍ŏ���
const float		  CMeteor::METEOR_BLINK_ADJ = 0.01f;                 // �_�ŃA�j���[�V�����̒���

//========================================
// �R���X�g���N�^
//========================================
CMeteor::CMeteor(void) {
	Manager::StageObjectMgr()->AddList(this);
	
	// ��ނ̐ݒ�
	m_type = TYPE::METEOR;

	// �傫���̐ݒ�
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;

	// �e���̏�����
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Meteorite.x");
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_fBlink = 0.0f;
	m_nBlinkAnim = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CMeteor::~CMeteor(void) {

}

//========================================
// ����������
// Author:KOMURO HIROMU
//========================================
void CMeteor::Init(void) {

}

//========================================
// �I������
// Author:KOMURO HIROMU
//========================================
void CMeteor::Uninit(void) {

}

//========================================
// �X�V����
// Author:KOMURO HIROMU
//========================================
void CMeteor::Update(void) {

	m_posOld = m_pos;		// �ߋ��ʒu�̍X�V
	
	//�_�ŃA�j���[�V��������
	BlinkAnimation();

	m_rot += METEOR_ADDROT;	// �����̈ړ��ʂ̒ǉ�
	m_pos += m_move;		// �ړ��ʂ̒ǉ�

	// �u���b�N�Ƃ̓����蔻�菈��
	CollisionBlock();

	//���f���z�u
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot,  false)->SetOutLineIdx(true)
		->SetBrightnessOfEmissive(m_fBlink)->SetCol(m_color);

}

//========================================
// �_�ŃA�j���[�V��������
// Author:KOMURO HIROMU  Hirasawa Shion
//========================================
void CMeteor::BlinkAnimation(void)
{
	// �_�ŃA�j���[�V�����̑���
	m_nBlinkAnim++;	

	// �_�ŃA�j���[�V�����̌v�Z
	if (m_nBlinkAnim < METEOR_BLINK_MAX * METEOR_ANIME_MAG)
	{
		m_fBlink -= METEOR_BLINK_MAX - m_nBlinkAnim;
		m_fBlink = m_fBlink / (10 * METEOR_ANIME_MAG);

		if (m_fBlink < 0.0f)
		{
			m_fBlink = fabsf(m_fBlink);
		}
	}
	else
	{
		m_nBlinkAnim = 0;
	}
}
//========================================
// �u���b�N�Ƃ̓����蔻�菈��
// Author:KOMURO HIROMU
//========================================
void CMeteor::CollisionBlock(void)
{
	//�I�u�W�F�N�g�̃|�C���^���i�[
	CObject *obj = NULL;

	//�I�u�W�F�N�g���擾
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//�擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		//�I�u�W�F�N�g�̓����蔻����擾
		const D3DXVECTOR3 POS = stageObj->GetPos();
		const float WIDTH = stageObj->GetWidth() * 0.5f;
		const float HEIGHT = stageObj->GetHeight() * 0.5f;

		//�I�u�W�F�N�g�̍ŏ��E�ő�ʒu
		const D3DXVECTOR2 MinPos = D3DXVECTOR3(POS.x - WIDTH, POS.y - HEIGHT, 0.0f);
		const D3DXVECTOR2 MaxPos = D3DXVECTOR3(POS.x + WIDTH, POS.y + HEIGHT, 0.0f);

		//��ގ擾
		const CStageObject::TYPE type = stageObj->GetType();

		//��ނ��ƂɊ֐�����
		switch (type)
		{
		case CStageObject::TYPE::BLOCK:
			if (MaxPos.x > m_pos.x - m_width  * 0.5f&&
				MinPos.x < m_pos.x + m_width  * 0.5f&&
				MaxPos.y > m_pos.y - m_height * 0.5f&&
				MinPos.y < m_pos.y + m_height * 0.5f)
			{
				// �G�t�F�N�g�̐���
				Manager::EffectMgr()->EffectMeteorCreate(m_pos);
				// �폜
				Delete();
				return;
			}
			break;
		}
	}
}
