//========================================
// 
// デモオブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_object.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]デモオブジェクトクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDemoObject::CDemoObject(const TYPE& type) {

	// リストに追加
	RNDemo::Get().GetDemoObjMgr().AddList(this);

	m_type = type;
}

//========================================
// デストラクタ
//========================================
CDemoObject::~CDemoObject() {

	// リストから削除
	RNDemo::Get().GetDemoObjMgr().SubList(this);
}