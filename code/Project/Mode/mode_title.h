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
	// *** 定義 ***
	static const int WORDS_MAX = 7;		// 文字の最大数

	// *** 列挙型 ***
	enum class STATE {
		NONE,
	};

	// タイトルモード
	enum TITLE
	{
		TITLE_PLANET = 0,	// 惑星
		TITLE_TITLE,		// タイトル
		TITLE_OUTSET,		// 演出終了
		TITLE_MENU_ANIME,	// メニュー
		TITLE_MENU,			// メニュー生成
		TITLE_SELECT,		// ステージ選択
		TITLE_NEXT,			// 次の画面に移動 
		TITLE_MAX
	};

	// メニュー
	enum MENU
	{
		MENU_GAME = 0,	// ゲーム
		MENU_SERRING,	// 設定
		MENU_END,		// 終了
		MENU_MAX
	};

	// *** 情報構造体 ***

	// ステージ種類情報
	struct PlanetType
	{
		int nTex;				// 画像
		char Text[TXT_MAX];		// ステージ名
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

	// テクスチャ
	enum TEX
	{
		TEX_BG = 0,		// 背景
		TEX_PLANET00,	// 惑星
		TEX_PLANET01,	// 惑星
		TEX_ICON,		// アイコン
		TEX_MAX
	};

	// *** 関数 ***
	/* 惑星アニメメーション		*/void PlanetAnime(void);
	/* タイトルアニメーション	*/void TitleAnime(void);
	/* メニューアニメーション	*/void MenuAnime(void);
	/* メニュー生成				*/void MenuCreate(void);
	/* メニュー					*/void Menu(void);
	/* ステージ選択生成			*/void SelectCreate(void);
	/* ステージ選択				*/void StageSelect(void);
	/* テキスト削除				*/void TextClear(void);
	/* モード切り替え			*/void SwapMode(TITLE aTitle);

	// *** 静的変数 ***
	static bool s_bStageSelect;

	// *** 変数 ***
	TITLE Title;
	D3DXVECTOR3 m_BgPos[TEX_MAX];
	D3DXVECTOR3 m_MenuPos[MENU_MAX];
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_nSelect;
	int m_nOldSelect;
	int m_nPlanetIdx;
	int m_nOldnPlanet;
	bool m_bMove[WORDS_MAX];
	bool m_bMenuAnime;
	bool m_bBackMode;
	bool m_bBackAnime;
	CWords *m_Words[WORDS_MAX];
	CWords *m_WordsShadow[WORDS_MAX];
	CFontText *m_Menu[MENU_MAX];
	PlanetType *m_PlanetType;
};