//========================================
// 
// ドール3Dマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ドール3Dマネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Doll3DMgr::_RNC_Doll3DMgr() : CObjectMgr("RN_DollMgr") {

	m_editInfo = {};
}

//========================================
// デストラクタ
//========================================
_RNC_Doll3DMgr::~_RNC_Doll3DMgr() {

}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]ドール3Dマネージャークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 更新処理
//========================================
void _RNC_Doll3DMgr::Update(void) {

}