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
class _RNC_CameraMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_CameraMgr ();
	~_RNC_CameraMgr();

private:
	//========== [[[ 関数宣言 ]]]
	void Update(void);
};