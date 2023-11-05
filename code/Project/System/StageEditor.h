//========================================
// 
// ステージエディットの読み込み
// Author:KEISUKE OTONO
// 
//========================================
#pragma once
using namespace std;

//****************************************
// 前方宣言
//****************************************
class CSVFILE;

//****************************************
// クラス定義
//****************************************
class CStageEditor {

public:

	// 定数
	static const char* STAGE_INFO_FILE;		// ステージ情報のファイルパス

	// *** 情報構造体 ***

	// ステージ種類情報
	struct StageType
	{
		int		nIdx;				// ステージ番号
		char	aFileName[0xff];	// ファイルパス
		char	aName[0xff];		// ステージ名
	};

	// 惑星種類情報
	struct PlanetType
	{
		char	aTexFile[0xff];		// テクスチャパス
		char	aName[0xff];		// 名前
		StageType *StageType;		// ステージ情報
		int nStageIdx;				// 現在のステージ
		int nStageMax;				// 最大ステージ
	};

	// ステージ情報
	struct StageInfo
	{
		// ファイル関連
		int nRow;		// 行数
		int nLine;		// 列数
		int nRowMax;	// 行数の最大
		int nLineMax;	// 列数の最大

		// ステージ関連
		int nPlanetIdx;	// 現在の惑星
		int nPlanetMax;	// 最大惑星
	};

	// 色
	struct StageColor
	{
		Color Set;
		Color Player1;
		Color Player2;
		Color Block;
		Color FillBlock;
	};

	// *** 関数宣言 ***
	CStageEditor();
	~CStageEditor();
	/* 終了*/ void Uninit();

	/* ステージ切り替え	*/void SwapStage(int nStageIdx);

	// -- 取得 ---------------------------------------------
	/* 最大値			*/int GetPlanetMax(void) { return m_Info.nPlanetMax; }
	/* 現在のステージ	*/int GetPlanetIdx(void) { return m_Info.nPlanetIdx; }
	/* 惑星種類情報		*/PlanetType *GetType(void) { return m_PlanetType; }
	/* 変換				*/bool ToData(int &val, CSVFILE *pFile, int nRow, int nLine);

	// -- 読込 ---------------------------------------------
	/* ファイルパス	*/void FileLoad(void);
	/* ステージ読込	*/void StageLoad(int planet,int stage);

private:

	// *** 列挙型 ***
	enum Type
	{
		TYPE_BLOCK = 0,				// ブロック
		TYPE_TRAMPOLINE,			// トランポリン
		TYPE_SPIKE,					// 棘
		TYPE_LIFT,					// リフト
		TYPE_Meteor,				// 隕石
		TYPE_Laser,					// レーザー
		TYPE_FILL_BLOCK_11 = 11,	// ブロック(判定 無) 1 * 1
		TYPE_FILL_BLOCK_12 = 12,	// ブロック(判定 無) 1 * 2
		TYPE_FILL_BLOCK_13 = 13,	// ブロック(判定 無) 1 * 3
		TYPE_FILL_BLOCK_21 = 21,	// ブロック(判定 無) 2 * 1
		TYPE_FILL_BLOCK_22 = 22,	// ブロック(判定 無) 2 * 2
		TYPE_FILL_BLOCK_31 = 31,	// ブロック(判定 無) 3 * 1
		TYPE_FILL_BLOCK_33 = 33,	// ブロック(判定 無) 3 * 3
		TYPE_FILL_BLOCK_44 = 44,	// ブロック(判定 無) 4 * 4
		TYPE_PLAYER_0 = 90,			// １Ｐ
		TYPE_PLAYER_1,				// ２Ｐ
		TYPE_GOALGATE = 97,			// ゴールゲート
		TYPE_PARTS,					// パーツ
		TYPE_GOAL,					// ゴール
		TYPE_MAX
	};

	// *** 関数宣言 ***
	/* ステージ色	*/void StgColor(CSVFILE *pFile, int nRow, int nLine);
	/* 色設定		*/void SetColor(CSVFILE *pFile, int nRow, int nLine);
	/* OBJ配置		*/void ObjPlace(float fSizeX, float fSizeY, D3DXVECTOR3 pos,int nType);

	/* ステージ生成 */void SetStage(int nType);

	// *** 変数宣言 ***
	static PlanetType *m_PlanetType;	// 惑星情報
	static StageColor m_StageColor;		// 色情報
	StageInfo m_Info;					// ステージ情報
};
