//========================================
// 
// �����̏���
// Author:KOMURO HIROMU
// 
//========================================
#include "torch.h"
#include "../../main.h"
#include "../../Effect/eff-manager.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTorch�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CTorch::CTorch(void) {
	Manager::StageObjectMgr()->AddList(this);

	// ��ނ̐ݒ�

	// �傫���̐ݒ�
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;

	// �e���̏�����
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Torch.x");
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_fBlink = 0.0f;
	m_nBlinkAnim = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CTorch::~CTorch(void) {

}

//========================================
// ����������
// Author:KOMURO HIROMU
//========================================
void CTorch::Init(void) {

}

//========================================
// �I������
// Author:KOMURO HIROMU
//========================================
void CTorch::Uninit(void) {

}

//========================================
// �X�V����
// Author:KOMURO HIROMU
//========================================
void CTorch::Update(void) {

	//���f���z�u
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot, false)->SetOutLineIdx(true)
		->SetBrightnessOfEmissive(m_fBlink)->SetCol(m_color);

}