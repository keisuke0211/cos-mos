//========================================
// 
// エフェクト処理
// Author:RYUKI FUJIWARA
// 
//========================================
// *** block.cpp ***
//========================================
#include "effect.h"
#include "../main.h"

#define MAX_COUNT (60)	//カウント

//========================================
// 静的変数
//========================================
int CEffect::m_nNumAll = 0;

//========================================
// コンストラクタ
//========================================
CEffect::CEffect(void)
{
	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.col = INITCOLOR;
	m_Info.nCount = MAX_COUNT;
	m_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CEffect::~CEffect()
{
	m_nNumAll--;
}

//========================================
// 初期化
//========================================
HRESULT CEffect::Init(int nTex)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITCOLOR;
	m_Info.nTex = nTex;
	m_Info.nCount = MAX_COUNT;

	return S_OK;
}

//========================================
// 終了
//========================================
void CEffect::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CEffect::Update(void)
{
	// 過去の位置
	RNLib::Polygon3D().Put(m_Info.pos + m_Info.move, INITD3DXVECTOR3)
		->SetTex(m_Info.nTex)
		->SetBillboard(true)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x,m_Info.scale.y)
		->SetAlphaBlendMode(CDrawState::ALPHA_BLEND_MODE::ADD)
		->SetPriority(1);

	m_Info.nCount--;

	//割合計算
	float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_Info.nCount, MAX_COUNT);

	m_Info.col.a = m_Info.col.a * fCountRate;

	if (m_Info.nCount <= 0)
	{
		CObject::Delete();
	}
}

//========================================
// 描画
//========================================
void CEffect::Draw(void)
{

}