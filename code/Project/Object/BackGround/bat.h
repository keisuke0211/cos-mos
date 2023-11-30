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
	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		STATE_DEFAULT,
		STATE_FLY,
		STATE_MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CBat();
	virtual        ~CBat();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SeState(STATE state) { m_State = state; }	// 状態の設定

protected:

private:
	//========== [[[ 変数宣言 ]]]
	CDoll3D* m_doll;
	STATE m_State;
	D3DXVECTOR3 m_move;
	int m_nFlyCounter;
	int m_nStayCounter;
	int m_nIdxMotion;
	bool m_bUpDown;
};

