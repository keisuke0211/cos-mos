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
		X,	// Ⅹベクトル
		Y,	// Ｙベクトル
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
		Pos3D    StartPos;// 開始位置
		CDoll3D* doll;
		Pos3D    pos;     // 位置
		Pos3D    posOld;  // 前回位置
		Scale3D  scale;
		Pos3D    rot;     // 向き
		Pos3D    move;    // 移動量
		Color    color;   // 色

		//-------------------------------
		//スワップ用
		//-------------------------------
		UShort nSwapAlpha; // スワップマークのα値
		float  fSwapPosY;  // スワップ先のＹ座標
		float  fSwapMoveY; // スワップ移動時の速度
		float  fGuideTexVPos;   // ガイドのテクスチャＶ座標
		float  fGuideTexVSize;  // ガイドのテクスチャＶサイズ
		float  fGuideMoveSpeed; // ガイドのテクスチャ移動スピード

		//-------------------------------
		// ジャンプ用
		//-------------------------------
		bool  bGround;    // 地面に接しているか
		bool  bGroundOld; // 地面に接しているか(過去)
		short landingCounter;
		bool  bJump;      // ジャンプ
		float fJumpPower; // ジャンプ量
		float fGravity;   // 重力
		float fMaxHeight; // 最高Ｙ座標

		//-------------------------------
		//特定のギミック用
		//-------------------------------
		bool  bRide; // ロケットに乗っているかどうか
		bool  bGoal; // ゴールしたかどうか
		int   nTramJumpCounter; // トランポリンによって跳ね上がる時間
		float fTramTargetPosY;  // トランポリン用の目標位置
		bool  bTramJump;        // トランポリン用の特殊ジャンプ
		bool  bExtendDog;       // ヌイ用の接触フラグ
		bool  bLandPile;        // 杭に乗っているかどうか

		//-------------------------------
		//死亡アニメーション用
		//-------------------------------
		int  expandCounter; // 膨らみカウンター
		bool isDeath;       // 死亡フラグ
		int  deathCounter;  // 死亡カウンター
		int  deathCounter2; // 死亡カウンター2

		// どちらの世界に存在するか
		WORLD_SIDE side;

		//-------------------------------
		//キーコンフィグ
		//-------------------------------
		int Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; // キーボードのキー配置
		CInput::BUTTON JoyPad[(int)KEY_CONFIG::MAX];              // ジョイパッドのボタン配置
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
	void SetPos(const int nNum, D3DXVECTOR3 pos)
	{ m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = m_aInfo[nNum].posOld = pos; }

	// プレイヤー色情報設定
	// 指定したプレイヤーの色情報を引数に渡してください。
	void SetColor(const int nNum, Color color) { m_aInfo[nNum].color = color; }

	// プレイヤーにトランポリン用のジャンプを設定
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

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

	// 出現
	static void Pop(void) {}

	//SEラベル
	enum class SE_LABEL {
		JUMP = 0,// ジャンプSE
		LANDING, // 着地SE
		DOG_00,  // 押す
		DOG_01,  // 縮む
		DOG_02,  // 伸びる
		DOG_03,  // 震える
		SWAPING, // スワップSE
		SWAPEND, // スワップ終了
		MAX
	};

	//プレイヤー用SE再生
	static void PlaySE(SE_LABEL label);

	//パーティクルテクスチャ
	enum class PARTI_TEX {
		SWAP_MARK = 0,  // スワップマーク
		SWAP_PARTI00,   // スワップパーティクル00
		SWAP_PARTI01,   // スワップパーティクル01
		SWAP_PARTI02,   // スワップパーティクル02
		SWAP_GUIDE,     // スワップガイド
		CHARACTER,      // キャラクター
		DEATH_MARK,     // 死亡マーク
		DEATH_PARTI,    // 死亡パーティクル
		GOAL_EFFECT,    // ゴール・ロケット乗車時のエフェクト
		MAX
	};

	//パーティクル番号取得
	static int GetParticleIdx(PARTI_TEX tex) { return s_ParticleTex[(int)tex]; };

	struct Motion {
		short neutral;
		short walk;
		short jump;
		short fall;
		short landing;
	};

	//モーション情報取得
	static Motion GetMotion(void) { return s_motion; }

	//スワップ完了取得
	static bool GetSwapEnd(void) { return ms_bSwapEnd; }
	static bool GetSwapAnim(void) { return s_bSwapAnim; }

	// Guideカウンター取得
	static UShort GetGuideCounter(void) { return ms_guideCounter; }
	static void SetGuideCounter(UShort guideCounter) { ms_guideCounter = guideCounter; }

	// ズームアップカウンター
	static int GetZoomUpCounter(void) { return s_zoomUpCounter; }
private:
	//種類の略称を設定
	typedef CStageObject::TYPE OBJECT_TYPE;

	//****************************************************************************
	//スワップアニメーションセットリスト
	//順番：詳細【その演出にかかる時間変数】
	//----------------------------------------------------------------------------
	//プロローグ：各プレイヤーが光に包まれ、プレイヤーが見えなくなる【SWAP_START_INTERVAL】
	//   中間   ：光は中心に集まりつつ、スワップ先まで直線で移動する【SWAP_MOVE_INTERVAL】
	//エピローグ：スワップ先から光が飛び散りプレイヤーが顕現する	【SWAP_END_INTERVAL】
	//****************************************************************************
	//アニメーション構成
	enum class SWAP_ANIM {
		PROLOGUE = 0,	//プロローグ
		MIDDLE,			//中間
		EPILOGUE,		//エピローグ
		MAX
	};
	static const int SWAP_PROLOGUE_INTERVAL = 10; // スワップ開始～移動までの時間
	static const int SWAP_MIDDLE_INTERVAL   = 50; // 移動～目的地到着までの時間
	static const int SWAP_EPILOGUE_INTERVAL = 10; // 目的地到着～終了までの時間
	static const int NORMAL_SWAP_ALPHA = 100;     // 通常時のスワップマークのα値
	static const float GUIDE_WIDTH;      // ガイドの幅
	static const float GUIDE_HEIGHT;     // ガイドの高さ
	static const int ZOOM_UP_TIME = 120; // ズームアップにかかる時間
	static const int EXPAND_TIME = 60;   // 膨らみにかかる時間
	static const int DEATH_TIME = 60;    // 死亡時間
	static const int DEATH_TIME2 = 120;  // 死亡時間2
	static SWAP_ANIM s_AnimState;        // アニメーション構成
	static       int s_nSwapInterval;    // 残りスワップインターバル
	static       bool s_bSwapAnim;       // スワップアニメーション中かどうか
	static const int GOAL_INTERVAL = 120;// ゴール後の余韻
	static       int s_nGoalInterval;    // ゴール後の余韻カウンター
	static       int s_zoomUpCounter;    // ズームアップカウンター

	void Swap(void);
	void SwapAnimation(void);
	void SwapAnim_Prologue(Info& Player, const int nIdxPlayer); //プロローグ処理
	void SwapAnim_Middle(Info& Player, const int nIdxPlayer);	//中間処理
	void SwapAnim_Epilogue(Info& Player, const int nIdxPlayer); //エピローグ処理

	static const char *PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX];
	static int s_ParticleTex[(int)PARTI_TEX::MAX];

	static const float MOVE_SPEED;		// 移動量
	static const float MAX_MOVE_SPEED;	// 最大移動量

	static const float JUMP_POWER;		// 基本ジャンプ量
	static const float GRAVITY_POWER;	// 基本重力加速度

	static const int TRAMPOLINE_JUMP_COUNTER;

	static const int OBJ_TRAMPOLINE = 2;// オブジェクトの最大数
	static const int OBJ_EXTENDDOG = 3;	// オブジェクトの最大数

	static bool ms_bSwapEnd;
	static UShort ms_guideCounter;

	void InitKeyConfig(void);// 各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
	void InitInfo(void);
	void ActionControl(void);
	void Move(VECTOL vec, int cntPlayer);
	void CtrlPos(Info *pInfo, VECTOL vec);	// 範囲外の制御
	void Death(Info& Player, const OBJECT_TYPE type, const int *pColliRot);// 死んだ場所を引数に指定（死亡パーティクルなどを描画するのに使用する

	void CollisionToStageObject(void);

	// 各プレイヤーの当たり判定が終わった後の処理
	void CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, int *pColliRot);

	bool IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	bool IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);

	// 情報更新処理（更新処理の最後に位置情報などを設定する
	void UpdateInfo(void);
	void UpdateDeath(Info& info, const int& count);

	Info m_aInfo[NUM_PLAYER];	// 各プレイヤーの情報

	struct SE {
		short jump;      // ジャンプSE
		short landing;   // 着地SE
		short dog[4];    // 壁ギミック用SE
		short Swaping;   // スワップ中SE
		short SwapEnd;   // スワップ終了SE
		short expand;    // 膨らみ
		short explosion; // 破裂

		CSound *pSound;	// サウンドクラス保管用
	};
	static SE s_SE;		//サウンド用構造体

	static Motion s_motion;

	static CCollision *s_pColli;

	static bool s_bAimPlayer;
	static float s_fCorrWidth;
	static float s_fCorrHeight;
	static float s_fAimWorkSpeed;
};