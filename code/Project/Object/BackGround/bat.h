//========================================
// 
// 蝙蝠の処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// 惑星の情報構造体
class CBat : public CStageObject {
public:

	//========== [[[ 関数宣言 ]]]
	CBat();
	virtual        ~CBat();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }

protected:

private:
	//========== [[[ 変数宣言 ]]]
	int ModelIdx;

};

