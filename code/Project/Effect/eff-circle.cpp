//========================================
// 
// 円形エフェクト処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "eff-circle.h"
#include "../manager.h"
#include "../resource.h"

//========================================
// コンストラクタ
//========================================
CEffCircle::CEffCircle(const int& life, const Pos3D& pos, const Color& col, const float& startScale, const float& endScale) {

	// リストに追加
	Manager::EffectMgr()->AddList(this);

	m_life       = 
	m_maxLife    = life;
	m_pos        = pos;
	m_col        = col;
	m_startScale = startScale;
	m_endScale   = endScale;
}

//========================================
// デストラクタ
//========================================
CEffCircle::~CEffCircle() {

	// リストから削除
	Manager::EffectMgr()->SubList(this);
}

//========================================
// 更新処理
//========================================
void CEffCircle::Update(void) {

	// 寿命割合を算出
	float lifeRate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_life, m_maxLife);
	float scale    = (lifeRate * m_startScale) + ((1.0f - lifeRate) * m_endScale);

	// 表示
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(Size2D(scale, scale))
		->SetCol(m_col.GetAlphaApplied(lifeRate))
		->SetAlphaBlendMode(_RNC_DrawState::ALPHA_BLEND_MODE::ADD)
		->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::EFF_CIRCLE])
		->SetZTest(false);

	// 寿命が尽きた時、自身を破棄
	if (--m_life == 0)
		Delete();
}