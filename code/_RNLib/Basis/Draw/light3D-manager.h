//========================================
// 
// ライト3Dマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// クラス定義
//****************************************
// ライト3Dマネージャークラス
class _RNC_Light3DMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Light3DMgr();
	~_RNC_Light3DMgr();
	void Update(void);
};