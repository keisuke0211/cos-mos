//========================================
// 
// メニュー
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

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
	static const char* TEXT_FILE;				// テキスト情報のファイルパス
	static const int WORDS_MAX = 7;				// 文字の最大数
	static const int FONT_TEXT_MAX = 10;		// テキストの最大数
	static const int PAUSE_LEFT_ANIME = 15;		// 画面左のアニメーション時間
	static const int PAUSE_RIGHT_ANIME = 15;	// 画面右のアニメーション時間
	static const int VOLUME_MSX = 20;			// サウンドの最大値
	static const int COOLDOWN = 20;				// クールダウン

	//========== [[[ 列挙型定義 ]]]
	enum MENU {
		MENU_GAME = 0,	// ゲーム
		MENU_CONTROLLER,// 操作方法
		MENU_SETTING,	// 設定
		MENU_END,		// 終了
		MENU_MAX
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
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
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

		Operation *pOperation;
		Setting *pSetting;
	};

	// *** 関数 ***
	CMenuUI();
	~CMenuUI();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	static CMenuUI *Create(void);

	// -- テキスト関連 -------------------------------------------------------------------
	/* 読込			*/void TextLoad(void);
	/* 各種類の解放	*/void TextRelease(TEXT type);

	// -- メニュー関連 -------------------------------------------------------------------
	/* アニメーション	*/void MenuAnime(void);
	/* メインの生成		*/void MenuCreate(void);
	/* サブの生成		*/void SubTextCreate(void);
	/* メニュー選択		*/void MenuSelect(void);

	// -- 取得 -------------------------------------------------------------------
	Menu GetInfo(void) { return m_Menu; }

	// *** 変数 ***
	bool m_MenuEnd;
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
private:
	
	// *** 変数 ***
	Menu m_Menu;
};
