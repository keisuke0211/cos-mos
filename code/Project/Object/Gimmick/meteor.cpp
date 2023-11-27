//========================================
// 
// 隕石の処理
// Author:KOMURO HIROMU  Hirasawa Shion
// 
//========================================
#include "meteor.h"
#include "../../main.h"
#include "../../Effect/eff-manager.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampolineクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
const D3DXVECTOR3 CMeteor::METEOR_ADDROT = { 0.10f,0.02f,0.01f };    // 隕石の回転の移動量
const D3DXVECTOR3 CMeteor::METEOR_MINPOS = { -150.0f,-150.0f,0.0f }; // 最小の位置
const D3DXVECTOR3 CMeteor::METEOR_MAXPOS = { 150.0f,150.0f,0.0f };   // 最大の位置
const int		  CMeteor::METEOR_ANIME_MAG = 2;                     // 点滅アニメーションの倍率
const int		  CMeteor::METEOR_BLINK_MAX = 10 * METEOR_ANIME_MAG; // 点滅アニメーションの最大数
const float		  CMeteor::METEOR_BLINK_MIN = 0.0f;                  // 点滅の最小数
const float		  CMeteor::METEOR_BLINK_ADJ = 0.01f;                 // 点滅アニメーションの調整

//========================================
// コンストラクタ
//========================================
CMeteor::CMeteor(void) {
	Manager::StageObjectMgr()->AddList(this);
	
	// 種類の設定
	m_type = TYPE::METEOR;

	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;

	// 各情報の初期化
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Meteorite.x");
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_fBlink = 0.0f;
	m_nBlinkAnim = 0;
}

//========================================
// デストラクタ
//========================================
CMeteor::~CMeteor(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CMeteor::Init(void) {

}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CMeteor::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CMeteor::Update(void) {

	m_posOld = m_pos;		// 過去位置の更新
	
	//点滅アニメーション処理
	BlinkAnimation();

	m_rot += METEOR_ADDROT;	// 向きの移動量の追加
	m_pos += m_move;		// 移動量の追加

	// ブロックとの当たり判定処理
	CollisionBlock();

	//モデル配置
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot,  false)->SetOutLineIdx(true)
		->SetBrightnessOfEmissive(m_fBlink)->SetCol(m_color);

}

//========================================
// 点滅アニメーション処理
// Author:KOMURO HIROMU  Hirasawa Shion
//========================================
void CMeteor::BlinkAnimation(void)
{
	// 点滅アニメーションの増加
	m_nBlinkAnim++;	

	// 点滅アニメーションの計算
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
// ブロックとの当たり判定処理
// Author:KOMURO HIROMU
//========================================
void CMeteor::CollisionBlock(void)
{
	//オブジェクトのポインタを格納
	CObject *obj = NULL;

	//オブジェクトを取得
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		//オブジェクトの当たり判定情報取得
		const D3DXVECTOR3 POS = stageObj->GetPos();
		const float WIDTH = stageObj->GetWidth() * 0.5f;
		const float HEIGHT = stageObj->GetHeight() * 0.5f;

		//オブジェクトの最小・最大位置
		const D3DXVECTOR2 MinPos = D3DXVECTOR3(POS.x - WIDTH, POS.y - HEIGHT, 0.0f);
		const D3DXVECTOR2 MaxPos = D3DXVECTOR3(POS.x + WIDTH, POS.y + HEIGHT, 0.0f);

		//種類取得
		const CStageObject::TYPE type = stageObj->GetType();

		//種類ごとに関数分け
		switch (type)
		{
		case CStageObject::TYPE::BLOCK:
			if (MaxPos.x > m_pos.x - m_width  * 0.5f&&
				MinPos.x < m_pos.x + m_width  * 0.5f&&
				MaxPos.y > m_pos.y - m_height * 0.5f&&
				MinPos.y < m_pos.y + m_height * 0.5f)
			{
				// エフェクトの生成
				Manager::EffectMgr()->EffectMeteorCreate(m_pos);
				// 削除
				Delete();
				return;
			}
			break;
		}
	}
}
