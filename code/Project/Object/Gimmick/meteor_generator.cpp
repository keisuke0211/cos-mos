//================================================================================================
//
//隕石生成器の処理[meteor_generator.cpp]
//Author:KOMURO HIROMU
//
//================================================================================================
#include "meteor_generator.h"
#include "../../manager.h"
#include "../stage-object.h"
#include "../Gimmick/meteor.h"

//=======================================
//静的メンバ変数
//=======================================

//=======================================
//コンストラクタ
//=======================================
CMeteorGenerator::CMeteorGenerator()
{
	Manager::StageObjectMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_nCntSummon = 0;
	m_nSummoninterval = 0;
	m_bLive = false;
}

//=======================================
//デストラクタ
//=======================================
CMeteorGenerator::~CMeteorGenerator()
{
}

//=======================================
//初期化処理
//=======================================
void CMeteorGenerator::Init(void)
{
	// 隕石の生成
	Manager::StageObjectMgr()->MeteorCreate(m_pos, m_move);
}

//=======================================
//終了処理
//=======================================
void CMeteorGenerator::Uninit(void)
{

}

//=======================================
//更新処理
//=======================================
void CMeteorGenerator::Update(void)
{
	m_bLive = false;

	//オブジェクトのポインタを格納
	CObject *obj = NULL;

	//オブジェクトを取得
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		//種類取得
		const CStageObject::TYPE type = stageObj->GetType();

		if (type == CStageObject::TYPE::METEOR)
		{
			m_bLive = true;	// 生きてる
		}
	}

	/*if (m_bLive == false)
	{*/// 隕石が生きていないとき
		m_nCntSummon++;	// 増加

		if (m_nCntSummon >= m_nSummoninterval)
		{// 生成の間隔を超えたとき

			// 隕石の生成
			Manager::StageObjectMgr()->MeteorCreate(m_pos, m_move);
			m_nCntSummon = 0;	// カウントの初期化
		}
	//}
}

