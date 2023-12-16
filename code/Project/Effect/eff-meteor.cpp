//========================================
// 
// 隕石エフェクト処理
// Author:KOMURO HIROMU
// 
//========================================
// *** effect-manager.cpp ***
//========================================
#include "eff-meteor.h"
#include "../main.h"
//========================================
// マクロ定義
//========================================
#define MAX_DELTECNT	(20)		// 破片が消えるまでの時間
#define MAX_RANDMOVE	(30)		// ランド関数で出る最大の移動量の値
#define MAX_SCALE		(Scale3D(2.0f,2.0f,2.0f))	// 最大の大きさ
//========================================
// 静的変数
//========================================
int CEffect_Meteor::m_nNumAll = 0;
int CEffect_Meteor::s_nModelIdx[3] = {};
//========================================
// コンストラクタ
//========================================
CEffect_Meteor::CEffect_Meteor(void)
{
	//Manager::BlockMgr()->AddList(this);
	Manager::EffectMgr()->AddList(this);

	for (int nCnt = 0; nCnt < MAX_INFO; nCnt++)
	{
		m_Info[nCnt].pos = INITD3DXVECTOR3;
		m_Info[nCnt].rot = INITD3DXVECTOR3;
		m_Info[nCnt].move = INITD3DXVECTOR3;
		m_Info[nCnt].scale = Scale3D(1.0f,1.0f,1.0f);
		m_Info[nCnt].col = INITCOLOR;
	}
	m_pos = INITD3DXVECTOR3;
	m_nCount = MAX_DELTECNT;
	m_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CEffect_Meteor::~CEffect_Meteor()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// 初期化
//========================================
HRESULT CEffect_Meteor::Init(void)
{
	s_nModelIdx[0] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_0.x");
	s_nModelIdx[1] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_1.x");
	s_nModelIdx[2] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_2.x");


	for (int nCnt = 0; nCnt < MAX_INFO; nCnt++)
	{
		m_Info[nCnt].pos.x = m_pos.x + (rand() % MAX_RANDMOVE - MAX_RANDMOVE + 10);
		m_Info[nCnt].pos.y = m_pos.y + (rand() % MAX_RANDMOVE - MAX_RANDMOVE + 10);
		m_Info[nCnt].pos.z = m_pos.z + (rand() % MAX_RANDMOVE - MAX_RANDMOVE + 10);
		m_Info[nCnt].move.x = (rand() % 4 - 2) * 0.5f;
		m_Info[nCnt].move.y = (rand() % 4 - 2) * 0.5f;
		m_Info[nCnt].move.z = (rand() % 4 - 2) * 0.5f;
		m_Info[nCnt].col = INITCOLOR;

	}
	return S_OK;
}

//========================================
// 終了
//========================================
void CEffect_Meteor::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CEffect_Meteor::Update(void)
{
	m_nCount--;

	//割合計算
	float fCountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_nCount, MAX_DELTECNT);

	for (int nCnt = 0; nCnt < MAX_INFO; nCnt++)
	{
		m_Info[nCnt].scale = MAX_SCALE * fCountRate;	// スケールの倍率

		//モデル配置
		RNLib::Model().Put(PRIORITY_OBJECT, s_nModelIdx[0], m_Info[nCnt].pos, m_Info[nCnt].rot,m_Info[nCnt].scale, false)
			->SetOutLineIdx(true);

		// 位置の増加
		m_Info[nCnt].pos += m_Info[nCnt].move;
	}

	if (m_nCount <= 0)
	{
		CObject::Delete();
	}
}

//========================================
// 描画
//========================================
void CEffect_Meteor::Draw(void)
{

}