//========================================
// 
// ドール3Dマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]ドール3Dマネージャークラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDoll3DMgr::CDoll3DMgr() : CObjectMgr("RN_DollMgr") {

	m_editInfo = {};
}

//========================================
// デストラクタ
//========================================
CDoll3DMgr::~CDoll3DMgr() {

}

//========================================
// 更新処理
//========================================
void CDoll3DMgr::Update(void) {

}