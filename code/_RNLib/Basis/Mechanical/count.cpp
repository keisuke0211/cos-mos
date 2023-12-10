//========================================
// 
// オブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| オブジェクトクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CCount::CCount() {

	m_count     = 0;
	m_isBlinkF2 = false;
	m_isBlinkF4 = false;
}

//========================================
// デストラクタ
//========================================
CCount::~CCount() {

}

//========================================
// 初期化処理
//========================================
void CCount::Init(void) {

}

//========================================
// 終了処理
//========================================
void CCount::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CCount::Update(void) {

	m_count     = (m_count + 1) % USHRT_MAX;
	m_isBlinkF2 = (m_count % 4 < 2);
	m_isBlinkF4 = (m_count % 8 < 4);
}