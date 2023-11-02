//========================================
// 
// 往復するレーザーの処理
// Author:KOMURO HIROMU
// 
//========================================
#include "RoadTripLaser.h"
#include "../../main.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CRoadTripLaserクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
static const D3DXVECTOR2  s_Size = D3DXVECTOR2(5.0f,50.0f);	// 高さ

//========================================
// コンストラクタ
//========================================
CRoadTripLaser::CRoadTripLaser(void) {
	Manager::BlockMgr()->AddList(this);

	m_type = TYPE::BACKGROUND;	// 種類の設定

	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 5;
	m_height = SIZE_OF_1_SQUARE * 5;

	// 各情報の初期化
	m_pos = INITD3DXVECTOR3;
	m_refPos = INITD3DXVECTOR3;
	m_frefdef = 0.0f;
	m_fGroundDis = 0.0f;
}

//========================================
// デストラクタ
//========================================
CRoadTripLaser::~CRoadTripLaser(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Init(void) {
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Lift.x");
	//RNLib::Texture().Load();

	m_refPos = m_pos;
	m_fGroundDis = m_pos.y - 0.0f;
	m_fGroundDis = fabsf(m_fGroundDis);

}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Update(void) {

	D3DXVECTOR3 Block = m_pos;	// 位置

	//オブジェクトを取得
	CObject *obj = NULL;
	float fDis = m_fGroundDis;
	while (Manager::BlockMgr()->ListLoop(&obj)) {
		//取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		TYPE type = stageObj->GetType();
		if (type == CStageObject::TYPE::MOVE_BLOCK || type == CStageObject::TYPE::BLOCK || type == CStageObject::TYPE::TRAMPOLINE)
		{// 想定された種類の時

			D3DXVECTOR3 stagepos = stageObj->GetPos();
			D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());

			if (stagepos.x + (stagesize.x * 0.5f) >= m_pos.x - s_Size.x &&
				stagepos.x - (stagesize.x * 0.5f) <= m_pos.x + s_Size.x)
			{// 範囲内にいるとき
				if (fDis >= m_pos.y - stagepos.y - stagesize.y * 0.5f)
				{// 距離が近いとき
					fDis = m_pos.y - stagepos.y - stagesize.y * 0.5f;	// 距離の更新
				}
			}
		}
	}
	
	// xの移動量の反転
	if (m_refPos.x + m_frefdef <= m_pos.x || m_refPos.x - m_frefdef >= m_pos.x)
	{
		m_move.x *= -1;
	}
	// yの移動量の反転
	if (m_refPos.y + m_frefdef <= m_pos.y || m_refPos.y - m_frefdef >= m_pos.y)
	{
		m_move.y *= -1;
	}

	// ブロックの位置設定
	m_pos += m_move;

	// ブロック
	RNLib::Model().Put(Block, m_rot, ModelIdx, false);

	// ビーム
	RNLib::Polygon3D().Put(D3DXVECTOR3(m_pos.x, (Block.y - fDis * 0.5f),m_pos.z), m_rot, false)
		->SetSize(s_Size.x, fDis);
}

//========================================
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Draw(void) {


}