//========================================
// 
// カメラマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// クラス定義
//****************************************
// カメラマネージャークラス
class CCameraMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CCameraMgr();
	~CCameraMgr();
	void Update(void);
};