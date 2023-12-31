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
_RNC_Count::_RNC_Count() {

	m_count     = 0;
	m_isBlinkF2 = false;
	m_isBlinkF4 = false;
}

//========================================
// デストラクタ
//========================================
_RNC_Count::~_RNC_Count() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Count::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Count::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Count::Update(void) {

	m_count     = (m_count + 1) % ULONG_MAX;
	m_isBlinkF2 = (m_count % 4 < 2);
	m_isBlinkF4 = (m_count % 8 < 4);
}