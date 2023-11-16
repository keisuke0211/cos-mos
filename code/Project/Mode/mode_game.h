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

//****************************************
// クラス定義
//****************************************
// モード(ゲーム)クラス
class CMode_Game :public CMode {
public:
	// *** 定義 ***
	static const int PAUSE_LEFT_ANIME = 20;		// 画面左のアニメーション時間

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	// メニュー
	enum MENU
	{
		MENU_RESUME = 0,// 続ける
		MENU_RESET,		// やり直す
		MENU_SELECT,	// ステージ選択
		MENU_CONTROLLER,// 操作方法
		MENU_SETTING,	// 設定
		MENU_MAX
	};

	// ポーズ情報
	struct Pause
	{
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
		int nCntAnime;
		int nSelect;
		bool bMenu;
		bool bClose;
	};

	//========== [[[ 関数宣言 ]]]
	      CMode_Game    ();
	      ~CMode_Game   ();
	void  Init          (void);
	void  Uninit        (void);
	void  Update        (void);
	void  ProcessState  (const PROCESS process);

	void BackGroundPut  (Color mincol, Color addcol);
	static CPlayer* GetPlayer(void);

	// -- 設定 -------------------------------------------------------------------
	static void SetStage(int planet, int stage) { m_nPlanetIdx = planet; m_nStageIdx = stage; }
	static void SetBgUpColor(Color color) { m_BgColorUp = color; }
	static void SetBgDownColor(Color color) { m_BgColorDown = color; }

	// -- 取得 -------------------------------------------------------------------
	static int GetStage(void)    { return m_nStageIdx;   }
	static CCamera& GetCameraUp(void)   { return *m_cameraUp;   }
	static CCamera& GetCameraDown(void) { return *m_cameraDown; }

private:

	//========== [[[ 関数宣言 ]]]
	void PauseCreate(void);
	void PauseSelect(void);
	void PauseAnime(void);
	void PauseMenu(void);

	//========== [[[ 変数宣言 ]]]
	static CPlayer *s_pPlayer;
	static int m_nPlanetIdx;
	static int m_nStageIdx;
	CFontText *m_Menu[MENU_MAX];
	CRocketPartsUI *m_rocketparts;
	static CCamera* m_cameraUp;
	static CCamera* m_cameraDown;
	static Color m_BgColorUp, m_BgColorDown;
	short m_wallModelIdx[2];
	Pause m_Pause;
};