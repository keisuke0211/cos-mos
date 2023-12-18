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
class CMenuUI;
class CFontText;
class CWords;
class CCoinUI;

//****************************************
// クラス定義
//****************************************
// モード(タイトル)クラス
class CMode_Title :public CMode{
public:
	//========== [[[ 定数定義 ]]]
	static const int WORDS_MAX = 7;		// 文字の最大数

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE,
	};
	enum TITLE {
		TITLE_TITLE = 0,	// タイトル
		TITLE_OUTSET,		// 演出終了
		TITLE_MENU_ANIME,	// メニュー演出
		TITLE_MENU,			// メニュー
		TITLE_SELECT,		// ステージ選択
		TITLE_NEXT,			// 次の画面に移動 
		TITLE_MAX
	};	

	//========== [[[ 関数宣言 ]]]
	static void SetSelect(bool bSelect) { m_bStageSelect = bSelect; }
	CMode_Title();
	~CMode_Title();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

	static int m_nPlanetIdx;
	static int m_nStageSelect;

private:
	//========== [[[ 列挙型定義 ]]]
	enum MENU{
		MENU_GAME = 0,	// ゲーム
		MENU_CONTROLLER,// 操作方法
		MENU_SETTING,	// 設定
		MENU_END,		// 終了
		MENU_MAX
	};
	enum TEX{
		TEX_BG = 0,		// 背景
		TEX_PLANET,		// 惑星
		TEX_NUM,		// 数字
		TEX_MAX
	};
	enum TEXT {
		TEXT_TITLE = 0,	// タイトル
		TEXT_MENU,		// メニュー
		TEXT_ALL,		// 全部
		TEXT_MAX
	};
	enum STAGE {
		NONE = 0,	// 何もない
		POP,		// 出現
		DESPAWN		// 消滅
	};

	//========== [[[ 構造体定義 ]]]
	// ステージ種類情報
	struct PlanetType{
		int nModel;				// モデル
	};

	// *** 関数 ***
	void MenuAnime(void);
	void TextAnime(void);
	void CreateStageSelectInfo(void);
	void StageSelect(void);
	void StageDraw(int nPlanet, int nStage, D3DXVECTOR3 poscor,float &RktAnimRt);
	void StagePop(int nPlanet,int &nStage,D3DXVECTOR3 poscor);
	void TextRelease(TEXT type);
	void SwapMode(TITLE aTitle);

	// *** 静的変数 ***
	static CMenuUI *m_MenuUI;
	static bool m_bStageSelect;

	// *** 変数 ***
	TITLE Title;
	TITLE NextTitle;
	D3DXVECTOR3 m_BgPos[TEX_MAX];
	D3DXVECTOR3 m_RocketPos;
	D3DXVECTOR3 m_RocketposDiff;
	D3DXVECTOR3 m_RocketPosOld;
	D3DXVECTOR3 m_RocketRot;
	D3DXVECTOR3 m_RocketRotOld;
	D3DXVECTOR3 m_RocketRotDiff;
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_EffTex;
	int m_nSelectTemp;
	int m_nOldSelect;
	int m_nOldnPlanet;
	int m_nDrawPlanet;
	int m_RocketIdx;
	int m_SelIdx;
	int m_StgBoardIdx;
	int m_CoinBoardIdx;
	int m_ArrowIdx;
	int *m_AnimCnt;
	int m_RocketAnimCnt;
	int m_NumAnimCnt;
	int m_ImageStgCnt;
	int m_RotCnt;
	int m_nCnt;
	bool m_bMove[WORDS_MAX];
	bool m_bBackMode;
	bool m_bStageChange;
	bool m_bRocketMove;
	bool m_bRocketRot;
	bool m_bRotDir;
	CWords *m_TITLE[WORDS_MAX];
	CWords *m_TitleShadow[WORDS_MAX];
	CFontText *m_pMenu[MENU_MAX];
	PlanetType *m_PlanetType;
	STAGE m_StgFlag;
	CCoinUI *m_CoinUI;
};