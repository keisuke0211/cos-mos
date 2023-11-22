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
	"data\\MODEL\\StageObject\\SoilBlock.x",
	"data\\MODEL\\StageObject\\SoilBlock_Grassy.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternA.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternB.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternC.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Up.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Center.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Down.x",
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf.x",
	"data\\MODEL\\StageObject\\TreeTrunk.x",
	"data\\MODEL\\StageObject\\TreeTrunk_Root.x",
	"NONEDATA",
};
const char* CBlock::OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX] = {
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf_Inside.x",
};
const char* CBlock::OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX] = {
	"data\\SETUP\\BaobabTree.txt",
};

//========================================
// �ϐ���`
//========================================
short CBlock::m_modelIdxes[(int)LOOKS_TYPE::MAX];
short CBlock::m_otherModelIdxes[(int)OTHER_MODEL::MAX];
short CBlock::m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];

//========================================
// �ǂݍ��ݏ���
//========================================
void CBlock::Load(void) {

	// ���f���̓ǂݍ���
	for (int cnt = 0; cnt < (int)LOOKS_TYPE::MAX; cnt++) {
		m_modelIdxes[cnt] = RNLib::Model().Load(MODEL_PATHS[cnt]);
	}
	for (int cnt = 0; cnt < (int)OTHER_MODEL::MAX; cnt++) {
		m_otherModelIdxes[cnt] = RNLib::Model().Load(OTHER_MODEL_PATHS[cnt]);
	}

	// �Z�b�g�A�b�v3D�ǂݍ���
	for (int cnt = 0; cnt < (int)OTHER_SETUP3D::MAX; cnt++) {
		m_otherSetUp3DlIdxes[cnt] = RNLib::SetUp3D().Load(OTHER_SETUP3D_PATHS[cnt]);
	}
}

//========================================
// �R���X�g���N�^
//========================================
CBlock::CBlock(void) {

	Manager::StageObjectMgr()->AddList(this);
	m_isCollision  = true;
	m_doll         = NULL;
	m_type         = TYPE::BLOCK;
	m_width        = SIZE_OF_1_SQUARE;
	m_height       = SIZE_OF_1_SQUARE;
	m_targetAddPos = INITPOS3D;
	m_oldAddPos    = INITPOS3D;
	m_addPos       = INITPOS3D;
	m_counter      = 0;
	m_counterMax   = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CBlock::~CBlock() {

	CMemory::Release(&m_doll);
}

//========================================
// ������
//========================================
HRESULT CBlock::Init(LOOKS_TYPE looksType) {
	m_looksType = looksType;

	// �h�[���̐���
	switch (m_looksType) {
	case LOOKS_TYPE::BAOBAB_TREE: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::BAOBAB_TREE]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, -8.0f - (((int)fabsf(m_pos.x) % 20) * (m_pos.y / fabsf(m_pos.y))), 30.0f + ((int)fabsf(m_pos.x) % 20)));
	}break;
	}

	// �ʒu����
	switch (m_looksType) {
	case LOOKS_TYPE::SOIL_BLOCK					:
	case LOOKS_TYPE::SOIL_BLOCK_GRASSY			:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK		:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_A	:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_B	:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_C	:
	case LOOKS_TYPE::LEAF_BLOCK					:
		m_pos.z -= ((int)fabsf(m_pos.x + m_pos.y) % 20) * 0.5f;
		break;
	}

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

	switch (m_looksType) {
	case LOOKS_TYPE::LEAF_BLOCK: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLine(m_isCollision);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLine(m_isCollision);
	}break;
	case LOOKS_TYPE::BAOBAB_TREE: {

	}break;
	default: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLine(m_isCollision);
	}break;
	}
}

//========================================
// �`��
//========================================
void CBlock::Draw(void) {

}