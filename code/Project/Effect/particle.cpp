//========================================
// 
// エフェクト処理
// Author:RYUKI FUJIWARA
// 
//========================================
// *** block.cpp ***
//========================================
#include "particle.h"
#include "../main.h"

#define PI				(628)		//円周
#define HARF_PI			(314)		//半円周
#define MAGNI			(100.0f)	//倍率
#define RANDOM_MAGNI	(16)		//ランダム倍率
#define ATTEN_RATE		(0.3f)		//減衰率

//========================================
// 静的変数
//========================================
int CParticle::m_nNumAll = 0;

//========================================
// コンストラクタ
//========================================
CParticle::CParticle(void)
{
	Manager::EffectMgr()->AddList(this);

	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.col = INITCOLOR;
	m_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CParticle::~CParticle()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// 初期化
//========================================
HRESULT CParticle::Init(int nTex,int nCount)
{
	m_Info.move = D3DXVECTOR3(
	sinf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % RANDOM_MAGNI - RANDOM_MAGNI * 0.5f),	//xの移動量
	cosf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % RANDOM_MAGNI - RANDOM_MAGNI * 0.5f),	//yの移動量
	INITD3DXVECTOR3.z);

	m_Info.col = INITCOLOR;
	m_Info.nTex = nTex;
	m_Info.nCount = m_Info.nCountMax = nCount;

	return S_OK;
}

//========================================
// 終了
//========================================
void CParticle::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CParticle::Update(void)
{
	//移動量加算
	m_Info.pos += m_Info.move;

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, INITD3DXVECTOR3)
		->SetTex(m_Info.nTex)
		->SetBillboard(true)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x, m_Info.scale.y)
		->SetZTest(false)
		->SetAlphaBlendMode(CDrawState::ALPHA_BLEND_MODE::ADD);

	//移動量減衰
	m_Info.move.x += (0.0f - m_Info.move.x) * ATTEN_RATE;
	m_Info.move.y += (0.0f - m_Info.move.y) * ATTEN_RATE;

	//割合計算
	float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_Info.nCount, m_Info.nCountMax);

	//透明にしていく
	m_Info.col.a = m_Info.col.a * fCountRate;

	//寿命減らす
	m_Info.nCount--;

	if (m_Info.nCount <= 0)
	{
		CObject::Delete();
	}
}

//========================================
// 描画
//========================================
void CParticle::Draw(void)
{

}