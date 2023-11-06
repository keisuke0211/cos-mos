//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// モードの情報構造体
class CTrampoline : public CStageObject {
public:

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE = 0,		//乗っていない
		UP_LAND,		//上に着地
		DOWN_LAND,		//下に着地
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CTrampoline();
	virtual        ~CTrampoline();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void   SetState(STATE state) { m_state = state; }			//種類設定

	STATE  GetState(void) { return m_state; }					//種類取得

protected:
	
private:

	//========== [[[ 関数宣言 ]]]
	void   Collision(void);	//当たり判定処理

	//========== [[[ 変数宣言 ]]]
	STATE m_state;			//種類
	int m_modelIdx[4];		//モデル番号
	float		m_fJamp;	//差分
	int			m_nCnt;		//カウント
};
