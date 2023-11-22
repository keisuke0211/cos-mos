//========================================
// 
// ゴールゲートの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

#define ETR_CNT			(16)	//最大カウント数

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
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void SetEntry(bool bEntry) { 
		m_bEntry = bEntry; 

		if (bEntry == true){
			m_numEntry++;
			m_nCntEtrX = ETR_CNT;
			m_nCntEtrY = ETR_CNT * 0.5;
		}
	}

	static void EntrySub(void) { m_numEntry--; }

protected:

private:

	void StateUpdate(void);
	void CountRate(float *CountRateX, float *CountRateY);

	//========== [[[ 変数宣言 ]]]
	STATE m_state;					//種類
	int m_modelIdx;					//モデル番号
	int m_TexIdx[2];				//テクスチャ番号
	Scale3D m_scale;				//拡縮
	int		m_nCnt;					//カウント
	bool	m_bEntry;				//入ったかどうか
	bool	m_bScale;				//拡大するか縮小するか
	int		m_nCntEtrX,m_nCntEtrY;	//XYの個別カウント
	static int m_num;
	static int m_numEntry;
};
