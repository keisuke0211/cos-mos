//========================================
// 
// エフェクト処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "eff.h"
#include "../main.h"

//========================================
// 静的変数
//========================================
int CEff::m_nNumAll = 0;

//========================================
// コンストラクタ
//========================================
CEff::CEff(void)
{
	Manager::EffectMgr()->AddList(this);

	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.scalemag = INITD3DXVECTOR3;
	m_Info.rot =INITD3DXVECTOR3;
	m_Info.col = COLOR_WHITE;
	m_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CEff::~CEff()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// 初期化
//========================================
HRESULT CEff::Init(int nTex, int nCount, D3DXVECTOR3 spin)
{
	m_Info.nTex = nTex;
	m_Info.nCount = m_Info.nCountMax = nCount;
	m_Info.spin = spin;

	return S_OK;
}

//========================================
// 終了
//========================================
void CEff::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CEff::Update(void)
{
	// 過去の位置
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetBillboard(m_Info.Billboard)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x, m_Info.scale.y)
		->SetAlphaBlendMode(_RNC_DrawState::ALPHA_BLEND_MODE::ADD)
		->SetZTest(false);

	if (m_Info.nCount != -44) {
		m_Info.nCount--;

		//割合計算
		float fCountRate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_Info.nCount, m_Info.nCountMax);

		m_Info.col.a = m_Info.col.a * fCountRate;

		m_Info.rot += m_Info.spin * fCountRate;

		m_Info.pos += m_Info.move;

		m_Info.scale.x *= m_Info.scalemag.x;
		m_Info.scale.y *= m_Info.scalemag.y;

		if (m_Info.nCount <= 0)
		{
			CObject::Delete();
		}
	}
}

//========================================
// 描画
//========================================
void CEff::Draw(void)
{
}