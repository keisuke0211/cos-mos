//========================================
// 
// ゴールゲートの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// モードの情報構造体
class CGoalGate : public CStageObject {
public:

	//========== [[[ 関数宣言 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	//設定
	void   SetGoal(bool bGoal) { m_bGoal = bGoal; }				//ゴール

	//取得
	bool   GetGoal(void) { return m_bGoal; }					//ゴール

protected:

private:

	//========== [[[ 変数宣言 ]]]
	int m_TexIdx[2];		//モデル番号
	bool m_bGoal;			//ゴールしたか
};
