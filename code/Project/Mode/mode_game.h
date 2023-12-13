//========================================
// 
// モード:ゲームの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// 前方宣言
//****************************************
class CPlayer;
class CFontText;
class CRocketPartsUI;
class CCoinUI;
class CMenuUI;

//****************************************
// クラス定義
//****************************************
// モード(ゲーム)クラス
class CMode_Game :public CMode {
public:
	//========== [[[ 定数定義 ]]]

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	//========== [[[ 関数宣言 ]]]
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);

	// *** 静的変数 ***
	static CMenuUI *m_MenuUI;

private:
	//========== [[[ 関数宣言 ]]]

};