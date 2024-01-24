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
	static const char* TITLE_LOGO_FILE;				// タイトルロゴ情報のファイルパス
	static const int TITLE_LOGO_MAX = 6;			// タイトルロゴの最大数
	static const int NUI_ANIME = 20;				// ヌイのアニメーション時間
	static const int TITLE_TEXT_ANIME = 40;			// タイトルテキストのアニメーション時間
	static const int COLOR_CHANGE_TIME = 60;		// 色の推移時間
	static const int TEXE_ANIME_STAND_TIME = 120;	// TextAnimeの待機時間
	static const int TEXE_ANIME_INTERVAL_TIME = 60;	// TextAnimeの待機時間

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
	enum TEX{
		TEX_SPACE = 0,	// 宇宙
		TEX_NUM,		// 数字
		TEX_LOCK,		// 錠前
		TEX_MAX
	};
	enum TEXT {
		TEXT_MENU = 0,	// メニュー
		TEXT_ALL,		// 全部
		TEXT_MAX
	};
	enum STAGE {
		NONE = 0,	// 何もない
		POP,		// 出現
		DESPAWN		// 消滅
	};

	enum TITLE_ANIME {
		ANIME_NUI = 0,	// ヌイの出現
		ANIME_TEXT1,	// テキストの出現
		ANIME_NUI_LEAN,	// ヌイを傾ける
		ANIME_TM,		// TM
		ANIME_NONE,		// 無し
	};

	enum TEXT_ANIME {
		TEXT_IN = 0,	// 出現
		TEXT_STAND,		// 待機
		TEXT_OUT,		// 消滅
		TEXT_INTERVAL,	// 間隔
	};

	enum TITLE_LOGO {
		LOGO_COS1 = 0,	// COS
		LOGO_MOS1,		// MOS
		LOGO_NUI,		// ヌイ
		LOGO_COS2,		// コス
		LOGO_MOS2,		// モス
		LOGO_TM,		// TM
	};

	enum DIRECTION {
		RIGHT = 0,	// 右
		LEFT,		// 左
	};
	//========== [[[ 構造体定義 ]]]
	// ステージ種類情報
	struct PlanetType{
		int nModel;				// モデル
	};

	// タイトルロゴ情報 
	struct TitleLogo {
		D3DXVECTOR3 InitPos;// 初期位置
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR2 size;	// サイズ
		Color color;		// 色
		int	TexIdx;			// テクスチャ番号
		float angle;		// 向き

		Pos2D VtxPos[4];	// 頂点位置
		Pos2D TexUV[4];		// テクスチャUV
	};

	// タイトルロゴの共通情報
	struct TitleLogoInfo {
		TitleLogo Logo[TITLE_LOGO_MAX];	// ロゴ情報
		int nCntAnime;					// アニメーションカウンター
	};

	// テキストアニメーションの情報
	struct TextAnimeInfo {
		TEXT_ANIME state = TEXT_IN;	// 状態
		int StandTime;				// 待機時間
		int Interval;				// 間隔
		int nLetterPopCount;		// 次の文字
		int nAppearTime;			// 次の文字が移動するまでの時間
		bool bOut;					// 消滅フラグ
	};

	// *** 関数 ***
	void ColorChange(void);
	void MenuAnime(void);
	void TitleAnime(void);
	void TextAnime(void);
	void CreateStageSelectInfo(void);
	void StageSelect(void);
	void StageDraw(int nPlanet, int nStage, D3DXVECTOR3 poscor,float &RktAnimRt);
	void StagePop(int nPlanet,int &nStage,D3DXVECTOR3 poscor);
	void TextRelease(TEXT type);
	void SwapMode(TITLE aTitle);
	void StageRel(int nPlanet, int nStage);
	void TitleLoad(void);
	void TitleLogoInit(bool bSwitch);

	// *** 静的変数 ***
	static CMenuUI *m_MenuUI;
	static bool m_bStageSelect;

	// *** 変数 ***
	TITLE Title;
	TITLE NextTitle;
	TITLE_ANIME TitleAnima;

	Color BgColor;
	Color BgOldColor;
	Color BgNextColor;
	int nCntColorChange;
	bool bColorChange;

	D3DXVECTOR3 m_BgPos[TEX_MAX];
	Pos2D m_BgTexPthPos[4];
	Pos2D m_BgTexPthMove[4];
	D3DXVECTOR3 m_RocketPos;
	D3DXVECTOR3 m_RocketposDiff;
	D3DXVECTOR3 m_RocketPosOld;
	D3DXVECTOR3 m_RocketRot;
	D3DXVECTOR3 m_RocketRotOld;
	D3DXVECTOR3 m_RocketRotDiff;
	D3DXVECTOR3 m_rotEff;
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_MapIdx;
	int m_EffTex[3];
	int m_nSelectTemp;
	int m_nOldSelect;
	int m_nOldnPlanet;
	int m_nDrawPlanet;
	int m_RocketIdx;
	int m_SelIdx;
	int m_WldBoardIdx[3];
	int m_CoinBoardIdx;
	int m_ArrowIdx;
	int *m_AnimCnt;
	int m_RocketAnimCnt;
	int m_NumAnimCnt;
	int m_ImageStgCnt;
	int m_RotCnt;
	int m_nCnt;
	int m_nStgStartCnt;
	bool m_bBackMode;
	bool m_bWorldChange;
	bool m_bStageChange;
	bool m_bRocketMove;
	bool m_bRocketRot;
	bool m_bRotDir;
	bool m_bStgEnter;

	TextAnimeInfo m_TextAnime;	// テキストアニメーション

	TitleLogoInfo m_TitleLogo;
	CFontText *m_pText;
	PlanetType *m_PlanetType;
	STAGE m_StgFlag;
	CCoinUI *m_CoinUI;
	CRail3D m_RocketRail;
	DIRECTION m_Direction;
};