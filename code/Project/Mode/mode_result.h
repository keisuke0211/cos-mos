//========================================
// 
// モード:リザルトの処理
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
// モード(リザルト)クラス
class CMode_Result :public CMode {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
	};

	//========== [[[ 関数宣言 ]]]
	CMode_Result();
	~CMode_Result();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

private:

};