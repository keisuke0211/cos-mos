//========================================
// 
// コイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
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

	static void AddNumAll(void) { s_NumAll += s_Num; s_Num = 0;}

	static int GetNum(void) { return s_Num; }
	static int GetNumAll(void) { return s_NumAll; }

private:

	int m_nID;
	int m_ModelIdx;
	Color m_color;

	static int s_Num;    //ステージ内で取得したコイン
	static int s_NumAll; //今までのコイン総数
	static int s_CorrNumAll; //今現在のステージのコイン総数
};