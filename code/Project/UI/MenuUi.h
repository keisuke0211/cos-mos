//========================================
// 
// メニュー
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "../manager.h"

//****************************************
// 前方宣言
//****************************************
class CFontText;

//****************************************
// クラス定義
//****************************************
// モード(タイトル)クラス
class CMenuUI {
public:
	//========== [[[ 定数定義 ]]]
	static const char* TITLE_MENU_FILE;			// メインメニュー情報のファイルパス
	static const char* PAUSE_MENU_FILE;			// メインメニュー情報のファイルパス
	static const char* SUB_MENU_FILE;			// サブメニュー情報のファイルパス
	static const int WORDS_MAX = 7;				// 文字の最大数
	static const int FONT_TEXT_MAX = 10;		// テキストの最大数
	static const int PAUSE_LEFT_ANIME = 20;		// 画面左のアニメーション時間
	static const int PAUSE_RIGHT_ANIME = 20;	// 画面右のアニメーション時間
	static const int VOLUME_MSX = 20;			// サウンドの最大値
	static const int COOLDOWN = 20;				// クールダウン

	//========== [[[ 列挙型定義 ]]]
	enum TITLE_MENU {
		TITLE_MENU_GAME = 0,	// ゲーム
		TITLE_MENU_SETTING,		// 設定
		TITLE_MENU_END,			// 終了
		TITLE_MENU_MAX,
	};

	// メニュー
	enum PAUSE_MENU {
		PAUSE_MENU_RESUME = 0,	// 続ける
		PAUSE_MENU_RESET,		// やり直す
		PAUSE_MENU_SELECT,		// ステージ選択
		PAUSE_MENU_CONTROLLER,	// 操作方法
		PAUSE_MENU_SETTING,		// 設定
		PAUSE_MENU_MAX
	};

	enum CONTROLLER {
		INPUT_TITLE = 0,// タイトル
		INPUT_MOVE,		// 移動
		INPUT_JUMP,		// ジャンプ
		INPUT_SWAP,		// スワップ
		INPUT_DECISION,	// 決定
		INPUT_BACK,		// 戻る
		INPUT_MAX
	};
	enum SETTING {
		SETTING_SCREEN = 1,	// フルスクリーン
		SETTING_BGM,		// BGM
		SETTING_SE,			// SE
		SETTING_BACK,
		SETTING_BGM_TEXT,
		SETTING_SE_TEXT,
		SETTING_MAX
	};
	enum TEXT {
		TEXT_TITLE = 0,	// タイトル
		TEXT_MENU,		// メニュー
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

	// メニュー情報
	struct Menu {
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;

		float LeftScaleX;
		float RightScaleX;

		float LeftScaleMaxX;
		float RightScaleMaxX;

		int nCntLeftAnime;
		int nCntRightAnime;
		int nRightCoolDown;	//　左画面出現のクールダウン
		int nMaineSelect;
		int nMaineOldSelect;
		int nSubSelect;
		int nRightTextType;

		// メニュー
		bool bMenu;				// 生成したかのフラグ
		bool bBackMode;			// 前の画面に戻るかのフラグ
		bool bClose;			// メニュー閉じるかのフラグ
		bool bGameEnd;			// ゲーム終了


		// サブメニュー
		bool bSubMenu;			// 生成したかフラグ
		bool bSubMenuMove;		// 移動方向の切替フラグ
		bool bSubMenuDisp;		// 削除フラグ
		bool SubMenuCD;			// 生成間隔のクールダウン

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

		// ファイル
		int MainMenuMax;
		int SubMenuMax;

		Operation *pOperation;
		Setting *pSetting;
	};


	/* *** 読込関連  *** */

	// テクスチャ情報
	struct Texture {
		char TexFile[0xff];		// ファイルパス
		int PtnIdx;				// パターン	番号
		int PtnX;				//			Xの分割数
		int PtnY;				//			Yの分割数
		bool bSet;				// 設定したか
	};

	// メインメニューの情報
	struct MaineMenu {
		char Text[0xff];		// テキスト
		Texture Tex;

		int nSubMenuID;			// サブメニューの番号
	};

	// サブメニューの情報
	struct SubMenu {
		char Text[0xff];		// テキスト
		Texture Tex;
	};

	// *** 関数 ***
	CMenuUI();
	~CMenuUI();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	static CMenuUI *Create(CMode::TYPE type);

	// -- メニュー関連 -------------------------------------------------------------------
	/* アニメーション	*/void MenuAnime(void);
	/* メインの生成		*/void MenuCreate(void);
	/* サブの生成		*/void SubTextCreate(void);
	/* メニュー選択		*/void MenuSelect(void);

	// -- 入力関連 -----------------------------------------------------------------------
	/* 決定	*/void DecisionInput(void);
	/* 選択	*/void SelectInput(void);


	// -- テキスト関連 -------------------------------------------------------------------
	/* 各種類の解放	*/void TextRelease(TEXT type);

	// -- ファイル関連 -------------------------------------------------------------------
	/* メインメニュー	*/void MaineMenuLoad(CMode::TYPE type);
	/* サブメニュー		*/void SubMenuLoad(void);

	// -- 取得 ---------------------------------------------------------------------------
	Menu GetInfo(void) { return m_Menu; }

	// *** 変数 ***
	bool m_MenuEnd;
	MaineMenu *m_MaineMenu;
	SubMenu *m_SubMenu;
	CFontText *m_pMenu[FONT_TEXT_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
private:
	
	// *** 変数 ***
	Menu m_Menu;
};
