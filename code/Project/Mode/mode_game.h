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

	// プレイタイム情報
	struct GameTime {
		DWORD Start; // ゲーム開始時間
		DWORD Pause; // ポーズ中の時間
		DWORD Play;  // プレイ時間を格納
		DWORD End;   // ステージクリア時間

		DWORD LastPause;//最後にポーズだった時間

		bool bMeasure;// 計測するかどうか
	};
	//========== [[[ 関数宣言 ]]]
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);

	//プレイ時間を返す
	static float GetPlayTime(void);

	//計測ON/OFF true：ON  false：OFF
	static void SetMeasureTime(const bool bMeasure) { s_GameTime.bMeasure = bMeasure; }

	static void RestartTime(void);

private:
	//========== [[[ 列挙型定義 ]]]
	enum class TimeType {
		Play = 0, // プレイ時間
		Pause,    // ポーズ時間
		Max
	};

	//========== [[[ 関数宣言 ]]]
	oid MeasureTime(TimeType type);
	static void FormatGameTime(void);

	//========== [[[ 変数宣言 ]]]
	static GameTime s_GameTime; //プレイ時間
};