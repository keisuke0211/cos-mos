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
class CEffect3DMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CEffect3DMgr();
	~CEffect3DMgr();
	void Update(void);
};