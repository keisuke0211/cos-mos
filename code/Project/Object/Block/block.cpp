//========================================
// 
// ブロック処理
// Author:KEISUKE OTONO
// 
//========================================
#include "block.h"
#include "../../main.h"

//========================================
// 定数定義
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
};
const char* CBlock::OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX] = {
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf_Inside.x",
};

//========================================
// 変数定義
//========================================
short CBlock::m_modelIdxes[(int)LOOKS_TYPE::MAX];
short CBlock::m_otherModelIdxes[(int)OTHER_MODEL::MAX];

//========================================
// 読み込み処理
//========================================
void CBlock::Load(void) {

	// モデルの読み込み
	for (int cntLooks = 0; cntLooks < (int)LOOKS_TYPE::MAX; cntLooks++) {
		m_modelIdxes[cntLooks] = RNLib::Model().Load(MODEL_PATHS[cntLooks]);
	}
	for (int cntLooks = 0; cntLooks < (int)OTHER_MODEL::MAX; cntLooks++) {
		m_otherModelIdxes[cntLooks] = RNLib::Model().Load(OTHER_MODEL_PATHS[cntLooks]);
	}
}

//========================================
// コンストラクタ
//========================================
CBlock::CBlock(void) {

	Manager::StageObjectMgr()->AddList(this);
	m_isCollision  = true;
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
// デストラクタ
//========================================
CBlock::~CBlock() {

}

//========================================
// 初期化
//========================================
HRESULT CBlock::Init(LOOKS_TYPE looksType) {
	m_looksType = looksType;

	return S_OK;
}

//========================================
// 終了
//========================================
void CBlock::Uninit(void) {
	
}

//========================================
// 更新
//========================================
void CBlock::Update(void) {

	// 過去の当たりフラグ保存
	for (int cnt = 0; cnt < 6; cnt++){
		m_isHitOlds[cnt] = m_isHits[cnt];
	}

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
			->SetCol(m_color);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color);
	}break;
	default: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color);
	}break;
	}
}

//========================================
// 描画
//========================================
void CBlock::Draw(void) {

}