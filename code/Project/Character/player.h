//================================================================================================
// 
// プレイヤー処理[player.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

#include "../Object/stage-object.h"

// 前方宣言
class CStageObject;
class CMoveBlock;
class CParts;
class CRocket;
class CRoadTripLaser;
class CExtenddog;
class CPile;
class CCollision;
class CGoalGate;

// プレイヤークラス
class CPlayer {
public:
	// 表裏どちらの世界に存在するか
	enum class WORLD_SIDE {
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
		int      idx;
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
		bool  bRide;         // ロケットに乗っているかどうか
		int   nRideInterval; //ロケットに乗り降りした時のインターバル
		bool  bGoal;         // ゴールしたかどうか
		CGoalGate *pGoalGate;
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
		int  DeathType;     // 死亡タイプ

		//-------------------------------
		//SWAP待ちの吹き出し用
		//-------------------------------
		int  swapWaitCounter;

		// どちらの世界に存在するか
		WORLD_SIDE side;

		//-------------------------------
		//キーコンフィグ
		//-------------------------------
		int Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; // キーボードのキー配置
		_RNC_Input::BUTTON JoyPad[(int)KEY_CONFIG::MAX];              // ジョイパッドのボタン配置
	};

	static CFloat SIZE_WIDTH;	// 横幅
	static CFloat SIZE_HEIGHT;	// 高さ

	static CInt SWAP_INTERVAL;	// スワップインターバル
	static CInt NUM_PLAYER = 2;// プレイヤーの数

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//----------------------------
	// プレイヤー生成
	// プレイヤーのポインタを返します
	//----------------------------
	static CPlayer* Create(void);

	// プレイヤー情報設定
	// 指定された番号のプレイヤー情報を設定します。
	void SetInfo(const Info info, CInt nNum);

	// プレイヤー情報設定
	// 各プレイヤーの位置情報などを引数に渡してください。
	// 構造体「Info」に関してはplayer.hのクラス宣言を確認してください。
	void SetInfo(Info p1, Info p2);

	// プレイヤー位置情報設定
	// 指定したプレイヤーの位置情報を引数に渡してください。
	void SetPos(CInt nNum, D3DXVECTOR3 pos) { m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = m_aInfo[nNum].posOld = pos; }

	// プレイヤー色情報設定
	// 指定したプレイヤーの色情報を引数に渡してください。
	void SetColor(CInt nNum, Color color) { m_aInfo[nNum].color = color; }

	// プレイヤーにトランポリン用のジャンプを設定
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

	// プレイヤー情報取得
	// 各引数にプレイヤー情報のアドレスを渡します
	void GetInfo(Info*& pP1, Info*& pP2) { pP1 = &m_aInfo[0]; pP2 = &m_aInfo[1]; }

	// プレイヤー情報取得
	// 指定された番号のプレイヤー情報のアドレスを返します
	static Info* GetInfo(int nNum) { return &m_aInfo[nNum]; }

	// プレイヤー情報取得
	// 指定された世界にいるプレイヤーの情報を返します
	static Info* GetInfo(WORLD_SIDE side);

	// スワップインターバルを設定
	// 既にインターバルがあれば設定しない
	static void SetSwapInterval(void) { s_nSwapInterval = s_nSwapInterval == 0 ? SWAP_INTERVAL : s_nSwapInterval; }

	// スワップインターバルを取得
	static int GetSwapInterval(void) { return s_nSwapInterval; }

	// スワップ待ちフラグを取得
	static bool GetIsSwapWait(void) { return m_aInfo[0].swapWaitCounter > 0 || m_aInfo[1].swapWaitCounter > 0; }

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
		DEATH_INK,      // 死亡インク
		DEATH_PARTI,    // 死亡パーティクル
		GOAL_EFFECT,    // ゴール・ロケット乗車時のエフェクト
		SMOKE_EFFECT,   // 煙のエフェクト
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
		short dance;
		short getup;
	};

	//モーション情報取得
	static Motion GetMotion(int idx) { return s_motion[idx]; }

	//スワップ完了取得
	static bool GetSwapEnd(void) { return ms_bSwapEnd; }
	static bool GetSwapAnim(void) { return s_bSwapAnim; }

	// 死亡取得
	static bool GetDeath(void) { return m_aInfo[0].isDeath || m_aInfo[1].isDeath; }

	// Guideカウンター
	static UShort GetGuideCounter(void) { return ms_guideCounter; }
	static void SetGuideCounter(UShort guideCounter) { ms_guideCounter = guideCounter; }

	// ズームアップカウンター
	static int GetZoomUpCounter(void) { return s_zoomUpCounter; }

	static bool IsKeyConfigTrigger(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	static bool IsKeyConfigTrigger(KEY_CONFIG KeyConfig);
	static bool IsKeyConfigPress(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	static bool IsKeyConfigPress(KEY_CONFIG KeyConfig);

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
	static CInt SWAP_PROLOGUE_INTERVAL = 10; // スワップ開始～移動までの時間
	static CInt SWAP_MIDDLE_INTERVAL   = 50; // 移動～目的地到着までの時間
	static CInt SWAP_EPILOGUE_INTERVAL = 10; // 目的地到着～終了までの時間
	static CInt NORMAL_SWAP_ALPHA = 100;     // 通常時のスワップマークのα値

	static CFloat GUIDE_WIDTH;      // ガイドの幅
	static CFloat GUIDE_HEIGHT;     // ガイドの高さ
	static CFloat MAX_GUIDE_SPEED;  // ガイドアニメーションの最大速度

	static CInt EXPAND_TIME = 60;   // 膨らみにかかる時間
	static CInt DEATH_TIME = 60;    // 死亡時間
	static CInt DEATH_TIME2 = 120;  // 死亡時間2

	static CInt SWAP_WAIT_BALLOON_TIME = 5;  // スワップ待ち吹き出し時間
	static SWAP_ANIM s_AnimState;        // アニメーション構成
	static       int s_nSwapInterval;    // 残りスワップインターバル
	static       bool s_bSwapAnim;       // スワップアニメーション中かどうか

	static CInt GOAL_INTERVAL = 120;// ゴール後の余韻
	static CInt POP_CLEARTIME = 60; // クリアタイム表示時間
	static       int s_nGoalInterval;    // ゴール後の余韻カウンター

	static CInt ZOOM_UP_TIME       = 120; // ズームアップにかかる時間
	static CInt ZOOM_UP_FIXED_TIME = 90;  // ズームアップの固定時間
	static       int s_zoomUpCounter;          // ズームアップカウンター
	static       int s_zoomUpFixedCounter;    // ズームアップ固定カウンター2

	void Swap(void);
	void SwapAnimation(void);
	void SwapAnim_Prologue(Info& Player, CInt nIdxPlayer); // プロローグ処理
	void SwapAnim_Middle(Info& Player, CInt nIdxPlayer);   // 中間処理
	void SwapAnim_Epilogue(Info& Player, CInt nIdxPlayer); // エピローグ処理
	void SwapGuide(Info& Player);                          // ガイド表示
	bool SwapGuideText(void);

	static const char *PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX];
	static int s_ParticleTex[(int)PARTI_TEX::MAX];

	static CFloat MOVE_SPEED;		// 移動量
	static CFloat MAX_MOVE_SPEED;	// 最大移動量

	static CFloat JUMP_POWER;		// 基本ジャンプ量
	static CFloat GRAVITY_POWER;	// 基本重力加速度

	static CInt TRAMPOLINE_JUMP_COUNTER;

	static CInt OBJ_TRAMPOLINE = 2;// オブジェクトの最大数
	static CInt OBJ_EXTENDDOG = 3;	// オブジェクトの最大数

	static bool ms_bSwapEnd;
	static UShort ms_guideCounter;

	void InitKeyConfig(void);// 各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
	void InitInfo(void);
	void ActionControl(void);
	void Move(VECTOL vec, int cntPlayer);
	void Death(Info& Player, const OBJECT_TYPE type);// 死んだ場所を引数に指定（死亡パーティクルなどを描画するのに使用する

	//ゴール後の演出
	void GoalDirector(void);

	void CollisionToStageObject(void);
	bool UniqueColliOpption(CStageObject *pObj, const OBJECT_TYPE type, Info &Player, Pos3D *pPos, Pos3D *pPosOld, float *pWidth, float *pHeight);

	// 各プレイヤーの当たり判定が終わった後の処理
	void CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, CInt *pColliRot, Info& info);

	// 情報更新処理（更新処理の最後に位置情報などを設定する
	void UpdateInfo(void);
	void UpdateDeath(Info& info, CInt& count);

	static Info m_aInfo[NUM_PLAYER];	// 各プレイヤーの情報

	struct SE {
		short jump;      // ジャンプSE
		short dog[4];    // 壁ギミック用SE
		short Swaping;   // スワップ中SE
		short SwapEnd;   // スワップ終了SE
		short expand;    // 膨らみ
		short explosion; // 破裂

		_RNC_Sound *pSound;	// サウンドクラス保管用
	};
	static SE s_SE;		//サウンド用構造体

	static Motion s_motion[2];

	static bool s_bAimPlayer;
	static int s_nAimNo;
	static Pos3D s_addPosV;
	static Pos3D s_addPosR;
	static float s_fCorrWidth;
	static float s_fCorrHeight;
	static float s_fAimWorkSpeed;

	int nCnt;
};