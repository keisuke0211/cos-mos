//========================================
// 
// 遷移の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//****************************************
// マクロ定義
//****************************************
#define BLACK_TIME     (10)
#define HOLE_SCALE_MAX (6.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]遷移クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CTransition::CTransition() {

	m_type = TYPE::NONE;
	m_state = STATE::NONE;
	m_fMove = 0.0f;
	m_stateCounter = 0;
	m_time = 0;
	m_col = INITCOLOR;
}

//========================================
// デストラクタ
//========================================
CTransition::~CTransition() {

}

//========================================
// 初期化処理
//========================================
void CTransition::Init(void) {

}

//========================================
// 終了処理
//========================================
void CTransition::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CTransition::Update(void) {

	float rate = 0.0f;	// 何もない 0 ~ 1 完全に埋まる

	switch (m_state) {
		// [[[ 無し ]]]
	case STATE::NONE: {

	}break;
		// [[[ オープン ]]]
	case STATE::OPEN: {
		rate = 1.0f - ((float)m_stateCounter / m_time);

		// 状態を無しに
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::NONE;
		}
	}break;
		// [[[ オープン待ち ]]]
	case STATE::OPEN_WAIT: {
		rate = 1.0f;
	}break;
		// [[[ クローズ ]]]
	case STATE::CLOSE: {
		rate = ((float)m_stateCounter / m_time);

		// 状態をオープン待ちに
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::OPEN_WAIT;
		}
	}break;
	}

	// 割合が0であれば終了
	if (rate == 0.0f)
		return;

	// 画面埋め処理
	FillScreen(rate);
}

//========================================
// オープン処理
//========================================
bool CTransition::Open(const TYPE& type, const UShort& time) {

	// 状態オープン待ちで無い時、
	if (m_state != STATE::OPEN_WAIT)
		return false;

	m_stateCounter = 0;
	m_state = STATE::OPEN;
	m_time = time;

	return true;
}

//========================================
// クローズ処理
//========================================
bool CTransition::Close(const TYPE& type, const Color& col, const UShort& time) {

	// 状態が無しで無い時、
	if (m_state != STATE::NONE)
		return false;

	m_stateCounter = 0;
	m_state = STATE::CLOSE;
	m_time = time;
	m_col = col;

	return true;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]遷移クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 画面埋め処理
//========================================
void CTransition::FillScreen(const float& rate) {

	// ポリゴン2Dの設置
	RNLib::Polygon2D().Put(1, RNLib::Window().GetCenterPos(), 0.0f, false)
		->SetZTest(false)
		->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	//// ポリゴン2Dの設置
	//RNLib::Polygon2D().Put(1, D3DXVECTOR3(0.0f + m_fMove, RNLib::Window().GetHeight() / 3, 0.0f), 0.0f, true)
	//	->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
	//	->SetSize(m_fMove, RNLib::Window().GetHeight() / 2);

	//// ポリゴン2Dの設置
	//RNLib::Polygon2D().Put(1, D3DXVECTOR3(RNLib::Window().GetHeight() - m_fMove, RNLib::Window().GetHeight() / 3 - RNLib::Window().GetHeight() / 2, 0.0f), 0.0f, true)
	//	->SetCol(Color{ m_col.r,m_col.g,m_col.b,(UShort)(m_col.a * rate) })
	//	->SetSize(m_fMove, RNLib::Window().GetHeight() / 2);

	m_fMove++;

}