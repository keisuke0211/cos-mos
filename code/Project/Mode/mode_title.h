//========================================
// 
// モード:タイトルの処理
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// 前方宣言
//****************************************
class CFontText;
class CWords;

//****************************************
// クラス定義
//****************************************
// モード(タイトル)クラス
class CMode_Title :public CMode {
public:
	//========== [[[ 定数定義 ]]]
	static const char* TEXT_FILE;				// テキスト情報のファイルパス
	static const int WORDS_MAX = 7;				// 文字の最大数
	static const int FONT_TEXT_MAX = 8;		// テキストの最大数
	static const int PAUSE_LEFT_ANIME = 15;		// 画面左のアニメーション時間
	static const int PAUSE_RIGHT_ANIME = 15;	// 画面右のアニメーション時間

	// *** 列挙型 ***
	enum class STATE {
		NONE,
	};

	// タイトルモード
	enum TITLE
	{
		TITLE_TITLE = 0,	// タイトル
		TITLE_OUTSET,		// 演出終了
		TITLE_MENU_ANIME,	// メニュー演出
		TITLE_MENU,			// メニュー
		TITLE_SELECT,		// ステージ選択
		TITLE_NEXT,			// 次の画面に移動 
		TITLE_MAX
	};	

	// *** 関数 ***
	CMode_Title();
	~CMode_Title();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

	/*　タイトルモード	*/static void SetSelect(bool bSelect) { s_bStageSelect = bSelect; }

private:
	// *** 列挙型 ***
	
	// メニュー
	enum MENU{
		MENU_GAME = 0,	// ゲーム
		MENU_CONTROLLER,// 操作方法
		MENU_SERRING,	// 設定
		MENU_END,		// 終了
		MENU_MAX
	};

	// テクスチャ
	enum TEX{
		TEX_BG = 0,		// 背景
		TEX_PLANET00,	// 惑星
		TEX_PLANET01,	// 惑星
		TEX_ICON,		// アイコン
		TEX_MAX
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

	// テキスト
	enum TEXT {
		TEXT_TITLE = 0,	// タイトル
		TEXT_MENU,		// メニュー
		TEXT_RIGHT,		// 左画面
		TEXT_ALL,		// 全部
		TEXT_MAX
	};

	// *** 構造体 ***

	// ステージ種類情報
	struct PlanetType{
		int nModel;				// モデル
		char Text[TXT_MAX];		// ステージ名
	};

	// 操作方法のテキスト情報
	struct Operation{
		char Text[TXT_MAX];		// テキスト
	};

	// ステージ種類情報
	struct Setting{
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
		int nSelect;
		int nRightTextType;
		bool bMenu;
		bool bRightMove;
		bool bRightDisp;
		bool bClose;

		int BoxTex;
		int OperationMax;
		int SettingMax;

		Operation *pOperation;
		Setting *pSetting;
	};

	// *** 関数 ***
	/* テキストの読込			*/void TextLoad(void);
	/* タイトルアニメーション	*/void TitleAnime(void);
	/* メニューアニメーション	*/void MenuAnime(void);
	/* メニュー生成				*/void MenuCreate(void);
	/* メニュー選択				*/void MenuSelect(void);
	/* サブテキストの生成		*/void SubTextCreate(void);
	/* ステージ読込				*/void StageLoad(void);
	/* ステージ選択				*/void StageSelect(void);
	/* テキスト削除				*/void TextRelease(TEXT type);
	/* モード切り替え			*/void SwapMode(TITLE aTitle);

	// *** 静的変数 ***
	static bool s_bStageSelect;

	// *** 変数 ***
	TITLE Title;
	Menu m_Anime;
	D3DXVECTOR3 m_BgPos[TEX_MAX];
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_nSelect;
	int m_nOldSelect;
	int m_nPlanetIdx;
	int m_nOldnPlanet;
	bool m_bMove[WORDS_MAX];
	bool m_bBackMode;
	CWords *m_TITLE[WORDS_MAX];
	CWords *m_TitleShadow[WORDS_MAX];
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
	PlanetType *m_PlanetType;

	char m_RightTxt[INPUT_MAX][TXT_MAX] = {
		{ "操作方法" },
		{ "・移動　　：Ω" },
		{ "・ジャンプ：Σ" },
		{ "・スワップ：Φ" },
		{ "・決定　　：Д" },
		{ "・戻る　　：Σ" },
	};
};