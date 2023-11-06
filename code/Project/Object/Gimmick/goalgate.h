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

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		SCALE_UP = 0,		//大きくなる
		SCALE_DOWN,			//小さくなる
		MAX,
	};

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
	STATE m_state;			//種類
	int m_modelIdx;			//モデル番号
	int m_TexIdx[2];		//テクスチャ番号
	bool m_bGoal;			//ゴールしたか
	Scale3D m_scale;	//拡縮
	int			m_nCnt;		//カウント
};
