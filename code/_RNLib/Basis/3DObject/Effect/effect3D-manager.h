//========================================
// 
// エフェクト3Dマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object-manager.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3Dマネージャークラス
class _RNC_Effect3DMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Effect3DMgr ();
	~_RNC_Effect3DMgr();

private:
	//========== [[[ 関数宣言 ]]]
	void Update(void);
};