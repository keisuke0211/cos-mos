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

		int		nNumCoin;			// 解放に必要なコイン数
		int		nLimitTime;			// 制限時間
		bool	bRelease;			// ステージが解放されたか
	};

	// 惑星種類情報
	struct PlanetType
	{
		char	aTexFile[0xff];		// テクスチャパス
		Color	color;				// 背景色
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
		D3DXVECTOR3 CameraPos;	// カメラ位置
		int nPlanetIdx;			// 現在の惑星
		char	*aBgFile;		// 背景
		char	*aSoundFile;	// サウンド

		// 最大値
		int nPlanetMax;	// 最大惑星
		int nLiftMax;	// リフトの最大
		int nMateorMax;	// 隕石の最大
		int nLaserMax;	// レーザーの最大
		int nDogMax;	// 犬の最大値
		int nPileMax;	// 杭の最大値
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

	// リフト情報
	struct Liftinfo
	{
		D3DXVECTOR3 posV;	// 視点位置
		D3DXVECTOR3 posR;	// 終点位置
		D3DXVECTOR3 move;	// 移動量
	};

	// 隕石情報
	struct MeteorInfo
	{
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 move;	// 移動量
		int inteval;		// 生成間隔
	};

	// レーザー情報
	struct LaserInfo
	{
		D3DXVECTOR3 posV;	// 視点位置
		D3DXVECTOR3 posR;	// 終点位置
		D3DXVECTOR3 move;	// 移動量
	};

	// Dog情報
	struct DogInfo
	{
		D3DXVECTOR3 HeadPos;// 頭の位置
		D3DXVECTOR3 HipPos;	// 尻の位置
		int HeightMin;		// 胴体の高さ 最低値
		int HeightMax;		// 　　　　　 最大値
		bool bElasticity;	// 伸縮フラグ
	};

	// 杭の情報
	struct PileInfo
	{
		D3DXVECTOR3 pos;	// 位置
		int nNumPile;		// 杭の数
		float fCaveIn;		// 減りこみ量
	};


	// *** 関数宣言 ***
	CStageEditor();
	~CStageEditor();
	/* 終了*/ void Uninit();

	/* ステージ切り替え	*/void SwapStage(int nStageIdx);

	// -- 取得 ---------------------------------------------
	/* ステージ解放		*/void SetStageRel(int planet, int stage,bool bRel);


	// -- 取得 ---------------------------------------------
	/* 解放のコイン数	*/int GetStageCoin(int planet, int stage);
	/* 制限時間      	*/int GetStageLimittime(int planet, int stage);
	/* ステージ解放		*/bool GetStageRel(int planet, int stage);
	/* 行数の最大値		*/int GetRowMax(void) { return m_Info.nRowMax; }
	/* 列数の最大値		*/int GetLineMax(void) { return m_Info.nLineMax; }
	/* 最大値			*/int GetPlanetMax(void) { return m_Info.nPlanetMax; }
	/* 現在のステージ	*/int GetPlanetIdx(void) { return m_Info.nPlanetIdx; }
	/* 惑星種類情報		*/PlanetType *GetType(void) { return m_PlanetType; }
	/* カメラの初期位置	*/D3DXVECTOR3 GetCameraPos(void) { return m_Info.CameraPos; }
	/* 背景				*/char *GetBgFile(void) { return m_Info.aBgFile; }
	/* サウンド			*/char *GetSoundFile(void) { return m_Info.aSoundFile; }
	/* 位置				*/D3DXVECTOR3 GetPos(int nRow, int nLine);
	/* ステージ総数取得 */void GetPlanetAndStageMax(CInt planet, int& NumPlanet, int& NumStage);

	// -- 変換 ---------------------------------------------
	/* int		*/bool ToData(int &val, CSVFILE *pFile, int nRow, int nLine);
	/* float	*/bool ToData(float &val, CSVFILE *pFile, int nRow, int nLine);
	/* ushort	*/bool ToData(UShort& val, CSVFILE* pFile, int nRow, int nLine);
	/* double	*/bool ToData(double &val, CSVFILE *pFile, int nRow, int nLine);

	// -- 読込 ---------------------------------------------
	/* ファイルパス	*/void FileLoad(void);
	/* ステージ読込	*/void StageLoad(int planet,int stage);

private:

	// *** 列挙型 ***
	enum Type
	{
		TYPE_BLOCK         = 0,		// ブロック
		TYPE_TRAMPOLINE    = 1,		// トランポリン
		TYPE_SPIKE         = 2,		// 棘
		TYPE_SPIKE_L	   = 24,	// 棘左
		TYPE_SPIKE_R	   = 25,	// 棘右
		TYPE_LIFT		   = 3,		// リフト
		TYPE_Meteor        = 4,		// 隕石
		TYPE_Laser         = 5,		// レーザー
		TYPE_Extenddog     = 6,		// ヌイ
		TYPE_COIN          = 7,		// コイン
		TYPE_MAGMA_BLOCK   = 8,		// マグマブロック
		TYPE_FILL_BLOCK_11 = 11,	// ブロック(判定 無) 1 * 1
		TYPE_FILL_BLOCK_12 = 12,	// ブロック(判定 無) 1 * 2
		TYPE_FILL_BLOCK_13 = 13,	// ブロック(判定 無) 1 * 3
		TYPE_FILL_BLOCK_21 = 21,	// ブロック(判定 無) 2 * 1
		TYPE_FILL_BLOCK_22 = 22,	// ブロック(判定 無) 2 * 2
		TYPE_FILL_BLOCK_31 = 31,	// ブロック(判定 無) 3 * 1
		TYPE_FILL_BLOCK_33 = 33,	// ブロック(判定 無) 3 * 3
		TYPE_FILL_BLOCK_44 = 44,	// ブロック(判定 無) 4 * 4
		TYPE_PLAYER_0      = 90,	// １Ｐ
		TYPE_PLAYER_1      = 91,	// ２Ｐ
		TYPE_GOALGATE      = 97,	// ゴールゲート
		TYPE_PARTS         = 98,	// パーツ
		TYPE_GOAL          = 99,	// ゴール
		TYPE_MAX
	};

	// *** 関数宣言 ***
	/* ステージ色	*/void StgColor(CSVFILE *pFile, int nRow, int nLine);
	/* 色設定		*/void SetColor(CSVFILE *pFile, int nRow, int nLine);
	/* リフト設定	*/void SetLiftInfo(CSVFILE *pFile, int nRow, int nLine);
	/* 隕石設定		*/void SetMeteorInfo(CSVFILE *pFile, int nRow, int nLine);
	/* レーザー設定	*/void SetLaserInfo(CSVFILE *pFile, int nRow, int nLine);
	/* ヌイ設定		*/void SetDogInfo(CSVFILE *pFile, int nRow, int nLine);
	/* 杭の設定		*/void SetPileInfo(CSVFILE *pFile, int nRow, int nLine);
	/* OBJ配置		*/void ObjPlace(float fSizeX, float fSizeY, D3DXVECTOR3 pos,int nType);

	/* ステージ生成 */void SetStage(int nType);

	// メモリ開放
	// 惑星情報まで開放するかどうか（trueで開放
	void Release(bool bPlanetRelease = false);

	// *** 変数宣言 ***
	static PlanetType *m_PlanetType;	// 惑星情報
	static StageColor m_StageColor;		// 色情報
	Liftinfo *m_LiftInfo;				// リフト情報
	MeteorInfo *m_MeteorInfo;			// 隕石情報
	LaserInfo *m_LaserInfo;				// レーザー情報
	DogInfo *m_DogInfo;					// 犬の情報
	PileInfo *m_PileInfo;				// 杭の情報

	StageInfo m_Info;					// ステージ情報

	bool bRocket;
};
