//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
#include "block.h"
#include "../../main.h"

//========================================
// �萔��`
//========================================
const char* CBlock::MODEL_PATHS[(int)LOOKS_TYPE::MAX] = {
	"data\\MODEL\\Object\\SoilBlock.x",
	"data\\MODEL\\Object\\SoilBlock_Grassy.x",
	"data\\MODEL\\Object\\AncientStoneBlock.x",
	"data\\MODEL\\Object\\AncientStoneBlock_PatternA.x",
	"data\\MODEL\\Object\\AncientStoneBlock_PatternB.x",
	"data\\MODEL\\Object\\AncientStoneBlock_PatternC.x",
	"data\\MODEL\\Object\\AncientStonePillar_Up.x",
	"data\\MODEL\\Object\\AncientStonePillar_Center.x",
	"data\\MODEL\\Object\\AncientStonePillar_Down.x",
};

//========================================
// �R���X�g���N�^
//========================================
CBlock::CBlock(void) {

	Manager::StageObjectMgr()->AddList(this);
	m_type   = TYPE::BLOCK;
	m_width  = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;
}

//========================================
// �f�X�g���N�^
//========================================
CBlock::~CBlock() {

}

//========================================
// ������
//========================================
HRESULT CBlock::Init(LOOKS_TYPE looksType) {
	m_Info.looksType = looksType;

	// ���f���ǂݍ���
	m_Info.modelIdx = RNLib::Model().Load(MODEL_PATHS[(int)looksType]);

	return S_OK;
}

//========================================
// �I��
//========================================
void CBlock::Uninit(void) {
	
}

//========================================
// �X�V
//========================================
void CBlock::Update(void) {

	// �ߋ��̈ʒu
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.modelIdx, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f),  false)
		->SetOutLine(true)
		->SetCol(m_color);
}

//========================================
// �`��
//========================================
void CBlock::Draw(void) {

}