//========================================
// 
// モード:ロゴの処理
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
// モード(ロゴ)クラス
class CMode_Logo :public CMode {
public:

	// ***** 定義 *****
	static const int WAIT_TIME = 60;	// 待機時間
	static const int TIME = 60;			// 推移時間

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
		OPEN,
		OPEN_WAIT,
		CLOSE_WAIT,
		CLOSE,
	};

	//========== [[[ 関数宣言 ]]]
	CMode_Logo();
	~CMode_Logo();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

private:
	static int m_TexLogo;
	static int m_TexBg;
	int m_nStateCtr;
	STATE m_state;

};