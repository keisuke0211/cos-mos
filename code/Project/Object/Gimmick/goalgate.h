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

	//========== [[[ 列挙型定義 ]]]
	enum class RAINBOW {
		RED = 0,	//赤
		PURPLE,		//紫
		BLUE,		//青
		LIGHT_BLUE,	//水
		GREEN,		//緑
		YELLOW,		//黄
		MAX		
	};

	//========== [[[ 関数宣言 ]]]
	CGoalGate();
	virtual        ~CGoalGate();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void SetEntry(bool bEntry);
	void SetStartGate(bool bStartGate) { m_bStartGate = bStartGate; }

	bool GetStartGate(void) { return m_bStartGate; }

	static void EntrySub(void) { s_numEntry--; }

	static void ResetEtr(void) { s_numEntry = 0; }

protected:

private:

	void StateUpdate(void);
	void ColUpdate(void);
	void CountRate(float *CountRateX, float *CountRateY,float *CountRateZ);
	void EscapeGuide(void);

	//========== [[[ 変数宣言 ]]]
	STATE m_state;							//種類
	RAINBOW m_Rainbow;						//色状態
	Color m_col;							//色
	Color m_RainbowCol[(int)RAINBOW::MAX];	//色
	static int s_modelIdx;					//モデル番号
	static int s_TexIdx[2];					//テクスチャ番号
	Scale3D m_scale;						//拡縮
	int		m_nCnt;							//カウント
	bool	m_bEntry;						//入ったかどうか
	bool	m_bScale;						//拡大するか縮小するか
	int		m_nCntEtrX,m_nCntEtrY;			//XYの個別カウント
	static int s_num;
	static int s_numEntry;
	bool	m_bStartGate;
	bool	m_bCloseGate;

	static const int ESCAPE_GUIDE_POPUP_TIME = 60;
	static int s_nEscapeGuideTexID; // 脱出アイコンのテクスチャID
	int m_nEntryNo;      // エントリーNo
	int m_nEntryCounter; // 入ってからの時間カウンター
	UShort m_GuideAlpha; // ガイドUIのα値
};