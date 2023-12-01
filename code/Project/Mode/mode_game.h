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
	//========== [[[ 定数定義 ]]]
	static const char* TEXT_FILE;				// テキスト情報のファイルパス
	static const int PAUSE_LEFT_ANIME  = 20;	// 画面左のアニメーション時間
	static const int PAUSE_RIGHT_ANIME = 20;	// 画面右のアニメーション時間
	static const int FONT_TEXT_MAX = 8;			// テキストの最大数
	static const int VOLUME_MSX = 20;			// サウンドの最大値
	static const int COOLDOWN = 20;				// クールダウン

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	// メニュー
	enum MENU {
		MENU_RESUME = 0,// 続ける
		MENU_RESET,		// やり直す
		MENU_SELECT,	// ステージ選択
		MENU_CONTROLLER,// 操作方法
		MENU_SETTING,	// 設定
		MENU_MAX
	};

	// 操作方法
	enum CONTROLLER {
		INPUT_TITLE = 0,// タイトル
		INPUT_MOVE,		// 移動
		INPUT_JUMP,		// ジャンプ
		INPUT_SWAP,		// スワップ
		INPUT_DECISION,	// 決定
		INPUT_BACK,		// 戻る
		INPUT_MAX
	};

	// 設定
	enum SETTING {
		SETTING_SCREEN = 1,	// フルスクリーン
		SETTING_BGM,		// BGM
		SETTING_SE,			// SE
		SETTING_BACK,
		SETTING_BGM_TEXT,
		SETTING_SE_TEXT,
		SETTING_MAX
	};

	// テキスト
	enum TEXT {
		TEXT_MENU = 0,	// メニュー
		TEXT_RIGHT,		// 左画面
		TEXT_ALL,		// 全部
		TEXT_MAX
	};

	//========== [[[ 構造体定義 ]]]

	// 操作方法のテキスト情報
	struct Operation {
		char Text[TXT_MAX];		// テキスト
	};

	// 設定情報
	struct Setting {
		char Text[TXT_MAX];		// テキスト
	};

	// ポーズ情報
	struct Pause {
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
		int nCntLeftAnime;
		int nCntRightAnime;
		int nRightCoolDown;	//　左画面出現のクールダウン
		int nMaineSelect;
		int nMaineOldSelect;
		int nSubSelect;
		int nRightTextType;
		bool bMenu;
		bool bRightMove;
		bool bRightDisp;
		bool bRightCoolDown;
		bool bClose;
		bool bSubMenu;

		int BoxTex;
		int OperationMax;
		int SettingMax;

		// スクリーン
		int nCntScrChg;		// スクリーン変更のカウント
		bool bFullScreen;	// スクリーンモード

		// サウンド
		int nBGMVolume;
		int nSEVolume;
		int nBGMOldVolume;
		int nSEOldVolume;

		Operation *pOperation;
		Setting *pSetting;
	};

	//========== [[[ 関数宣言 ]]]
	static CPlayer* GetPlayer(void);
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);
	// -- 設定 -------------------------------------------------------------------
	static void SetStage(int planet, int stage) { m_nPlanetIdx = planet; m_nStageIdx = stage; }
	static void SetBgUpColor(Color color) { m_BgColorUp = color; }
	static void SetBgDownColor(Color color) { m_BgColorDown = color; }
	static void SetRocketParts(CRocketPartsUI *rctpar) { m_rocketparts = rctpar; }
	// -- 取得 -------------------------------------------------------------------
	static int GetStage(void) { return m_nStageIdx; }

private:
	//========== [[[ 関数宣言 ]]]
	void PauseCreate(void);
	void PauseSelect(void);
	void PauseAnime(void);
	void PauseMenu(void);
	void SubTextCreate(void);
	void TextLoad(void);
	void TextRelease(TEXT type);

	//========== [[[ 変数宣言 ]]]
	static CPlayer *s_pPlayer;
	static int m_nPlanetIdx;
	static int m_nStageIdx;
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
	static CRocketPartsUI *m_rocketparts;
	static Color m_BgColorUp, m_BgColorDown;
	short m_wallModelIdx[2];
	Pause m_Pause;
	short m_BGMIdx;
	CSound::CPlay* play;
};