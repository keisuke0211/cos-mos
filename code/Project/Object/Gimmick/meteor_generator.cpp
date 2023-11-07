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
//�}�N����`
//=======================================
#define SUMMON_INTERVAL		(120)		// �o���̊Ԋu

//=======================================
//�ÓI�����o�ϐ�
//=======================================
CMeteor *CMeteorGenerator::pMeteor = NULL;	

//=======================================
//�R���X�g���N�^
//=======================================
CMeteorGenerator::CMeteorGenerator()
{
	Manager::BlockMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_nCntSummon = 0;
	m_nSummoninterval = 0;
}

//=======================================
//�f�X�g���N�^
//=======================================
CMeteorGenerator::~CMeteorGenerator()
{
	pMeteor = NULL;
}

//=======================================
//����������
//=======================================
void CMeteorGenerator::Init(void)
{
	// 覐΂̐���
	pMeteor = Manager::BlockMgr()->MeteorCreate(m_pos, m_move);
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
	if (pMeteor == NULL)
	{// 覐΂��g�p����Ă��Ȃ��Ƃ�
		m_nCntSummon++;	// ����

		if (m_nCntSummon >= m_nSummoninterval)
		{// �����̊Ԋu�𒴂����Ƃ�

			// 覐΂̐���
			pMeteor = Manager::BlockMgr()->MeteorCreate(m_pos, m_move);
		}
	}
}

