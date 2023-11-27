//========================================
// 
// �������郌�[�U�[�̏���
// Author:KOMURO HIROMU
// 
//========================================
#include "RoadTripLaser.h"
#include "../../main.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CRoadTripLaser�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CRoadTripLaser::CRoadTripLaser(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::LASER;	// ��ނ̐ݒ�

	// �傫���̐ݒ�
	m_width = SIZE_OF_1_SQUARE*2;
	m_height = SIZE_OF_1_SQUARE;

	// �e���̏�����
	m_pos = INITD3DXVECTOR3;
	m_posL = INITD3DXVECTOR3;
	m_posV = INITD3DXVECTOR3;
	m_fGroundDis = 0.0f;
	m_LaserSize = D3DXVECTOR2(5.0f, 50.0f);
	m_LaserPos = INITD3DXVECTOR3;
	m_bXMoveReturn = false;
}

//========================================
// �f�X�g���N�^
//========================================
CRoadTripLaser::~CRoadTripLaser(void) {

}

//========================================
// ����������
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Init(void) {
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Laser_0.x");

	m_pos = m_posV;	// �n�_�Ɉʒu��ݒ�
	m_LaserPos = m_pos;
	// �n�ʂ܂ł̋��������߂�
	m_fGroundDis = m_pos.y - 0.0f;
	m_fGroundDis = fabsf(m_fGroundDis);
}

//========================================
// �I������
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Uninit(void) {

}

//========================================
// �X�V����
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Update(void) {

	D3DXVECTOR3 Block = m_pos;	// �ʒu

	//�I�u�W�F�N�g���擾
	CObject *obj = NULL;
	float fDis = m_fGroundDis;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//�擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		TYPE type = stageObj->GetType();
		if (type == CStageObject::TYPE::MOVE_BLOCK || type == CStageObject::TYPE::BLOCK || type == CStageObject::TYPE::TRAMPOLINE)
		{// �z�肳�ꂽ��ނ̎�

			D3DXVECTOR3 stagepos = stageObj->GetPos();
			D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());

			if (stagepos.x + (stagesize.x * 0.5f) >= m_pos.x - m_LaserSize.x &&
				stagepos.x - (stagesize.x * 0.5f) <= m_pos.x + m_LaserSize.x)
			{// �͈͓��ɂ���Ƃ�
				if (fDis >= m_pos.y - stagepos.y - stagesize.y * 0.5f)
				{// �������߂��Ƃ�
					m_LaserSize.y = m_pos.y - stagepos.y - stagesize.y * 0.5f;	// �����̍X�V
				}
			}
		}
	}

	// -1�ȉ��̐��l�𔽓]������
	if (m_LaserSize.y <= -1)
	{
		m_LaserSize.y *= -1;
	}

	if (m_bXMoveReturn == false)
	{
		// x�̈ړ��ʂ̔��]
		if (m_posV.x > m_pos.x || m_posL.x < m_pos.x)
		{
			m_move.x *= -1;
		}	
	}
	else
	{
		// x�̈ړ��ʂ̔��]
		if (m_posV.x < m_pos.x || m_posL.x > m_pos.x)
		{
			m_move.x *= -1;
		}
	
	}
	// y�̈ړ��ʂ̔��]
	if (m_posV.y > m_pos.y || m_posL.y < m_pos.y)
	{
		m_move.y *= -1;
	}

	// �u���b�N�̈ʒu�ݒ�
	m_pos += m_move;

	// �u���b�N
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, Block, m_rot, false);

	m_LaserPos = Block;
	// �r�[��
	if (m_rot.z == 0.0f)
		m_LaserPos.y = (Block.y - m_LaserSize.y * 0.5f);
	else if (m_rot.z == D3DX_PI)
		m_LaserPos.y = (Block.y + m_LaserSize.y * 0.5f);

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_LaserPos, INITD3DXVECTOR3, false)
		->SetSize(m_LaserSize.x, m_LaserSize.y)
		->SetCol(Color{ 255,0,0,255 });
	
	Manager::EffectMgr()->ParticleCreate(RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png"), m_LaserPos, INIT_EFFECT_SCALE, INITCOLOR, CParticle::TYPE::TYPE_NORMAL,1);
}

//========================================
// �`�揈��
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Draw(void) {


}