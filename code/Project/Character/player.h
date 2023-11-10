//================================================================================================
//
//プレイヤー処理[player.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//前方宣言
class CStageObject;
class CMoveBlock;
class CParts;
class CRocket;
class CRoadTripLaser;
class CExtenddog;

//プレイヤークラス
class CPlayer
{
public:
	//表裏どちらの世界に存在するか
	enum class WORLD_SIDE	{
		FACE = 0,	//表
		BEHIND,		//裏
		MAX,
	};

	//当たり判定の順番列挙
	enum class COLLI_VEC {
		X = 0,	//Ⅹベクトル
		Y,		//Ｙベクトル
		MAX
	};

	//当たり判定が働いた方向
	enum class COLLI_ROT {
		NONE = 0,	//何もなし
		OVER,		//上
		UNDER,		//下
		LEFT,		//左
		RIGHT,		//右
		UNKNOWN,	//当たっているけど方向が分からない（当たられる側が動いている可能性アリ
		MAX
	};

	//操作の割り当て
	enum class KEY_CONFIG {
		MOVE_LEFT = 0,	//左移動
		MOVE_RIGHT,		//右移動
		JUMP,			//ジャンプ
		SWAP,			//スワップ
		DECIDE,			//決定
		PAUSE,			//ポーズ
		MAX
	};

	//当たり判定情報
	struct Colli
	{
		D3DXVECTOR3 pos;        //　位置
		D3DXVECTOR3 posOLd;     //　前回位置
		D3DXVECTOR3 MinPos;		// 最小位置
		D3DXVECTOR3 MaxPos;		// 最大位置
		float		fWidth;		// 幅
		float		fHeight;	// 高さ

		COLLI_ROT ColliRot;		// 当たり判定
	};

	//プレイヤー情報
	struct Info
	{
		D3DXVECTOR3 StartPos;     //開始位置

		D3DXVECTOR3 pos;          //位置
		D3DXVECTOR3 posOLd;       //前回位置
		D3DXVECTOR3 rot;          //向き
		D3DXVECTOR3 move;         //移動量
		Color		color;		  //色
		bool		bGround;      //地面に接しているか
		bool		bJump;        //ジャンプ
		bool		bRide;        //ロケットに乗っているかどうか
		bool		bGoal;		  //ゴールしたかどうか
		float		fJumpPower;   //ジャンプ量
		float		fGravity;     //重力
		float		fMaxHeight;   //最高Ｙ座標
		int			nTramJumpCounter;	//トランポリンによって跳ね上がる時間
		float		fTramTargetPosY;	//トランポリン用の目標位置
		bool		bTramJump;			//トランポリン用の特殊ジャンプ
		COLLI_ROT	TramColliRot;		//トランポリン用の当たり判定
		bool		bExtendDog;	  //ヌイ用の接触フラグ
		int			nModelIdx;    //モデル番号
		WORLD_SIDE  side;         //どちらの世界に存在するか
		int             Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; //キーボードのキー配置
		CInput::BUTTON  JoyPad[(int)KEY_CONFIG::MAX];                         //ジョイパッドのボタン配置
	};

	static const float SIZE_WIDTH;	//横幅
	static const float SIZE_HEIGHT;	//高さ

	static const int SWAP_INTERVAL;	//スワップインターバル
	static const int NUM_PLAYER = 2;//プレイヤーの数

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//----------------------------
	//プレイヤー生成
	//プレイヤーのポインタを返します
	//----------------------------
	static CPlayer *Create(void);

	//プレイヤー情報設定
	//指定された番号のプレイヤー情報を設定します。
	void SetInfo(const Info info, const int nNum);

	//プレイヤー情報設定
	//各プレイヤーの位置情報などを引数に渡してください。
	//構造体「Info」に関してはplayer.hのクラス宣言を確認してください。
	void SetInfo(Info p1, Info p2);

	//プレイヤー位置情報設定
	//指定したプレイヤーの位置情報を引数に渡してください。
	void SetPos(const int nNum, D3DXVECTOR3 pos) { m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = pos; }

	//プレイヤー色情報設定
	//指定したプレイヤーの色情報を引数に渡してください。
	void SetColor(const int nNum, Color color) { m_aInfo[nNum].color = color; }

	//プレイヤーにトランポリン用のジャンプを設定
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

	//プレイヤー情報取得
	//各引数にプレイヤー情報のアドレスを渡します
	void GetInfo(Info*& pP1, Info*& pP2) { pP1 = &m_aInfo[0]; pP2 = &m_aInfo[1]; }

	//プレイヤー情報取得
	//指定された番号のプレイヤー情報のアドレスを返します
	Info *GetInfo(int nNum) { return &m_aInfo[nNum]; }

	//プレイヤー情報取得
	//指定された世界にいるプレイヤーの情報を返します
	Info *GetInfo(WORLD_SIDE side);

	//取得したパーツ数
	int GetNumParts(void) { return s_nNumGetParts; }

	//スワップインターバルを設定
	//既にインターバルがあれば設定しない
	static void SetSwapInterval(void) { s_nSwapInterval = s_nSwapInterval == 0 ? SWAP_INTERVAL : s_nSwapInterval; }

private:
	static int	s_nSwapInterval;//残りスワップインターバル

	static const float MOVE_SPEED;		//移動量
	static const float MAX_MOVE_SPEED;	//最大移動量

	static const float JUMP_POWER;		//基本ジャンプ量
	static const float GRAVITY_POWER;	//基本重力加速度

	static const int TRAMPOLINE_JUMP_COUNTER;
	static int	s_nNumGetParts;	//取得したパーツの数
	static bool	s_bRideRocket;	//ロケットに乗れるかどうか

	static const int OBJ_TRAMPOLINE = 2;// オブジェクトの最大数
	static const int OBJ_EXTENDDOG = 3;	// オブジェクトの最大数

	void InitKeyConfig(void);//各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
	void SetPosOld(void);
	void ActionControl(void);
	void Move(COLLI_VEC vec);
	void Swap(void);
	void Death(D3DXVECTOR3 *pDeathPos);//死んだ場所を引数に指定（死亡パーティクルなどを描画するのに使用する

	void WholeCollision(void);

	void CntrPos(void);	// 範囲外の制御

	//========================
	//対象物の中にめり込んでいるかどうか判定
	//------------------------
	// 引数１	pos			：現在位置
	// 引数２	posOld		：前回位置
	// 引数３	fWidth		：幅
	// 引数４	fHeight		：高さ
	// 引数５	TargetPos	：対象の現在位置
	// 引数６	TargetPosOld：対象の前回位置（オブジェクトにPosOld変数が無いなら、現在位置をいれればOK
	// 引数７	TargetWidth	：対象の幅
	// 引数８	TargetHeight：対象の高さ
	// 引数９	value		：ベクトル
	// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
	//========================
	COLLI_ROT IsBoxCollider(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetPosOld, float TargetWidth, float TargetHeight, COLLI_VEC value);

	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY,float fHeight);	//上からの当たり判定による位置・移動量修正
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);	//下からの当たり判定による位置・移動量修正
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);	//左からの当たり判定による位置・移動量修正
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);	//右からの当たり判定による位置・移動量修正

	void CollisionBlock(Info *pInfo, Colli *pColli);
	void CollisionFillBlock(Info *pInfo,COLLI_ROT ColliRot);
	void CollisionTrampoline(Info *pInfo, Colli *pColli, Colli *pOthColli);
	void CollisionSpike(Info *pInfo, Colli *pColli);
	void CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, Colli *pColli);
	void CollisionMeteor(Info *pInfo, Colli *pColli);
	void CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, Colli *pColli, Colli *pOthColli);
	void CollisionDog(Info *pInfo, CExtenddog *pExtenddog, Colli *pColli, Colli *pOthColli);
	void CollisionGoalGate(Info *pInfo, Colli *pColli);
	void CollisionParts(Info *pInfo, CParts *pParts);
	void CollisionRocket(Info *pInfo, CRocket *pRocket);

	bool IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	bool IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);

	//情報更新処理（更新処理の最後に位置情報などを設定する
	void UpdateInfo(void);

	void OthColliDelete(void);

	Info m_aInfo[NUM_PLAYER];	//各プレイヤーの情報
	Colli *pOthColli;			//他パーツの当たり判定情報
	bool bPluralColli;			//パーツ種類　単体か複数か
	static int s_nSwapMarkTex;  //スワップ先のマークテクスチャ番号
	static int s_nSwapParticle; //スワップ時のパーティクルテクスチャ番号
	short m_jumpSEIdx;			//ジャンプ時のSE番号
	short m_landingSEIdx;		//着地時のSE番号
	short m_dogSEIdx[4];		//壁ギミック用SE番号
};

#endif