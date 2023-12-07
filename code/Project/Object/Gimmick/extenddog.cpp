//========================================
// 
// �L�т錢�̏���
// Author:KOMURO HIROMU
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
#include "../../Character/player.h"


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
	Manager::StageObjectMgr()->AddList(this);

	//�������
	m_type = TYPE::EXTEND_DOG;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLand = false;
	m_bShrink = false;
	m_bInversion = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_House.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHip.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHead.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[4] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_modelIdx[5] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Body.x");
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
	m_nHeight = 0;
	m_HeadPos = INITD3DXVECTOR3;
	m_HeadPosOid = INITD3DXVECTOR3;
	m_BodyPos = INITD3DXVECTOR3;
	m_HipPos = INITD3DXVECTOR3;
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
	m_HeadPos.y = m_nHeight * SIZE_OF_1_SQUARE;
	m_BodyPos = m_HeadPos;
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

	m_HeadPosOid = m_HeadPos;
	
	CObject *obj = NULL;

	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//�擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		TYPE type = stageObj->GetType();
		if (type == CStageObject::TYPE::BLOCK)
		{// �z�肳�ꂽ��ނ̎�

			D3DXVECTOR3 stagepos = stageObj->GetPos();
			D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());

			if (stagepos.x + (stagesize.x * 0.5f) >= m_HeadPos.x &&
				stagepos.x - (stagesize.x * 0.5f) <= m_HeadPos.x)
			{// �͈͓��ɂ���Ƃ�
				if (m_pos.y <= m_HeadPos.y && m_bInversion == false)
				{
					if (m_pos.y <= stagepos.y)
						m_pos = stagepos;
				}
				if (m_pos.y >= m_HeadPos.y && m_bInversion == true)
				{
					if (m_pos.y >= stagepos.y)
						m_pos = stagepos;
				}
			}
		}
	}

	if (m_state == STATE::NONE)
	{//�L�т錢���쓮���Ă���

		if (m_bShrink == false)
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
	else if (m_state == STATE::RETURN)
	{//�L�т錢���쓮���Ă��Ȃ�

		if (m_bShrink == false)
		{
			// �k�ރJ�E���g
				m_nCntShrink++;
			if (m_nCntShrink >= MAX_COUNT){
				m_nCntShrink = MAX_COUNT;
				m_state = STATE::NONE;
			}
		}
		else
		{
			// �k�ރJ�E���g
			m_nCntShrink--;
			if (m_nCntShrink <= 0) {
				m_nCntShrink = 0;
				m_state = STATE::NONE;

			}
		}
	}

	// �����v�Z 
	float fCountRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);

	if (m_bInversion == false)
	{
		//y���W�̍X�V
		float fDowncurrenty = (SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		if (fDowncurrenty < 0.0f)
		{
			m_rot.z = 3.16f;
		}

		// �K
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[3], m_HipPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetOutLineIdx(true);

		// ��
		m_HeadPos.y = fDowncurrenty + m_pos.y;
		
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[4], m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetOutLineIdx(true);

		// ��
		m_BodyPos.y = (m_HeadPos.y + m_pos.y - SIZE_OF_1_SQUARE * 0.5f) / 2;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[5], m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight + (SIZE_OF_1_SQUARE * m_nHeight * (1.0f - fCountRate))) * 7, 1.0f),  false)
			->SetOutLineIdx(true); 
	}
	else
	{// ���]��Ԃ̎�
		// y���W�̍X�V
		float fDowncurrenty = -(SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		if (fDowncurrenty > 0.0f)
		{
			m_rot.z = 3.16f;
		}

		// �K
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[3], m_HipPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), false)
			->SetOutLineIdx(true);

		// ��
		m_HeadPos.y = fDowncurrenty + m_pos.y;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[4], m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),  false)
			->SetOutLineIdx(true);

		// ��
		//m_BodyPos.y = (m_HeadPos.y + m_pos.y - SIZE_OF_1_SQUARE * 0.5f) * 0.5f;
		m_BodyPos.y = (m_HeadPos.y + m_pos.y + SIZE_OF_1_SQUARE * 0.5f) * 0.5f;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[5], m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight - (SIZE_OF_1_SQUARE * (m_nHeight - 1.5) * fCountRate)) * 11, 1.0f),  false)
			->SetOutLineIdx(true);
	}
}
//========================================
// �`�揈��
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Draw(void) {

}