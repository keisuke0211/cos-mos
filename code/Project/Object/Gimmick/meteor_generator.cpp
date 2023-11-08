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
//マクロ定義
//=======================================
#define SUMMON_INTERVAL		(120)		// 出現の間隔

//=======================================
//静的メンバ変数
//=======================================
CMeteor *CMeteorGenerator::pMeteor = NULL;	

//=======================================
//コンストラクタ
//=======================================
CMeteorGenerator::CMeteorGenerator()
{
	Manager::BlockMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_nCntSummon = 0;
	m_nSummoninterval = 0;
}

//=======================================
//デストラクタ
//=======================================
CMeteorGenerator::~CMeteorGenerator()
{
	pMeteor = NULL;
}

//=======================================
//初期化処理
//=======================================
void CMeteorGenerator::Init(void)
{
	// 隕石の生成
	pMeteor = Manager::BlockMgr()->MeteorCreate(m_pos, m_move);
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
	if (pMeteor == NULL)
	{// 隕石が使用されていないとき
		m_nCntSummon++;	// 増加

		if (m_nCntSummon >= m_nSummoninterval)
		{// 生成の間隔を超えたとき

			// 隕石の生成
			pMeteor = Manager::BlockMgr()->MeteorCreate(m_pos, m_move);
		}
	}
}

