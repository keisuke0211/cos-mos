//================================================================================================
// 
// プレイヤー処理[player.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../main.h"
#include "player.h"
#include "../collision.h"
#include "../../_RNLib/Basis/Calculation/number.h"
#include "../Object/Gimmick/pile.h"

// スワップインターバル
const int	CPlayer::SWAP_INTERVAL = 90;	// スワップインターバル
int			CPlayer::s_nSwapInterval = 0;	// 残りスワップインターバル
bool		CPlayer::s_bSwapAnim = false;	//スワップアニメーション中かどうか
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//アニメーション構成

const float CPlayer::SIZE_WIDTH = 7.0f;	// 横幅
const float CPlayer::SIZE_HEIGHT = 8.0f;// 高さ

const float CPlayer::MOVE_SPEED = 0.3f;		// 移動量
const float CPlayer::MAX_MOVE_SPEED = 2.3f;	// 最大移動量

const float CPlayer::JUMP_POWER = 5.0f;		// 基本ジャンプ量
const float CPlayer::GRAVITY_POWER = -0.3f;	// 基本重力加速度

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// スワップマーク
	"data\\TEXTURE\\Effect\\eff_Star_000.png",	// スワップパーティクル
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// 死亡マーク
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// 死亡パーティクル
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// ゴール・ロケット乗車時のエフェクト
};
int			CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE CPlayer::s_SE = {};	//サウンド用構造体
CCollision *CPlayer::s_pColli = NULL;

//=======================================
// コンストラクタ
//=======================================
CPlayer::CPlayer()
{
	// 残りスワップインターバル
	s_nSwapInterval = 0;
	s_bSwapAnim = false;
	s_AnimState = SWAP_ANIM::PROLOGUE;

	s_SE.pSound = NULL;
	s_SE.jump = 0;
	s_SE.landing = 0;
	s_SE.Swap = 0;
	for each(short &dog in s_SE.dog)
	{
		dog = 0;
	}

	for each(Info &Player in m_aInfo)
	{
		Player.StartPos = INITD3DXVECTOR3;		// 開始位置
		Player.pos = INITD3DXVECTOR3;			// 位置
		Player.posOld = INITD3DXVECTOR3;		// 前回位置
		Player.rot = INITD3DXVECTOR3;			// 向き
		Player.move = INITD3DXVECTOR3;			// 移動量
		Player.color = INITCOLOR;				// 色
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA;	// スワップマークのα値
		Player.fSwapPosY = 0.0f;				// スワップ先のＹ座標
		Player.fSwapMoveY = 0.0f;				// スワップ移動時の速度
		Player.bGround = false;					// 地面に接しているか
		Player.bGroundOld = false;				// 地面に接しているか(過去)
		Player.bJump = false;					// ジャンプ
		Player.bRide = false;					// ロケットに乗っているかどうか
		Player.bGoal = false;					// ゴールしたかどうか
		Player.fJumpPower = 0.0f;				// ジャンプ量
		Player.fGravity = 0.0f;					// 重力
		Player.fMaxHeight = 0.0f;				// 最高Ｙ座標
		Player.nTramJumpCounter = 0;			// トランポリンによって跳ね上がる最高到達地点
		Player.fTramTargetPosY = 0.0f;			// トランポリン用の目標位置
		Player.bTramJump = false;				// トランポリン用の特殊ジャンプ
		//Player.TramColliRot = CCollision::ROT::NONE;	// トランポリン用の判定
		Player.bExtendDog = false;				// ヌイ用の接触フラグ
		Player.bLandPile = false;				// 杭に乗っているかどうか
		Player.nModelIdx = NONEDATA;			// モデル番号
		Player.side = WORLD_SIDE::FACE;			// どちらの世界に存在するか
	}

	s_pColli = NULL;	// 当たり判定クラス
}

//=======================================
// デストラクタ
//=======================================
CPlayer::~CPlayer()
{
	if (s_pColli != NULL)
	{
		s_pColli->OthColliDelete();
		delete s_pColli;
	}
	s_pColli = NULL;
}

//=======================================
// 生成処理
//=======================================
CPlayer *CPlayer::Create(void)
{
	// プレイヤーインスタンスの生成
	CPlayer *pPlayer = new CPlayer;

	// 初期化処理
	pPlayer->Init();

	// プレイヤーを返す
	return pPlayer;
}

//=====================================================================================================================
// 初期化処理
//=====================================================================================================================
HRESULT CPlayer::Init(void)
{
	// １Ｐ初期情報
	m_aInfo[0].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Human.x");

	// ２Ｐ初期情報
	m_aInfo[1].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Octopus.x");
	m_aInfo[1].rot = CStageObject::INVERSEVECTOR3;

	// キーコンフィグ初期化
	InitKeyConfig();

	//パーティクルテクスチャ読込
	CTexture *pTexture = &RNLib::Texture();
	for (int nCntTex = 0; nCntTex < (int)PARTI_TEX::MAX; nCntTex++)
	{
		s_ParticleTex[nCntTex] = pTexture->Load(PARTICLE_TEX_PATH[nCntTex]);
	}

	s_SE.pSound	= &RNLib::Sound();
	s_SE.jump	= s_SE.pSound->Load("data\\SOUND\\SE\\jamp_01.wav");
	s_SE.landing = s_SE.pSound->Load("data\\SOUND\\SE\\jamp.wav");
	s_SE.dog[0]	= s_SE.pSound->Load("data\\SOUND\\SE\\heron2.wav");		// 押す
	s_SE.dog[1]	= s_SE.pSound->Load("data\\SOUND\\SE\\shrink.wav");		// 縮む
	s_SE.dog[2]	= s_SE.pSound->Load("data\\SOUND\\SE\\extend.wav");		// 伸びる
	s_SE.dog[3]	= s_SE.pSound->Load("data\\SOUND\\SE\\vibration.wav");	// 震える
	s_SE.Swap	= s_SE.pSound->Load("data\\SOUND\\SE\\swap.wav");

	// 初期情報設定
	Death(NULL);

	// 初期値設定
	// ※ 来れないとステージ入る前に一瞬着地SEがなる
	m_aInfo[0].bJump = m_aInfo[1].bJump = false;

	if(s_pColli == NULL)
	s_pColli = new CCollision;

	// 初期化成功
	return S_OK;
}

//=======================================
// 各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
//=======================================
void CPlayer::InitKeyConfig(void)
{
	// どちらの世界でも共通のキー配置設定
	for (int nCntSide = 0; nCntSide < (int)WORLD_SIDE::MAX; nCntSide++)
	{
		// １Ｐのキーボード配置
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_A;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_D;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		// ２Ｐのキーボード配置
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_LEFTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_RIGHTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		// 世界で変わるキー配置設定
		switch ((WORLD_SIDE)nCntSide)
		{
		case WORLD_SIDE::FACE:
			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_W;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_UPARROW;

			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_S;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_DOWNARROW;
			break;

		case WORLD_SIDE::BEHIND:
			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_S;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_DOWNARROW;

			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_W;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_UPARROW;
			break;
		}

	}

	// ジョイパッドの設定は両者共通
	for each(Info &Player in m_aInfo)
	{
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = CInput::BUTTON::LEFT;  // 左移動
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = CInput::BUTTON::RIGHT; // 右移動
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = CInput::BUTTON::A;     // ジャンプ
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = CInput::BUTTON::X;     // スワップ
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = CInput::BUTTON::A;     // 決定
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = CInput::BUTTON::START; // ポーズ
	}
}

//=====================================================================================================================
// 終了処理
//=====================================================================================================================
void CPlayer::Uninit(void)
{

}

//=======================================
// 前回位置更新処理
//=======================================
void CPlayer::SetPosOld(void)
{
	// プレイヤーの前回位置更新
	for each (Info &Player in m_aInfo)
	{
		// ロケットに乗っていないなら　or ゴールしていないなら
		if (!Player.bRide || Player.bGoal)
		Player.posOld = Player.pos;
		Player.bGroundOld = Player.bGround;
	}
}

//=====================================================================================================================
// 更新処理
//=====================================================================================================================
void CPlayer::Update(void)
{
	//スワップアニメーション中
	if (s_bSwapAnim)
	{
		SwapAnimation();
		return;
	}

	// 前回位置更新
	SetPosOld();

	// 操作処理
	ActionControl();

	// 両者ともにゴールしてなかったら
	if (!m_aInfo[0].bGoal && !m_aInfo[1].bGoal)
	{
		// スワップ
		Swap();
	}
	else if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
	{
		const int planet = Manager::StgEd()->GetPlanetIdx();
		const int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

	// 当たり判定まとめ
	CollisionToStageObject();

	// 情報更新
	UpdateInfo();

	RNLib::Text2D().PutDebugLog(CreateText("FPS:%d", RNSystem::GetFPS()));
}

//----------------------------
// 情報更新処理
//----------------------------
void CPlayer::UpdateInfo(void)
{
	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;
		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		// 位置設定
		RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
			->SetOutLine(true)
			->SetCol(Player.color);

		// スワップ先のマークを描画する位置
		D3DXVECTOR3 MarkPos = Player.pos;
		MarkPos.z = -10.0f;
		MarkPos.y *= -1.0f;

		RNLib::Polygon3D().Put(PRIORITY_EFFECT, MarkPos, INITD3DXVECTOR3)
			->SetSize(20.0f, 20.0f)
			->SetBillboard(true)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_MARK))
			->SetCol(Color{ Player.color.r,Player.color.g,Player.color.b, (UShort)Player.nSwapAlpha });

		// 最高Ｙ座標更新
		switch (Player.side)
		{
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
		RNLib::Text2D().PutDebugLog(CreateText("%dP最高Y座標：%f    現在Y座標：%f", nCntPlayer + 1, Player.fMaxHeight, Player.pos.y));
	}
}

//----------------------------
// 操作処理
//----------------------------
void CPlayer::ActionControl(void)
{
	// プレイヤー番号
	int nIdxPlayer = -1;

	for each (Info &Player in m_aInfo)
	{
		// 次のプレイヤー番号へ
		nIdxPlayer++;

		// 相方がゴールしていなければ出る
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer +1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bRide = false;
			Player.bGoal = false;
			Player.move.x *= -2.0f;
		}

		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		// ジャンプ入力（空中じゃない）
		if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bGround = false;				// 地面から離れた
			Player.move.y = Player.fJumpPower;	// ジャンプ量代入
			Player.bJump = true;				// ジャンプした

												// SE再生
			s_SE.pSound->Play(s_SE.jump, CSound::CATEGORY::SE, false);
		}

		// 右に移動
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer))
			Player.move.x += MOVE_SPEED;

		// 左に移動
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer))
			Player.move.x -= MOVE_SPEED;

		// スワップ入力
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
		{
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI), Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
			Player.nSwapAlpha = 255;
		}
		//スワップ先のマークカラーを変更
		else Player.nSwapAlpha = NORMAL_SWAP_ALPHA;
	}
}

//#################################################
// スワップ処理
//#################################################
void CPlayer::Swap(void)
{
	// 両者ともにスワップボタンを押しているまたはどちらかがロケットに乗っている
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) || m_aInfo[0].bRide) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) || m_aInfo[1].bRide))
	{
		// スワップアニメーション設定
		s_AnimState = SWAP_ANIM::PROLOGUE;
		s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
		s_bSwapAnim = true;

		//ロケットに乗っていないときにサウンド再生
		if(CRocket::GetCounter() != NUM_PLAYER)	s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);

		for each (Info &Player in m_aInfo)
		{
			// ロケットに乗ってたらスキップ
			if (Player.bRide) continue;

			for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI), Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}

			// 位置・重力加速度・ジャンプ量・存在する世界を反転
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
			//Player.TramColliRot = CCollision::ROT::NONE;
			Player.bTramJump = false;
		}

		// 前回位置更新
		SetPosOld();
	}
}

//*************************************************
// スワップアニメーション
//*************************************************
void CPlayer::SwapAnimation(void)
{
	RNLib::Text2D().PutDebugLog(CreateText("インターバル：%d", s_nSwapInterval));
	switch (s_AnimState)
	{
		case CPlayer::SWAP_ANIM::PROLOGUE:RNLib::Text2D().PutDebugLog("プロローグ"); break;
		case CPlayer::SWAP_ANIM::MIDDLE:  RNLib::Text2D().PutDebugLog("中間"); break;
		case CPlayer::SWAP_ANIM::EPILOGUE:RNLib::Text2D().PutDebugLog("エピローグ"); break;
	}

	//if (!RNLib::Input().GetKeyTrigger(DIK_RETURN) &&
	//	!RNLib::Input().GetKeyPress(DIK_RSHIFT)) return;

	//インターバル減少
	s_nSwapInterval--;

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//プレイヤー情報参照（移動時だけパーティクルの位置を手前に出す
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer);break;//プロローグ
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;//中間
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer);break;//エピローグ
		}

		for (int nCntPar = 0; nCntPar < 1; nCntPar++)
		{
			Color setCol;
			if (nCntPlayer == 0){
				setCol = Color{ (UShort)(215 + rand() % 40),(UShort)(135 + rand() % 40),(UShort)(39 + rand() % 40),255 };
			}
			else{
				setCol = Color{ (UShort)(45 + rand() % 40),(UShort)(203 + rand() % 40),(UShort)(190 + rand() % 40),255 };
			}
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_MARK), Player.pos, Vector3D(16.0f, 16.0f, 0.0f), setCol);
		}
	}
}

//*************************************************
//プロローグ処理
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, const int nIdxPlayer)
{
	// 位置設定
	RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
		->SetOutLine(true)
		->SetCol(Player.color);

	//次のインターバルへ
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_MIDDLE_INTERVAL;
	s_AnimState = SWAP_ANIM::MIDDLE;
}

//*************************************************
//中間処理
//*************************************************
void CPlayer::SwapAnim_Middle(Info& Player, const int nIdxPlayer)
{
	//スワップ先へ移動
	Player.pos.y += Player.fSwapMoveY;

	//次のインターバルへ
	if (s_nSwapInterval > 0) return;
	Player.pos.y = Player.fSwapPosY;
	Player.move.y *= -1.0f;
	Player.fGravity *= -1.0f;
	Player.fJumpPower *= -1.0f;
	Player.side = (WORLD_SIDE)(((int)Player.side + 1) % (int)WORLD_SIDE::MAX);
	if (Player.side == WORLD_SIDE::FACE)
		Player.rot.z = 0.0f;
	else Player.rot.z = D3DX_PI;

	//最後のプレイヤーのときにアニメーション状態を遷移
	if (nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_EPILOGUE_INTERVAL;
	s_AnimState = SWAP_ANIM::EPILOGUE;
}

//*************************************************
//エピローグ処理
//*************************************************
void CPlayer::SwapAnim_Epilogue(Info& Player, const int nIdxPlayer)
{
	// 位置設定
	RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
		->SetOutLine(true)
		->SetCol(Player.color);

	//最後のプレイヤーのときにスワップアニメーション終了
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_bSwapAnim = false;
}

//----------------------------
// 死亡処理
//----------------------------
void CPlayer::Death(D3DXVECTOR3 *pDeathPos)
{
	if (pDeathPos != NULL)
	{
		Manager::EffectMgr()->EffectCreate(GetParticleIdx(PARTI_TEX::DEATH_MARK), *pDeathPos, INIT_EFFECT_SCALE, Color{ 255,0,255,255 });

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::DEATH_PARTI), *pDeathPos, INIT_EFFECT_SCALE * 0.5f, Color{ 255,0,0,255 });
		}
	}

	// １Ｐ用初期情報
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;

	// ２Ｐ用初期情報
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;

	// 両者共通初期情報
	for each (Info &Player in m_aInfo)
	{
		Player.posOld = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
	}

	//搭乗しているプレイヤーの数リセット
	CRocket::ResetCounter();
}

//----------------------------
// 移動処理
//----------------------------
void CPlayer::Move(VECTOL vec)
{
	// プレイヤーの位置更新
	for each (Info &Player in m_aInfo)
	{
		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		// 過去の位置を設定
		Player.posOld = Player.pos;

		// 移動量反映
		switch (vec)
		{
		case VECTOL::X:
			// 慣性処理
			Player.move.x += (0.0f - Player.move.x) * 0.12f;

			// Ⅹの移動量を修正
			FloatControl(&Player.move.x, MAX_MOVE_SPEED, -MAX_MOVE_SPEED);

			// 位置更新
			Player.pos.x += Player.move.x;
			break;

			// 重力処理
		case VECTOL::Y:

			// トランポリンによる特殊ジャンプ中
			if (Player.bTramJump)
			{// カウンターを減らして、０になったら特殊ジャンプ終了
				
				float diff = Player.fTramTargetPosY - Player.pos.y;

				if (--Player.nTramJumpCounter <= 0)
				{
					Player.bTramJump = false;
					Player.move.y = diff;
				}
				else
				{
					Player.move.y = diff / Player.nTramJumpCounter;
				}
			}
			// 通常時なら、重力処理でＹの移動量を計算
			else Player.move.y += Player.fGravity;

			// 位置更新
			Player.pos.y += Player.move.y;
			break;
		}

		//位置制御
		CtrlPos(&Player, vec);
	}
}

//----------------------------
// 制御処理
//----------------------------
void CPlayer::CtrlPos(Info *pInfo, VECTOL vec)
{
	int RowMax = Manager::StgEd()->GetRowMax() - 1;
	int LineMax = Manager::StgEd()->GetLineMax() - 1;

	D3DXVECTOR3 MinPos = Manager::StgEd()->GetPos(0, 0);
	D3DXVECTOR3 MaxPos = Manager::StgEd()->GetPos(RowMax, LineMax);

	// 1マスずらす
	MinPos.y = -MinPos.y - CStageObject::SIZE_OF_1_SQUARE * 0.5f;
	MaxPos.y = -MaxPos.y + CStageObject::SIZE_OF_1_SQUARE * 0.5f;

	// プレイヤーの位置更新
	switch (vec)
	{
			//Ｘ座標
		case CPlayer::VECTOL::X:
			if		(pInfo->pos.x <= MinPos.x)	pInfo->pos.x = MinPos.x;
			else if (pInfo->pos.x > MaxPos.x)	pInfo->pos.x = MaxPos.x;
			break;

			//Ｙ座標
		case CPlayer::VECTOL::Y:
			if		(pInfo->pos.y <= MinPos.y)	pInfo->pos.y = MinPos.y;
			else if (pInfo->pos.y > MaxPos.y)	pInfo->pos.y = MaxPos.y;
			break;
	}
}

//========================================
// 当たり判定処理
// Author:HIRASAWA SHION
// Arrange:RIKU NISHIMURA  OTONO KEISUKE
//========================================
void CPlayer::CollisionToStageObject(void)
{
	// 一旦両プレイヤーともにジャンプ不可
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++) {

		// 衝突ベクトルに変換
		const VECTOL vec = (VECTOL)nCntVec;

		// 移動・位置制御処理
		Move(vec);

		// オブジェクト1つ1つを見ていく
		CObject* obj = NULL;
		while (Manager::StageObjectMgr()->ListLoop(&obj)) {

			// 取得したオブジェクトをキャスト
			CStageObject* stageObj = (CStageObject*)obj;

			// 種類取得
			const CStageObject::TYPE type = stageObj->GetType();

			// オブジェクトの当たり判定情報を設定
			CCollision::ColliInfo m_colliInfo;
			m_colliInfo.pos     = stageObj->GetPos();
			m_colliInfo.posOld  = m_colliInfo.pos;
			m_colliInfo.fWidth  = stageObj->GetWidth() * 0.5f;
			m_colliInfo.fHeight = stageObj->GetHeight() * 0.5f;
			m_colliInfo.minPos  = D3DXVECTOR3(m_colliInfo.pos.x - m_colliInfo.fWidth, m_colliInfo.pos.y - m_colliInfo.fHeight, 0.0f);
			m_colliInfo.maxPos  = D3DXVECTOR3(m_colliInfo.pos.x + m_colliInfo.fWidth, m_colliInfo.pos.y + m_colliInfo.fHeight, 0.0f);

			// プレイヤー毎に処理していく
			for each(Info& Player in m_aInfo) {
				// ロケットに乗ってるかゴールしていたら折り返す
				if (Player.bRide || Player.bGoal)
					continue;

				// プレイヤーの近くにオブジェクトがあるか判定
				// ※特定オブジェクトを除く
				if (type != CStageObject::TYPE::TRAMPOLINE && 
					type != CStageObject::TYPE::LASER && 
					type != CStageObject::TYPE::EXTEND_DOG) {

					if (D3DXVec3Length(&(m_colliInfo.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(m_colliInfo.fWidth + SIZE_WIDTH, m_colliInfo.fHeight + SIZE_HEIGHT)))
						continue;
				}

				//自分の情報
				CCollision::SelfInfo Self;
				Self.pos = Player.pos;
				Self.posOld = Player.posOld;
				Self.move = Player.move;
				Self.fWidth = SIZE_WIDTH;
				Self.fHeight = SIZE_HEIGHT;

				// 他パーツの当たり判定フラグ
				//bool bOtherColl = false;

				// 移動するオブジェクトは、
				// 当たり判定位置に前回位置を設定する
				switch (type) {
					// 移動床
				case CStageObject::TYPE::BLOCK: {
					CBlock* pBlock = (CBlock*)stageObj;
					if (!pBlock->GetCollision())
						continue;
				}break;

					// 移動床
				case CStageObject::TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock*)stageObj;
					m_colliInfo.posOld = pBlock->GetPosOld();
				}
				break;

					// 隕石
				case CStageObject::TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					m_colliInfo.posOld = pMeteor->GetPosOld();
				}
				break;

					// レーザー
				case CStageObject::TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					//m_pOthColli = new CollInfo;
					//
					//m_pOthColli->pos = pLaser->GetLaserPos();
					//m_pOthColli->posOld = m_pOthColli->pos;
					//m_pOthColli->fWidth = pLaser->GetLaserSize().x * 0.5f;
					//m_pOthColli->fHeight = pLaser->GetLaserSize().y * 0.5f;
					//
					//// オブジェクトの最小・最大位置
					//m_pOthColli->minPos = D3DXVECTOR3(m_pOthColli->pos.x - m_pOthColli->fWidth, m_pOthColli->pos.y - m_pOthColli->fHeight, 0.0f);
					//m_pOthColli->maxPos = D3DXVECTOR3(m_pOthColli->pos.x + m_pOthColli->fWidth, m_pOthColli->pos.y + m_pOthColli->fHeight, 0.0f);
					//
					//// 当たった方向を格納
					//m_pOthColli->ColliRot = IsBoxCollider(Player.pos, Player.posOld, SIZE_WIDTH, SIZE_HEIGHT, m_pOthColli->pos, m_pOthColli->posOld, m_pOthColli->fWidth, m_pOthColli->fHeight, vec);

					//if (m_pOthColli->ColliRot != CCollision::ROT::NONE) {
					//	bOtherColl = true;
					//}
				}
				break;

					// ヌイ
				case CStageObject::TYPE::EXTEND_DOG:{
					//CExtenddog *pDog = (CExtenddog *)stageObj;

					//m_pOthColli = new CollInfo[OBJ_EXTENDDOG];

					//for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
					//	switch (nCnt){
					//	case 0:
					//		m_pOthColli[nCnt].pos = pDog->GetHeadPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetHeadPosOid();
					//		break;
					//	case 1:
					//		m_pOthColli[nCnt].pos = pDog->GetBodyPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetBodyPos();
					//		break;
					//	case 2:
					//		m_pOthColli[nCnt].pos = pDog->GetHipPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetHipPos();
					//		break;
					//	}

					//	m_pOthColli[nCnt].fWidth = pDog->GetWidth() * 0.5f;
					//	m_pOthColli[nCnt].fHeight = pDog->GetHeight() * 0.5f;

					//	if (D3DXVec3Length(&(m_pOthColli[nCnt].pos - Player.pos)) >
					//		D3DXVec2Length(&D3DXVECTOR2(m_pOthColli[nCnt].fWidth + SIZE_WIDTH, m_pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

					//	// オブジェクトの最小・最大位置
					//	m_pOthColli[nCnt].minPos = D3DXVECTOR3(m_pOthColli[nCnt].pos.x - m_pOthColli[nCnt].fWidth, m_pOthColli[nCnt].pos.y - m_pOthColli[nCnt].fHeight, 0.0f);
					//	m_pOthColli[nCnt].maxPos = D3DXVECTOR3(m_pOthColli[nCnt].pos.x + m_pOthColli[nCnt].fWidth, m_pOthColli[nCnt].pos.y + m_pOthColli[nCnt].fHeight, 0.0f);

					//	// 当たった方向を格納
					//	m_pOthColli[nCnt].ColliRot = s_pColli->IsBoxCollider(*pSelf, m_colliInfo, vec);

					//	//if (m_pOthColli[nCnt].ColliRot != CCollision::ROT::NONE){
					//	//	bOtherColl = true;
					//	//}
					//}

					//CExtenddog::STATE state = pDog->GetState();
					//if ((m_pOthColli[2].ColliRot != CCollision::ROT::UNDER) ||
					//	(m_pOthColli[2].ColliRot != CCollision::ROT::OVER && state == CExtenddog::STATE::DOWN_LAND)) {
					//	Player.bExtendDog = false;
					//}
				}
				break;

					//杭
				case CStageObject::TYPE::PILE:
				{
					CPile *pPile = (CPile *)stageObj;

					m_colliInfo.pos = pPile->GetPos();
					m_colliInfo.posOld = pPile->GetPosOld();
				}
				break;
				}

				// 当たった方向を格納
				m_colliInfo.Rot = s_pColli->IsBoxCollider(Self, m_colliInfo, vec);

				// 当たっていない
				if (m_colliInfo.Rot == CCollision::ROT::NONE)
				{
					//杭に当たっていないなら乗っていない
					if (type == CStageObject::TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				// 種類ごとに関数分け
				switch (type)
				{
				case CStageObject::TYPE::BLOCK:		s_pColli->Block(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::FILLBLOCK:	s_pColli->FillBlock(&Self, m_colliInfo.Rot, &Player.side); break;
				case CStageObject::TYPE::TRAMPOLINE:s_pColli->Trampoline(&Self, &m_colliInfo, (CTrampoline *)stageObj, &Player.side);	break;
				case CStageObject::TYPE::SPIKE:		s_pColli->Spike(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::MOVE_BLOCK:s_pColli->MoveBlock(&Self, (CMoveBlock *)stageObj, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::METEOR:	s_pColli->Meteor(&Self, &m_colliInfo, &Player.side); break;
				case CStageObject::TYPE::LASER:		s_pColli->Laser(&Self, (CRoadTripLaser *)stageObj,&m_colliInfo, NULL, &Player.side);	break;
				case CStageObject::TYPE::EXTEND_DOG:s_pColli->Dog(&Self, (CExtenddog *)stageObj, &m_colliInfo, NULL, &Player.side); break;
				case CStageObject::TYPE::GOALGATE:	s_pColli->GoalGate(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::PARTS:		s_pColli->Parts(&Self, (CParts *)stageObj, &Player.side); break;
				case CStageObject::TYPE::ROCKET:	s_pColli->Rocket(&Self, (CRocket *)stageObj, &Player.side); break;
				case CStageObject::TYPE::PILE:		s_pColli->Pile(&Self, &m_colliInfo, (CPile *)stageObj, &Player.side); break;
				}

				//情報代入
				Player.pos = Self.pos;
				Player.move = Self.move;

				// 当たれば即死のオブジェクトに当たっている
				if (type == CStageObject::TYPE::SPIKE || type == CStageObject::TYPE::METEOR || type == CStageObject::TYPE::LASER)
					break;
			}

			//当たり判定の事後処理
			CollisionAfter(stageObj, type);
		}
	}
}

//----------------------------
// 各プレイヤーの当たり判定が終わった後の処理
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type)
{
	// 種類ごとに関数分け
	switch (type)
	{
		// ヌイの状態設定
		case CStageObject::TYPE::EXTEND_DOG:
		{
			if (!m_aInfo[0].bExtendDog && !m_aInfo[1].bExtendDog)
			{
				CExtenddog *pDog = (CExtenddog *)pStageObj;
				pDog->SetState(CExtenddog::STATE::RETURN);
			}

			break;
		}

		//杭に乗っているプレイヤー
		case CStageObject::TYPE::PILE:
		{
			CPile *pPile = (CPile *)pStageObj;
			const float CaveInPos = pPile->GetPosCaveIn().y;
			const float Height = pPile->GetHeight() * 0.5f;

			for each (Info &Player in m_aInfo)
			{
				if(!Player.bLandPile) continue;

				switch (Player.side)
				{
					case WORLD_SIDE::FACE:	Player.pos.y = CaveInPos + Height + SIZE_HEIGHT;	break;
					case WORLD_SIDE::BEHIND:Player.pos.y = CaveInPos - Height - SIZE_HEIGHT;	break;
				}
				//次の杭で判定しないよう初期化
				Player.bLandPile = false;
			}
			break;
		}
	}
}

//----------------------------
// 上からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	// 位置・移動量修正
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// 下からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	// 位置・移動量修正
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// 左からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	// 位置・移動量修正
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// 右からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	// 位置・移動量修正
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// ブロックの当たり判定処理
//----------------------------
//void CPlayer::CollisionBlock(Info *pInfo, CollInfo *pColli)
//{
//	// 当たった方向ごとに処理を切り替え
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// 上に当たった
//		//*********************************
//	case CCollision::ROT::OVER:
//		// 位置・移動量修正
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 表の世界のプレイヤー
//		if (pInfo->side == WORLD_SIDE::FACE) {
//			if (pInfo->bJump == true)
//			{// 着地した
//				// SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//		}
//		break;
//
//		//*********************************
//		// 下に当たった
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// 位置・移動量修正
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 裏の世界のプレイヤーならジャンプ可能
//		if (pInfo->side == WORLD_SIDE::BEHIND) {
//			if (pInfo->bJump == true)
//			{// 着地した
//				// SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//		}
//		break;
//
//		//*********************************
//		// 左に当たった
//		//*********************************
//	case CCollision::ROT::LEFT:
//		// 位置・移動量修正
//		FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//		break;
//
//		//*********************************
//		// 右に当たった
//		//*********************************
//	case CCollision::ROT::RIGHT:
//		// 位置・移動量修正
//		FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//		break;
//
//		//*********************************
//		// 埋まった
//		//*********************************
//	case CCollision::ROT::UNKNOWN:
//		Death(&pInfo->pos);
//		break;
//	}
//}

//----------------------------
// 穴埋めブロックの当たり判定処理
//----------------------------
//void CPlayer::CollisionFillBlock(Info *pInfo,CCollision::ROT ColliRot)
//{
//	Death(&pInfo->pos);
//}

//----------------------------
// トランポリンの当たり判定処理
//----------------------------
//void CPlayer::CollisionTrampoline(Info *pInfo, CollInfo *pColli, CTrampoline *pTrampoline)
//{
//	// 当たった方向ごとに処理を切り替え
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// 上に当たった
//		//*********************************
//	case CCollision::ROT::OVER:
//
//		// 位置・移動量修正
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 表の世界のプレイヤー
//		if (pInfo->side == WORLD_SIDE::FACE)
//		{
//			if (pInfo->posOld.y > pInfo->pos.y)
//			{// 着地した
//			 // SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				pTrampoline->SetState(CTrampoline::STATE::UP_LAND);
//				pTrampoline->SetSpringForce(pInfo->fMaxHeight);
//				pTrampoline->SetCount(CTrampoline::MAX_COUNT);
//			}
//
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//
//			if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND) {
//				SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
//			}
//		}
//		break;
//
//		//*********************************
//		// 下に当たった
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// 位置・移動量修正
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 裏の世界のプレイヤーならジャンプ可能
//		if (pInfo->side == WORLD_SIDE::BEHIND)
//		{
//			if (pInfo->posOld.y < pInfo->pos.y)
//			{// 着地した
//				// SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				pTrampoline->SetState(CTrampoline::STATE::DOWN_LAND);
//				pTrampoline->SetSpringForce(pInfo->fMaxHeight);
//				pTrampoline->SetCount(CTrampoline::MAX_COUNT);
//			}
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//
//			if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND) {
//				SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
//			}
//		}
//		break;
//	}
//}

//----------------------------
// トゲの当たり判定処理
//----------------------------
//void CPlayer::CollisionSpike(Info *pInfo, CollInfo *pColli)
//{
//	switch (pColli->ColliRot)
//	{
//		//上下どちらかに当たれば死亡
//		case CCollision::ROT::OVER:
//		case CCollision::ROT::UNDER:
//			// 死亡処理
//			Death(&pInfo->pos);
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//	}
//}

//----------------------------
// 移動床の当たり判定処理
//----------------------------
//void CPlayer::CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, CollInfo *pColli)
//{
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// 上に当たった
//		//*********************************
//	case CCollision::ROT::OVER:
//		// 位置・移動量修正
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 表の世界のプレイヤーの場合
//		if (pInfo->side == WORLD_SIDE::FACE)
//		{
//			pInfo->move = pMoveBlock->GetMove();
//			if (pInfo->bJump == true)
//			{// 着地した
//			 // SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//		}
//		break;
//
//		//*********************************
//		// 下に当たった
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// 位置・移動量修正
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// 裏の世界のプレイヤーならジャンプ可能
//		if (pInfo->side == WORLD_SIDE::BEHIND)
//		{
//			pInfo->pos = pMoveBlock->GetMove();
//			if (pInfo->bJump == true)
//			{// 着地した
//			 // SE再生
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// 地面に接している
//			pInfo->bJump = false;	// ジャンプ可能
//			pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//		}
//		break;
//
//		//*********************************
//		// 左に当たった
//		//*********************************
//	case CCollision::ROT::LEFT:
//		// 位置・移動量修正
//		FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, pColli->fWidth);
//		break;
//
//		//*********************************
//		// 右に当たった
//		//*********************************
//	case CCollision::ROT::RIGHT:
//		// 位置・移動量修正
//		FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, pColli->fWidth);
//		break;
//
//	case CCollision::ROT::UNKNOWN:
//		// 移動床 -> プレイヤーへの当たり判定処理を実行
//		const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
//		const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
//		const float fWidth = pMoveBlock->GetWidth() * 0.5f;
//		const float fHeight = pMoveBlock->GetHeight() * 0.5f;
//
//		// 移動床からの当たり判定
//		for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++)
//		{
//			// プレイヤーのどの方向に当たっているか
//			pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOld, pColli->fWidth, pColli->fHeight, (VECTOL)nCntVec);
//
//			// それでも当たらないなら、スキップ
//			if (pColli->ColliRot == CCollision::ROT::NONE || pColli->ColliRot == CCollision::ROT::UNKNOWN) continue;
//
//			// 当たった方向（上下・左右）を反転する
//			{
//				// 当たった方向をint型に変換
//				const int nRot = (int)pColli->ColliRot;
//
//				pColli->ColliRot = (CCollision::ROT)(nRot - 1 + 2 * (nRot % 2));
//			}
//
//			// もう一度当たり判定
//			CollisionMoveBlock(pInfo, pMoveBlock, pColli);
//		}
//		break;
//	}
//}

//----------------------------
// 隕石の当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionMeteor(Info *pInfo, CollInfo *pColli)
//{
//	// 死亡処理
//	Death(&pInfo->pos);
//}

//----------------------------
// レーザーの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, CollInfo *pColli, CollInfo *pOthColli)
//{
//	// 本体
//	{
//		// 当たった方向ごとに処理を切り替え
//		switch (pColli->ColliRot)
//		{
//			//*********************************
//			// 上に当たった
//			//*********************************
//		case CCollision::ROT::OVER:
//			// 位置・移動量修正
//			FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤーの場合
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				pInfo->move = pRoadTripLaser->GetMove();
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 位置・移動量修正
//			FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤーの場合
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				pInfo->move = pRoadTripLaser->GetMove();
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN: 
//			// レーザー -> プレイヤーへの当たり判定処理を実行
//			const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
//			const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
//			const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
//			const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;
//
//			// 移動床からの当たり判定
//			for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++)
//			{
//				// プレイヤーのどの方向に当たっているか
//				pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOld, SIZE_WIDTH, SIZE_HEIGHT, (VECTOL)nCntVec);
//
//				// それでも当たらないなら、スキップ
//				if (pColli->ColliRot == CCollision::ROT::NONE || pColli->ColliRot == CCollision::ROT::UNKNOWN) continue;
//
//				// 当たった方向（上下・左右）を反転する
//				{
//					// 当たった方向をint型に変換
//					const int nRot = (int)pColli->ColliRot;
//
//					pColli->ColliRot = (CCollision::ROT)(nRot - 1 + 2 * (nRot % 2));
//				}
//
//				// もう一度当たり判定
//				CollisionLaser(pInfo, pRoadTripLaser, pColli, pOthColli);
//			}
//			break;
//		}
//	}
//
//	// レーザー
//	{
//		// 死亡処理
//		Death(&pInfo->pos);
//	}
//}

//----------------------------
// ヌイの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionDog(Info *pInfo, CExtenddog *pExtenddog, CollInfo *pColli, CollInfo *pOthColli)
//{
//	// 当たった方向ごとに処理を切り替え
//
//	CExtenddog::STATE State = pExtenddog->GetState();
//
//	// ハウス
//	{
//		switch (pColli->ColliRot)
//		{
//			//*********************************
//			// 上に当たった
//			//*********************************
//		case CCollision::ROT::OVER:
//			// 位置・移動量修正
//			FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤー
//			if (pInfo->side == WORLD_SIDE::FACE) {
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 位置・移動量修正
//			FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 裏の世界のプレイヤーならジャンプ可能
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//
//	// 頭
//	{
//		CExtenddog::STATE state = pExtenddog->GetState();
//		switch (pOthColli[0].ColliRot)
//		{
//			//*********************************
//			// 上に当たった
//			//*********************************
//		case CCollision::ROT::OVER:
//			// 位置・移動量修正
//			FixPos_OVER(&pInfo->pos.y, pOthColli[0].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤーの場合
//			if (pInfo->side == WORLD_SIDE::FACE){
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[0].maxPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 位置・移動量修正
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[0].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤーの場合
//			if (pInfo->side == WORLD_SIDE::FACE){
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[0].maxPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[0].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//
//			if (state == CExtenddog::STATE::NONE) {
//				// 位置・移動量修正
//				FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			}
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN: break;
//
//		}
//	}
//
//	// 体
//	{
//		switch (pOthColli[1].ColliRot)
//		{
//			//*********************************
//			// 上に当たった
//			//*********************************
//		case CCollision::ROT::OVER:
//			// 位置・移動量修正
//			FixPos_OVER(&pInfo->pos.y, pOthColli[1].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤー
//			if (pInfo->side == WORLD_SIDE::FACE) {
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[1].maxPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 位置・移動量修正
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[1].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 裏の世界のプレイヤーならジャンプ可能
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[1].minPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[1].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//
//	// 尻
//	{
//		switch (pOthColli[2].ColliRot)
//		{
//			//*********************************
//			// 上に当たった
//			//*********************************
//		case CCollision::ROT::OVER:
//			// 位置・移動量修正
//			FixPos_OVER(&pInfo->pos.y, pOthColli[2].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 表の世界のプレイヤー
//			if (pInfo->side == WORLD_SIDE::FACE) {
//
//				if (pInfo->bJump == true)
//					s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);
//
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[2].maxPos.y;// 最高Ｙ座標設定
//			}
//
//			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
//			pInfo->bExtendDog = true;
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 位置・移動量修正
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[2].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// 裏の世界のプレイヤーならジャンプ可能
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//
//				if (pInfo->bJump == true)
//					s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);
//
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pOthColli[2].minPos.y;// 最高Ｙ座標設定
//			}
//
//			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
//			pInfo->bExtendDog = true;
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[2].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pOthColli[2].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//}

//----------------------------
// ゴールゲートの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionGoalGate(Info *pInfo, CollInfo *pColli)
//{
//	if (!pInfo->bGoal)
//	{
//		pInfo->bGoal = true;
//
//		int ParTex = RNLib::Texture().Load(GetParticleIdx(PARTI_TEX::GOAL_EFFECT));
//
//		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
//		{
//			Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
//		}
//	}
//}

//----------------------------
// パーツの当たり判定処理
//----------------------------
//void CPlayer::CollisionParts(Info *pInfo, CParts *pParts)
//{
//	if (!pParts->GetDisp()) return;
//
//	// 取得したので描画OFF
//	pParts->DispSwitch(false);
//
//	// 取得数増加
//	s_nNumGetParts++;
//
//	// 取得した数が全体数と同じなら、ロケット乗車可能
//	if (s_nNumGetParts == CParts::GetNumAll()) s_bRideRocket = true;
//}

//----------------------------
// ロケットの当たり判定処理
//----------------------------
//void CPlayer::CollisionRocket(Info *pInfo, CRocket *pRocket)
//{
//	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
//
//	if (!s_bRideRocket) return;
//
//	// ロケットに搭乗
//	pInfo->bRide = true;
//	pRocket->Ride();
//
//	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
//	{
//		Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
//	}
//}

//----------------------------
// 杭の当たり判定処理
//----------------------------
//void CPlayer::CollisionPile(Info *pInfo, CollInfo *pColli, CPile *pPile)
//{
//	// 当たった方向ごとに処理を切り替え
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// 上に当たった
//		//*********************************
//		case CCollision::ROT::OVER:
//			// 表の世界のプレイヤー
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//
//				//ある程度の高さから落下してきた
//				if (pInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
//				{
//					pPile->CaveInTrunkHeight(pColli->maxPos.y - pInfo->pos.y - SIZE_HEIGHT);
//				}
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// 乗った
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 下に当たった
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// 裏の世界のプレイヤー
//			if (pInfo->side == WORLD_SIDE::BEHIND)
//			{
//				if (pInfo->bJump == true)
//				{// 着地した
//				 // SE再生
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//
//				//ある程度の高さから落下してきた
//				if (pInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
//				{
//					pPile->CaveInTrunkHeight(pColli->minPos.y - pInfo->pos.y + SIZE_HEIGHT);
//				}
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// 乗った
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//				pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//			}
//			break;
//
//			//*********************************
//			// 左に当たった
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// 位置・移動量修正
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 右に当たった
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// 位置・移動量修正
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// 埋まった
//			//*********************************
//		case CCollision::ROT::UNKNOWN:
//		{
//			if (s_nSwapInterval != 0) Death(&pInfo->pos);
//			else
//			{
//				if (pInfo->side == WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
//				else 								pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// 乗った
//				pInfo->bGround = true;	// 地面に接している
//				pInfo->bJump = false;	// ジャンプ可能
//			}
//		}break;
//	}
//}

//----------------------------
// プレイヤー情報設定
// 指定された番号のプレイヤー情報を設定します。
//----------------------------
void CPlayer::SetInfo(const Info info, const int nNum)
{
	if (0 <= nNum && nNum < NUM_PLAYER)
	{
		// 各プレイヤー情報設定
		m_aInfo[nNum] = info;
		m_aInfo[nNum].StartPos = info.pos;
	}
}

//----------------------------
// プレイヤー情報設定
//----------------------------
void CPlayer::SetInfo(Info p1, Info p2)
{
	// 各プレイヤー情報設定
	m_aInfo[0] = p1;	m_aInfo[0].StartPos = p1.pos;
	m_aInfo[1] = p2;	m_aInfo[1].StartPos = p2.pos;
}

//----------------------------
// プレイヤーにトランポリン用のジャンプを設定
//----------------------------
void CPlayer::SetTrampolineJump(Info*& pInfo, float fMaxHeight)
{
	SetSwapInterval();

	// ジャンプ量を継承
	const float diff = -fMaxHeight - pInfo->pos.y;
	pInfo->move.y = diff / TRAMPOLINE_JUMP_COUNTER;

	pInfo->fTramTargetPosY = -fMaxHeight;
	pInfo->nTramJumpCounter = TRAMPOLINE_JUMP_COUNTER;
	pInfo->bTramJump = true;
	pInfo->bGround = false;
	pInfo->bJump = true;
}

//----------------------------
// プレイヤー情報取得
// 指定された世界にいるプレイヤーの情報を返します
//----------------------------
CPlayer::Info *CPlayer::GetInfo(WORLD_SIDE side)
{
	// １Ｐのいる世界と合致したら１Ｐ情報を返す
	if (m_aInfo[0].side == side) return &m_aInfo[0];

	// 違うなら２Ｐ情報を返す
	else return &m_aInfo[1];
}

//----------------------------
// プレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// プレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//プレイヤー用SE再生
//----------------------------
void CPlayer::PlaySE(SE_LABEL label)
{
	switch (label)
	{
		case CPlayer::SE_LABEL::JUMP:	s_SE.pSound->Play(s_SE.jump, CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::LANDING:s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_00:	s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_01:	s_SE.pSound->Play(s_SE.dog[1], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_02:	s_SE.pSound->Play(s_SE.dog[2], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_03:	s_SE.pSound->Play(s_SE.dog[3], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::SWAP:	s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);	break;
	}
}