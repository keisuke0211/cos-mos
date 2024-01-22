//================================================================================================
//
//覐ΐ�����̏���[meteor_generator.cpp]
//Author:KOMURO HIROMU
//
//================================================================================================
#include "meteor_generator.h"
#include "../../manager.h"
#include "../stage-object.h"
#include "../Gimmick/meteor.h"

//=======================================
//�ÓI�����o�ϐ�
//=======================================

//=======================================
//�R���X�g���N�^
//=======================================
CMeteorGenerator::CMeteorGenerator()
{
	Manager::StageObjectMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_nCntSummon = 0;
	m_nSummoninterval = 0;
	m_bLive = false;
}

//=======================================
//�f�X�g���N�^
//=======================================
CMeteorGenerator::~CMeteorGenerator()
{
}

//=======================================
//����������
//=======================================
void CMeteorGenerator::Init(void)
{
	// 覐΂̐���
	Manager::StageObjectMgr()->MeteorCreate(m_pos, m_move);
}

//=======================================
//�I������
//=======================================
void CMeteorGenerator::Uninit(void)
{

}

//=======================================
//�X�V����
//=======================================
void CMeteorGenerator::Update(void)
{
	m_bLive = false;

	//�I�u�W�F�N�g�̃|�C���^���i�[
	CObject *obj = NULL;

	//�I�u�W�F�N�g���擾
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//�擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		//��ގ擾
		const CStageObject::TYPE type = stageObj->GetType();

		if (type == CStageObject::TYPE::METEOR)
		{
			m_bLive = true;	// �����Ă�
		}
	}

	/*if (m_bLive == false)
	{*/// 覐΂������Ă��Ȃ��Ƃ�
		m_nCntSummon++;	// ����

		if (m_nCntSummon >= m_nSummoninterval)
		{// �����̊Ԋu�𒴂����Ƃ�

			// 覐΂̐���
			Manager::StageObjectMgr()->MeteorCreate(m_pos, m_move);
			m_nCntSummon = 0;	// �J�E���g�̏�����
		}
	//}
}

