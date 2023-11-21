//================================================================================================
// 
// プレイヤー処理[player.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

// 前方宣言
class CStageObject;
class CMoveBlock;
class CParts;
class CRocket;
class CRoadTripLaser;
class CExtenddog;
class CPile;
class CCollision;

// プレイヤークラス
class CPlayer {
public:
	// 表裏どちらの世界に存在するか
	enum class WORLD_SIDE	{
		FACE = 0,	// 表
		BEHIND,		// 裏
		MAX,
	};

	// 当たり判定の順番列挙
	enum class VECTOL {
		X = 0,	// Ⅹベクトル
		Y,		// Ｙベクトル
		MAX
	};

	// 当たり判定が働いた方向
	enum class COLLI_ROT {
		NONE = 0,	// 何もなし
		OVER,		// 上
		UNDER,		// 下
		LEFT,		// 左
		RIGHT,		// 右
		UNKNOWN,	// 当たっているけど方向が分からない（当たられる側が動いている可能性アリ
		MAX
	};

	// 操作の割り当て
	enum class KEY_CONFIG {
		MOVE_LEFT = 0,	// 左移動
		MOVE_RIGHT,		// 右移動
		JUMP,			// ジャンプ
		SWAP,			// スワップ
		DECIDE,			// 決定
		PAUSE,			// ポーズ
		MAX
	};

	// プレイヤー情報
	struct Info
	{
		D3DXVECTOR3 StartPos;     // 開始位置

		D3DXVECTOR3 pos;          // 位置
		D3DXVECTOR3 posOld;       // 前回位置
		D3DXVECTOR3 rot;          // 向き
		D3DXVECTOR3 move;         // 移動量
		Color		color;		  // 色
		int			nSwapAlpha;   // スワップマークのα値
		bool		bGround;      // 地面に接しているか
		bool		bGroundOld;   // 地面に接しているか(過去)
		bool		bJump;        // ジャンプ
		bool		bRide;        // ロケットに乗っているかどうか
		bool		bGoal;		  // ゴールしたかどうか
		float		fJumpPower;   // ジャンプ量
		float		fGravity;     // 重力
		float		fMaxHeight;   // 最高Ｙ座標
		int			nTramJumpCounter;	// トランポリンによって跳ね上がる時間
		float		fTramTargetPosY;	// トランポリン用の目標位置
		bool		bTramJump;			// トランポリン用の特殊ジャンプ
		COLLI_ROT	TramColliRot;		// トランポリン用の当たり判定
		bool		bExtendDog;	  // ヌイ用の接触フラグ
		bool		bLandPile;	  // 杭に乗っているかどうか
		int			nModelIdx;    // モデル番号
		WORLD_SIDE  side;         // どちらの世界に存在するか
		int             Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; // キーボードのキー配置
		CInput::BUTTON  JoyPad[(int)KEY_CONFIG::MAX];                         // ジョイパッドのボタン配置
	};

	static const float SIZE_WIDTH;	// 横幅
	static const float SIZE_HEIGHT;	// 高さ

	static const int SWAP_INTERVAL;	// スワップインターバル
	static const int NUM_PLAYER = 2;// プレイヤーの数

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//----------------------------
	// プレイヤー生成
	// プレイヤーのポインタを返します
	//----------------------------
	static CPlayer *Create(void);

	// プレイヤー情報設定
	// 指定された番号のプレイヤー情報を設定します。
	void SetInfo(const Info info, const int nNum);

	// プレイヤー情報設定
	// 各プレイヤーの位置情報などを引数に渡してください。
	// 構造体「Info」に関してはplayer.hのクラス宣言を確認してください。
	void SetInfo(Info p1, Info p2);

	// プレイヤー位置情報設定
	// 指定したプレイヤーの位置情報を引数に渡してください。
	void SetPos(const int nNum, D3DXVECTOR3 pos) { m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = pos; }

	// プレイヤー色情報設定
	// 指定したプレイヤーの色情報を引数に渡してください。
	void SetColor(const int nNum, Color color) { m_aInfo[nNum].color = color; }

	// プレイヤーにトランポリン用のジャンプを設定
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

	// 死んだ場所を引数に指定（死亡パーティクルなどを描画するのに使用する
	void Death(D3DXVECTOR3 *pDeathPos);

	// プレイヤー情報取得
	// 各引数にプレイヤー情報のアドレスを渡します
	void GetInfo(Info*& pP1, Info*& pP2) { pP1 = &m_aInfo[0]; pP2 = &m_aInfo[1]; }

	// プレイヤー情報取得
	// 指定された番号のプレイヤー情報のアドレスを返します
	Info *GetInfo(int nNum) { return &m_aInfo[nNum]; }

	// プレイヤー情報取得
	// 指定された世界にいるプレイヤーの情報を返します
	Info *GetInfo(WORLD_SIDE side);

	// スワップインターバルを設定
	// 既にインターバルがあれば設定しない
	static void SetSwapInterval(void) { s_nSwapInterval = s_nSwapInterval == 0 ? SWAP_INTERVAL : s_nSwapInterval; }

	// スワップインターバルを取得
	static int GetSwapInterval(void) { return s_nSwapInterval; }

	//SEラベル
	enum class SE_LABEL {
		JUMP = 0,// ジャンプSE
		LANDING, // 着地SE
		DOG_00,	 // 押す
		DOG_01,	 // 縮む
		DOG_02,	 // 伸びる
		DOG_03,	 // 震える
		SWAP,	 // スワップSE
		MAX
	};

	//プレイヤー用SE再生
	static void PlaySE(SE_LABEL label);

	//パーティクルテクスチャ
	enum class PARTI_TEX {
		SWAP_MARK = 0,	// スワップマーク
		SWAP_PARTI,		// スワップパーティクル
		DEATH_MARK,		// 死亡マーク
		DEATH_PARTI,	// 死亡パーティクル
		GOAL_EFFECT,	// ゴール・ロケット乗車時のエフェクト
		MAX
	};

	//パーティクル番号取得
	static int GetParticleIdx(PARTI_TEX tex) { return s_ParticleTex[(int)tex]; };

private:
	static int s_nSwapInterval; // 残りスワップインターバル

	static const char *PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX];
	static int s_ParticleTex[(int)PARTI_TEX::MAX];
	static const int NORMAL_SWAP_ALPHA = 100;//通常時のスワップマークのα値

	static const float MOVE_SPEED;		// 移動量
	static const float MAX_MOVE_SPEED;	// 最大移動量

	static const float JUMP_POWER;		// 基本ジャンプ量
	static const float GRAVITY_POWER;	// 基本重力加速度

	static const int TRAMPOLINE_JUMP_COUNTER;

	static const int OBJ_TRAMPOLINE = 2;// オブジェクトの最大数
	static const int OBJ_EXTENDDOG = 3;	// オブジェクトの最大数

	void InitKeyConfig(void);// 各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
	void SetPosOld(void);
	void ActionControl(void);
	void Move(VECTOL vec);
	void CtrlPos(Info *pInfo, VECTOL vec);	// 範囲外の制御
	void Swap(void);
	void SwapAnimation(void);

	void CollisionToStageObject(void);

	// 各プレイヤーの当たり判定が終わった後の処理
	void CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type);

	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY,float fHeight);	// 上からの当たり判定による位置・移動量修正
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);	// 下からの当たり判定による位置・移動量修正
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);	// 左からの当たり判定による位置・移動量修正
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);	// 右からの当たり判定による位置・移動量修正

	//void CollisionBlock(Info *pInfo, CollInfo *pColli);
	//void CollisionFillBlock(Info *pInfo,COLLI_ROT ColliRot);
	//void CollisionTrampoline(Info *pInfo, CollInfo *pColli, CTrampoline *pTrampoline);
	//void CollisionSpike(Info *pInfo, CollInfo *pColli);
	//void CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, CollInfo *pColli);
	//void CollisionMeteor(Info *pInfo, CollInfo *pColli);
	//void CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, CollInfo *pColli, CollInfo *pOthColli);
	//void CollisionDog(Info *pInfo, CExtenddog *pExtenddog, CollInfo *pColli, CollInfo *pOthColli);
	//void CollisionGoalGate(Info *pInfo, CollInfo *pColli);
	//void CollisionParts(Info *pInfo, CParts *pParts);
	//void CollisionRocket(Info *pInfo, CRocket *pRocket);
	//void CollisionPile(Info *pInfo, CollInfo *pColli, CPile *pPile);

	bool IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	bool IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);

	// 情報更新処理（更新処理の最後に位置情報などを設定する
	void UpdateInfo(void);

	Info m_aInfo[NUM_PLAYER];	// 各プレイヤーの情報

	struct SE
	{
		short jump;		// ジャンプSE
		short landing;	// 着地SE
		short dog[4];	// 壁ギミック用SE
		short Swap;		// スワップSE

		CSound *pSound;	// サウンドクラス保管用
	};
	static SE s_SE;		//サウンド用構造体
	static CCollision *s_pColli;
};