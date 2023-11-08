//========================================
// 
// 遷移の処理
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ transition.cpp ]]]
//========================================
#include "../../RNLib.h"

//****************************************
// マクロ定義
//****************************************
#define TIME           (30)
#define WAIT_TIME      (10)
#define HOLE_SCALE_MAX (6.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTransitionクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CTransition::CTransition() {
	m_state = STATE::NONE;
	m_nStateCtr = 0;
	m_type = TYPE::NONE;
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CTransition::~CTransition() {

}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CTransition::Update(void) {

	//===== [[[ ローカル関数宣言 ]]]
	struct LocalFunc {
		static void FillScreen(const float& fRate) {
			RNLib::Polygon2D().Put(RNLib::Window().GetCenterPos(), 0.0f)
				->SetCol(Color{ 0,0,0,(int)(255 * fRate) })
				->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());
		}
	};

	if (m_state == STATE::NONE) {
		return;
	}
	else if (m_state == STATE::WAIT_SET_MODE) {
		//LocalFunc::FillScreen(1.0f);
		return;
	}
	else if (m_state == STATE::OPEN_WAIT) {
		if (++m_nStateCtr >= WAIT_TIME) {
			m_nStateCtr = 0;
			m_state = STATE::OPEN;
		}
		LocalFunc::FillScreen(1.0f);
		return;
	}
	else if (++m_nStateCtr >= TIME) {
		if (m_state == STATE::CLOSE) {
			m_state = STATE::WAIT_SET_MODE;
		}
		else {
			m_state = STATE::NONE;
		}
		return;
	}

	float fRate = (float)m_nStateCtr / TIME;
	if (m_state == STATE::CLOSE) {
		fRate = 1.0f - fRate;
	}
	float fRateOpp = 1.0f - fRate;

	LocalFunc::FillScreen(fRateOpp);
}

//========================================
// 設定処理
// Author:RIKU NISHIMURA
//========================================
void CTransition::Set(STATE state, TYPE type) {
	if (state == STATE::OPEN)
		m_state = STATE::OPEN_WAIT;
	else
		m_state = state;
	m_type = type;
	m_nStateCtr = 0;
}