//========================================
// 
// 松明の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "torch.h"
#include "../../main.h"
#include "../../Effect/eff-manager.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTorchクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CTorch::CTorch(void) {
	Manager::StageObjectMgr()->AddList(this);

	// 種類の設定

	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;

	// 各情報の初期化
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Torch.x");
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_fBlink = 0.0f;
	m_nBlinkAnim = 0;
}

//========================================
// デストラクタ
//========================================
CTorch::~CTorch(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CTorch::Init(void) {

}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CTorch::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CTorch::Update(void) {

	//モデル配置
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot, false)->SetOutLineIdx(true)
		->SetBrightnessOfEmissive(m_fBlink)->SetCol(m_color);

}