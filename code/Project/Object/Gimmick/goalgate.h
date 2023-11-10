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
		NONE = 0,	//何もない
		GROW,		//大きくなる
		SMALL,		//小さくなる
		DISAPP,		//消える
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

protected:

private:

	//========== [[[ 変数宣言 ]]]
	STATE m_state;			//種類
	int m_modelIdx;			//モデル番号
	int m_TexIdx[2];		//テクスチャ番号
	Scale3D m_scale;	//拡縮
	int			m_nCnt;		//カウント
};
