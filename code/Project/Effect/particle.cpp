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

#define PI				(314)		//円周
#define HARF_PI			(157)		//半円周
#define MAGNI			(100.0f)	//倍率
#define ATTEN_RATE		(0.3f)		//減衰率
#define FLOATUP_RATE	(0.05f)		//減衰率Y

//========================================
// 静的変数
//========================================
int CParticle::m_nNumAll = 0;
D3DXVECTOR3 CParticle::m_rot = INITD3DXVECTOR3;

//========================================
// コンストラクタ
//========================================
CParticle::CParticle(void)
{
	Manager::EffectMgr()->AddList(this);

	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.rot = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.col = COLOR_WHITE;
	m_move = INITVECTOR3D;

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
	if (m_type == TYPE::TYPE_NORMAL)
	{
		m_Info.move = D3DXVECTOR3(
			sinf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % m_Info.rdmMagni - m_Info.rdmMagni * 0.5f),	//xの移動量
			cosf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % m_Info.rdmMagni - m_Info.rdmMagni * 0.5f),	//yの移動量
			INITD3DXVECTOR3.z);
	}
	else if (m_type == TYPE::TYPE_SPIN)
	{
		m_Info.move = D3DXVECTOR3(
			m_move.x * sinf(m_Info.rot.z),
			m_move.y * cosf(m_Info.rot.z),
			0.0f);
	}
	else if (m_type == TYPE::TYPE_FLOATUP)
	{
		float rate = 0.01f + RNLib::Number().GetRandomFloat(0.01f);

		m_Info.move = D3DXVECTOR3(
			m_move.x * sinf(m_Info.rot.z) * rate,
			m_move.y * cosf(m_Info.rot.z) * rate,
			0.0f);

		m_Info.rot.x = 0.0f;
		m_Info.rot.y = 0.0f;
	}
	else if (m_type == TYPE::TYPE_STOP)
	{
		m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	m_Info.col = COLOR_WHITE;
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

	//割合計算
	float fCountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_Info.nCount, m_Info.nCountMax);

	D3DXVECTOR3 fScaleRate = m_Info.scale * fCountRate;

	m_Info.scale = fScaleRate;

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x, m_Info.scale.y)
		->SetZTest(true)
		->SetAlphaBlendMode(m_Info.alphamode);

	//移動量減衰
	if (m_type != TYPE::TYPE_FLOATUP){
		m_Info.move.x += (0.0f - m_Info.move.x) * ATTEN_RATE;
		m_Info.move.y += (0.0f - m_Info.move.y) * ATTEN_RATE;
	}
	else{
		m_Info.move.x += (0.0f - m_Info.move.x) * FLOATUP_RATE;
		m_Info.move.y += (0.0f - m_Info.move.y) * FLOATUP_RATE;
	}

	//透明にしていく
	m_Info.col.a = m_Info.col.a * fCountRate;

	//寿命減らす
	m_Info.nCount--;

	if (m_Info.nCount <= 0)
		CObject::Delete();
}

//========================================
// 描画
//========================================
void CParticle::Draw(void)
{

}