//========================================
// 
// コイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// コインクラス
class CCoin : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CCoin();
	~CCoin();
	void Uninit(void) {}
	void Update(void);

private:

};