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

//========================================
// コンストラクタ
//========================================
CRoadTripLaser::CRoadTripLaser(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::LASER;	// 種類の設定

	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE*2;
	m_height = SIZE_OF_1_SQUARE;

	// 各情報の初期化
	m_pos = INITD3DXVECTOR3;
	m_posL = INITD3DXVECTOR3;
	m_posV = INITD3DXVECTOR3;
	m_fGroundDis = 0.0f;
	m_LaserSize = D3DXVECTOR2(5.0f, 50.0f);
	m_LaserPos = INITD3DXVECTOR3;
	m_bXMoveReturn = false;
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
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Laser_0.x");

	m_pos = m_posV;	// 始点に位置を設定
	m_LaserPos = m_pos;
	// 地面までの距離を求める
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
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		TYPE type = stageObj->GetType();
		if (type == CStageObject::TYPE::MOVE_BLOCK || type == CStageObject::TYPE::BLOCK || type == CStageObject::TYPE::TRAMPOLINE)
		{// 想定された種類の時

			D3DXVECTOR3 stagepos = stageObj->GetPos();
			D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());

			if (stagepos.x + (stagesize.x * 0.5f) >= m_pos.x - m_LaserSize.x &&
				stagepos.x - (stagesize.x * 0.5f) <= m_pos.x + m_LaserSize.x)
			{// 範囲内にいるとき
				if (fDis >= m_pos.y - stagepos.y - stagesize.y * 0.5f)
				{// 距離が近いとき
					m_LaserSize.y = m_pos.y - stagepos.y - stagesize.y * 0.5f;	// 距離の更新
				}
			}
		}
	}

	// -1以下の数値を反転させる
	if (m_LaserSize.y <= -1)
	{
		m_LaserSize.y *= -1;
	}

	if (m_bXMoveReturn == false)
	{
		// xの移動量の反転
		if (m_posV.x > m_pos.x || m_posL.x < m_pos.x)
		{
			m_move.x *= -1;
		}	
	}
	else
	{
		// xの移動量の反転
		if (m_posV.x < m_pos.x || m_posL.x > m_pos.x)
		{
			m_move.x *= -1;
		}
	
	}
	// yの移動量の反転
	if (m_posV.y > m_pos.y || m_posL.y < m_pos.y)
	{
		m_move.y *= -1;
	}

	// ブロックの位置設定
	m_pos += m_move;

	// ブロック
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, Block, m_rot, false);

	m_LaserPos = Block;
	// ビーム
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
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CRoadTripLaser::Draw(void) {


}