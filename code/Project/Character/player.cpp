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
#include"../UI/miss.h"
#include"../Object/Block/Ghost.h"

// スワップインターバル
const int	CPlayer::SWAP_INTERVAL = 90;	// スワップインターバル
int			CPlayer::s_nSwapInterval = 0;	// 残りスワップインターバル
bool		CPlayer::s_bSwapAnim = false;	//スワップアニメーション中かどうか
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//アニメーション構成

int			CPlayer::s_nGoalInterval = 0;//ゴール後の余韻カウンター

const float CPlayer::SIZE_WIDTH = 7.0f;	// 横幅
const float CPlayer::SIZE_HEIGHT = 8.0f;// 高さ

const float CPlayer::MOVE_SPEED = 0.3f;		// 移動量
const float CPlayer::MAX_MOVE_SPEED = 2.3f;	// 最大移動量

const float CPlayer::JUMP_POWER = 5.0f;		// 基本ジャンプ量
const float CPlayer::GRAVITY_POWER = -0.3f;	// 基本重力加速度

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// スワップマーク
	"data\\TEXTURE\\Effect\\eff_Star_000.png",	// スワップパーティクル00
	"data\\TEXTURE\\Effect\\ink001.png",		// スワップパーティクル01
	"data\\TEXTURE\\Effect\\ink002.png",		// スワップパーティクル02
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// 死亡マーク
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// 死亡パーティクル
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// ゴール・ロケット乗車時のエフェクト
};
int			CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE CPlayer::s_SE = {};	//サウンド用構造体
CPlayer::Motion CPlayer::s_motion = {};	//モーション用構造体
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

	//ゴール後の余韻カウンター
	s_nGoalInterval = 0;

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
		Player.expandCounter = 0;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.StartPos = INITD3DXVECTOR3;		// 開始位置
		Player.doll = NULL;
		Player.pos = INITD3DXVECTOR3;			// 位置
		Player.posOld = INITD3DXVECTOR3;		// 前回位置
		Player.rot = INITD3DXVECTOR3;			// 向き
		Player.move = INITD3DXVECTOR3;			// 移動量
		Player.color = INITCOLOR;				// 色
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA;	// スワップマークのα値
		Player.fSwapPosY = 0.0f;				// スワップ先のＹ座標
		Player.fSwapMoveY = 0.0f;				// スワップ移動時の速度
		Player.bGround = false;					// 地面に接しているか
		Player.bGroundOld = false;				// 地面に接しているか(過去)7
		Player.landingCounter = false;
		Player.bJump = false;					// ジャンプ
		Player.bRide = false;					// ロケットに乗っているかどうか
		Player.bGoal = false;					// ゴールしたかどうか
		Player.fJumpPower = 0.0f;				// ジャンプ量
		Player.fGravity = 0.0f;					// 重力
		Player.fMaxHeight = 0.0f;				// 最高Ｙ座標
		Player.nTramJumpCounter = 0;			// トランポリンによって跳ね上がる最高到達地点
		Player.fTramTargetPosY = 0.0f;			// トランポリン用の目標位置
		Player.bTramJump = false;				// トランポリン用の特殊ジャンプ
		Player.bExtendDog = false;				// ヌイ用の接触フラグ
		Player.bLandPile = false;				// 杭に乗っているかどうか
		Player.side = WORLD_SIDE::FACE;			// どちらの世界に存在するか
	}

	s_pColli = NULL;	// 当たり判定クラス
}

//=======================================
// デストラクタ
//=======================================
CPlayer::~CPlayer()
{
	delete m_aInfo[0].doll;
	delete m_aInfo[1].doll;

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
	m_aInfo[0].doll = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"));
	m_aInfo[0].rot = Rot3D(0.0f, D3DX_PI, 0.0f);

	// ２Ｐ初期情報
	m_aInfo[1].doll = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"));
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
	s_SE.expand = s_SE.pSound->Load("data\\SOUND\\SE\\death_expand.wav");
	s_SE.explosion = s_SE.pSound->Load("data\\SOUND\\SE\\death_explosion.wav");

	s_motion.neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Default.txt");
	s_motion.walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Walk.txt");
	s_motion.jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Jump.txt");
	s_motion.fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Fall.txt");
	s_motion.landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Landing.txt");

	InitInfo();

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
// 情報初期化処理
//=====================================================================================================================
void CPlayer::InitInfo(void) {

	// １Ｐ用初期情報
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;
	m_aInfo[0].rot.y = D3DX_PI;
	m_aInfo[0].scale = INITSCALE3D;

	// ２Ｐ用初期情報
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;
	m_aInfo[1].rot.y = D3DX_PI;
	m_aInfo[1].scale = INITSCALE3D;

	// 両者共通初期情報
	for each (Info & Player in m_aInfo)
	{
		Player.posOld = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
		Player.expandCounter = 0;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
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
		if (++s_nGoalInterval >= GOAL_INTERVAL)
		{
			const int planet = Manager::StgEd()->GetPlanetIdx();
			const int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
			Manager::StgEd()->SwapStage(stage + 1);
		}
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

		// 位置設定
		if (Player.deathCounter == 0 && Player.deathCounter2 == 0 && !Player.bGoal && !s_bSwapAnim) {
			Player.doll->SetPos(Player.pos - Pos3D(0.0f, (fabsf(Player.pos.y) / Player.pos.y) * SIZE_HEIGHT, 0.0f));
			Player.doll->SetRot(Player.rot);
			Player.doll->SetScale(Player.scale);
		}
		else {
			Player.doll->SetPos(Pos3D(10000.0f, 10000.0f, 10000.0f));
		}

		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

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

	// 操作停止フラグを算出
	bool isControlStop = m_aInfo[0].expandCounter > 0 || m_aInfo[1].expandCounter > 0 || m_aInfo[0].deathCounter > 0 || m_aInfo[1].deathCounter > 0 || m_aInfo[0].deathCounter2 > 0 || m_aInfo[1].deathCounter2 > 0;

	if (!isControlStop) {
		// [[[ カメラ制御 ]]]
		Pos3D pos = (m_aInfo[0].pos + m_aInfo[1].pos) * 0.5f;
		pos.x *= 0.25f;
		pos.y = 0.0f;
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::GetMainCamera()->GetPosV(), pos);
	}

	for each (Info & Player in m_aInfo)
	{
		// 次のプレイヤー番号へ
		nIdxPlayer++;

		// 死亡カウンター&演出
		static Vector3D addVec = INITVECTOR3D;
		static Pos3D posVTemp = INITVECTOR3D;
		static Pos3D posRTemp = INITVECTOR3D;
		if (Player.deathCounter > 0) {
			if (--Player.deathCounter == 0) {
				Player.deathCounter2 = DEATH_TIME2;
				addVec = INITVECTOR3D;
				posVTemp = Manager::GetMainCamera()->GetPosV();
				posRTemp = Manager::GetMainCamera()->GetPosR();

				// ミスのテキスト生成
				CMiss::Create();

				// ゴーストの生成
				CGhost::Create(Player.pos, nIdxPlayer);
			}

			float rate = (float)Player.deathCounter / DEATH_TIME;
			float rateOpp = 1.0f - rate;
			Manager::GetMainCamera()->SetMotionBlurColor(Color{ 255,(UShort)(255 * rateOpp),(UShort)(255 * rateOpp),255 });
			Manager::GetMainCamera()->SetMotionBlurPower(0.25f + rate * 0.25f);
			Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.1f));
		}
		// 死亡カウンター2&演出
		if (Player.deathCounter2 > 0) {

			if (--Player.deathCounter2 == 0) {
				InitInfo();

				//オブジェクトのポインタを格納
				CObject *obj = NULL;

				//オブジェクトを取得
				while (Manager::StageObjectMgr()->ListLoop(&obj)) {
					//取得したオブジェクトをキャスト
					CStageObject* stageObj = (CStageObject*)obj;

					//種類取得
					const CStageObject::TYPE type = stageObj->GetType();

					switch (type)
					{
					case CStageObject::TYPE::MISS:
					{
						//取得したオブジェクトをキャスト
						CMiss* Miss = (CMiss*)obj;

						Miss->Delete();	// 削除処理
						break;
					}
					case CStageObject::TYPE::GHOST:
					{
						//取得したオブジェクトをキャスト
						CGhost* Ghost = (CGhost*)obj;

						Ghost->Delete();	// 削除処理
						break;
					}
					}
				}
			}

			bool isReturn = false;
			int counter = Player.deathCounter2;
			if (counter > DEATH_TIME2 / 2)
				counter -= DEATH_TIME2 / 2;
			float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, counter, DEATH_TIME2 / 2);
			if (rate > 0.5f) {
				rate = 0.5f + (0.5f - rate);
				isReturn = true;
			}
			rate /= 0.5f;

			float rate2 = ((float)Player.deathCounter2 / DEATH_TIME2);
			if (rate2 > 0.5f) {
				rate2 = 0.5f + (0.5f - rate2);
			}
			rate2 /= 0.5f;

			addVec += CGeometry::GetRandomVec();
			Manager::GetMainCamera()->SetMotionBlurPower(isReturn ? 0.25f + rate * 0.25f : rate * 0.5f);
			Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.02f));
			Manager::GetMainCamera()->SetPosVAndPosR(
				((Manager::GetMainCamera()->GetPosV() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posVTemp * (1.0f - rate2)),
				((Manager::GetMainCamera()->GetPosR() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posRTemp * (1.0f - rate2)));
		}
		// 膨らみカウンター＆演出
		else if (Player.expandCounter > 0) {
			if (--Player.expandCounter == 0) {
				RNLib::Sound().Play(s_SE.explosion, CSound::CATEGORY::SE, false);
				Manager::GetMainCamera()->SetVib(5.0f);

				const int NUM_PARTICLE = 8;
				Pos3D rot = INITVECTOR3D;
				for (int ParCnt = 0; ParCnt < NUM_PARTICLE; ParCnt++)
				{
					rot.z = -D3DX_PI + D3DX_PI_DOUBLE * fRand();
					CEffect_Death *pEff = Manager::EffectMgr()->DeathParticleCreate(
						RNLib::Model().Load("data\\MODEL\\Effect\\Ball.x"), Player.pos, INITVECTOR3D, rot, INITVECTOR3D, 0.0f, Color{ 255, 155, 59,255 }, CEffect_Death::TYPE::BALL);
					
					const CEffect_Death::BALL_SIZE_LV Lv = (CEffect_Death::BALL_SIZE_LV)(rand() % (int)(CEffect_Death::BALL_SIZE_LV::MAX));
					pEff->SetBallSize(Lv);
				}
				Player.deathCounter = DEATH_TIME;
			}
			Player.scale.x =
			Player.scale.y =
			Player.scale.z = 1.0f + (1.0f - CEase::Easing(CEase::TYPE::IN_SINE, Player.expandCounter, EXPAND_TIME)) * 0.2f;
		}

		// 操作停止であれば折り返す
		if (isControlStop)
			continue;

		// 相方がゴールしていなければ出る
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer + 1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			CGoalGate::EntrySub();
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

			Player.doll->OverwriteMotion(s_motion.jump);
		}

		bool isMove = false;

		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer)) 
		{// 右に移動
			Player.move.x += MOVE_SPEED;
			Player.rot.y += CGeometry::FindAngleDifference(Player.rot.y, D3DX_PI * 0.7f) * 0.5f;
			isMove = true;
		}
		else if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer)) 
		{// 左に移動
			Player.move.x -= MOVE_SPEED;
			Player.rot.y += CGeometry::FindAngleDifference(Player.rot.y, -D3DX_PI * 0.7f) * 0.5f;
			isMove = true;
		}

		if (!Player.bGround) {
			if ((Player.pos.y > 0.0f && Player.move.y < 0.0f) || (Player.pos.y < 0.0f && Player.move.y > 0.0f)) {
				Player.doll->OverwriteMotion(s_motion.fall);
			}
		}
		else if (Player.landingCounter == 0) {
			if (isMove) {
				Player.doll->OverwriteMotion(s_motion.walk);
			}
			else {
				Player.doll->OverwriteMotion(s_motion.neutral);
			}
		}
		else {
			Player.landingCounter--;
		}

		// スワップ入力
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
		{
			//Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI00), Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
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
		//ロケットに乗っていないときにサウンド再生
		if (CRocket::GetCounter() != NUM_PLAYER)
		{
			// スワップアニメーション設定
			s_AnimState = SWAP_ANIM::PROLOGUE;
			s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
			s_bSwapAnim = true;
			s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);
		}

		for each (Info &Player in m_aInfo)
		{
			// ロケットに乗ってたらスキップ
			if (Player.bRide) continue;

			/*for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI00), Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}*/

			// 位置・重力加速度・ジャンプ量・存在する世界を反転
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
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
	//インターバル減少
	s_nSwapInterval--;

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//プレイヤー情報参照（移動時だけパーティクルの位置を手前に出す
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer);break; //プロローグ
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;	//中間
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer);break;//エピローグ
		}

		for (int nCntPar = 0; nCntPar < 1; nCntPar++)
		{
			Color setCol;
			if (nCntPlayer == 0){
				setCol = Color{ (UShort)(215 + rand() % 40),(UShort)(135 + rand() % 40),(UShort)(39 + rand() % 40),255 };
			}
			else{
				setCol = Color{ (UShort)(45 + rand() % 40),(UShort)(130 + rand() % 125),(UShort)(130 + rand() % 125),255 };
			}

			int nTex = rand() % 2 + 2;

			Manager::EffectMgr()->ParticleCreate(GetParticleIdx((PARTI_TEX)nTex), Player.pos, Vector3D(16.0f, 16.0f, 0.0f), setCol,CParticle::TYPE::TYPE_NORMAL,300,D3DXVECTOR3(0.0f,0.0f,(float)(rand() % 629 - 314) / 100.0f),8, CDrawState::ALPHA_BLEND_MODE::NORMAL);
		}
	}
}

//*************************************************
//プロローグ処理
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, const int nIdxPlayer)
{
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
	//最後のプレイヤーのときにスワップアニメーション終了
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_bSwapAnim = false;
}

//----------------------------
// 死亡処理
//----------------------------
void CPlayer::Death(Info& Player, const OBJECT_TYPE type, const int *pColliRot)
{
	if (Player.expandCounter > 0 || Player.deathCounter > 0 || Player.deathCounter2 > 0)
		return;

	Player.expandCounter = EXPAND_TIME;
	RNLib::Sound().Play(s_SE.expand, CSound::CATEGORY::SE, false);
}

//----------------------------
// 移動処理
//----------------------------
void CPlayer::Move(VECTOL vec)
{
	if (m_aInfo[0].expandCounter > 0 || m_aInfo[1].expandCounter > 0 || 
		m_aInfo[0].deathCounter > 0 || m_aInfo[1].deathCounter > 0 ||
		m_aInfo[0].deathCounter2 > 0 || m_aInfo[1].deathCounter2 > 0) {
		return;
	}

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
			const OBJECT_TYPE type = stageObj->GetType();

			// オブジェクトの当たり判定情報を設定
			CCollision::ColliInfo colliInfo;
			colliInfo.pos     = stageObj->GetPos();
			colliInfo.posOld  = colliInfo.pos;
			colliInfo.fWidth  = stageObj->GetWidth() * 0.5f;
			colliInfo.fHeight = stageObj->GetHeight() * 0.5f;
			colliInfo.minPos  = D3DXVECTOR3(colliInfo.pos.x - colliInfo.fWidth, colliInfo.pos.y - colliInfo.fHeight, 0.0f);
			colliInfo.maxPos  = D3DXVECTOR3(colliInfo.pos.x + colliInfo.fWidth, colliInfo.pos.y + colliInfo.fHeight, 0.0f);

			int nColliRot[NUM_PLAYER] = {};
			int nCntPlayer = NONEDATA;
			// プレイヤー毎に処理していく
			for each(Info& Player in m_aInfo) {
				nCntPlayer++;
				// ロケットに乗ってるかゴールしていたら折り返す
				if (Player.bRide || Player.bGoal)
					continue;

				// プレイヤーの近くにオブジェクトがあるか判定
				// ※特定オブジェクトを除く
				if (type != OBJECT_TYPE::TRAMPOLINE && 
					type != OBJECT_TYPE::LASER && 
					type != OBJECT_TYPE::EXTEND_DOG) {

					if (D3DXVec3Length(&(colliInfo.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(colliInfo.fWidth + SIZE_WIDTH, colliInfo.fHeight + SIZE_HEIGHT)))
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

				//別の当たり判定情報
				CCollision::ColliInfo *OtherInfo = NULL;

				// 移動するオブジェクトは、
				// 当たり判定位置に前回位置を設定する
				switch (type) {
					// ブロック
				case OBJECT_TYPE::BLOCK: {
					CBlock* pBlock = (CBlock*)stageObj;
					if (!pBlock->GetCollision())
						continue;
				}break;

					// ゴールゲート
				case OBJECT_TYPE::GOALGATE: {
					CGoalGate* pGoalGate = (CGoalGate*)stageObj;
					if (pGoalGate->GetStartGate())
						continue;
				}break;

					// 移動床
				case OBJECT_TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock*)stageObj;
					colliInfo.posOld = pBlock->GetPosOld();
				}
				break;

					// 隕石
				case OBJECT_TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					colliInfo.posOld = pMeteor->GetPosOld();
				}
				break;

					// レーザー
				case OBJECT_TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					OtherInfo = new CCollision::ColliInfo;
					
					OtherInfo->pos = pLaser->GetLaserPos();
					OtherInfo->posOld = OtherInfo->pos;
					OtherInfo->fWidth = pLaser->GetLaserSize().x * 0.5f;
					OtherInfo->fHeight = pLaser->GetLaserSize().y * 0.5f;

					// 当たった方向を格納
					OtherInfo->Rot = s_pColli->IsBoxCollider(Self, *OtherInfo, vec);
				}
				break;

					// ヌイ
				case OBJECT_TYPE::EXTEND_DOG:{
					//CExtenddog *pDog = (CExtenddog *)stageObj;

					//OtherInfo = new CollInfo[OBJ_EXTENDDOG];

					//for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
					//	switch (nCnt){
					//	case 0:
					//		OtherInfo[nCnt].pos = pDog->GetHeadPos();
					//		OtherInfo[nCnt].posOld = pDog->GetHeadPosOid();
					//		break;
					//	case 1:
					//		OtherInfo[nCnt].pos = pDog->GetBodyPos();
					//		OtherInfo[nCnt].posOld = pDog->GetBodyPos();
					//		break;
					//	case 2:
					//		OtherInfo[nCnt].pos = pDog->GetHipPos();
					//		OtherInfo[nCnt].posOld = pDog->GetHipPos();
					//		break;
					//	}

					//	OtherInfo[nCnt].fWidth = pDog->GetWidth() * 0.5f;
					//	OtherInfo[nCnt].fHeight = pDog->GetHeight() * 0.5f;

					//	if (D3DXVec3Length(&(OtherInfo[nCnt].pos - Player.pos)) >
					//		D3DXVec2Length(&D3DXVECTOR2(OtherInfo[nCnt].fWidth + SIZE_WIDTH, OtherInfo[nCnt].fHeight + SIZE_HEIGHT))) continue;

					//	// オブジェクトの最小・最大位置
					//	OtherInfo[nCnt].minPos = D3DXVECTOR3(OtherInfo[nCnt].pos.x - OtherInfo[nCnt].fWidth, OtherInfo[nCnt].pos.y - OtherInfo[nCnt].fHeight, 0.0f);
					//	OtherInfo[nCnt].maxPos = D3DXVECTOR3(OtherInfo[nCnt].pos.x + OtherInfo[nCnt].fWidth, OtherInfo[nCnt].pos.y + OtherInfo[nCnt].fHeight, 0.0f);

					//	// 当たった方向を格納
					//	OtherInfo[nCnt].ColliRot = s_pColli->IsBoxCollider(*pSelf, colliInfo, vec);

					//	//if (OtherInfo[nCnt].ColliRot != CCollision::ROT::NONE){
					//	//	bOtherColl = true;
					//	//}
					//}

					//CExtenddog::STATE state = pDog->GetState();
					//if ((OtherInfo[2].ColliRot != CCollision::ROT::UNDER) ||
					//	(OtherInfo[2].ColliRot != CCollision::ROT::OVER && state == CExtenddog::STATE::DOWN_LAND)) {
					//	Player.bExtendDog = false;
					//}
				}
				break;

					//杭
				case OBJECT_TYPE::PILE:
				{
					CPile *pPile = (CPile *)stageObj;

					colliInfo.pos = pPile->GetPos();
					colliInfo.posOld = pPile->GetPosOld();
				}
				break;
				}

				// 当たった方向を格納
				colliInfo.Rot = s_pColli->IsBoxCollider(Self, colliInfo, vec);
				nColliRot[nCntPlayer] = (int)colliInfo.Rot;

				// 当たっていない
				if (colliInfo.Rot == CCollision::ROT::NONE)
				{
					//別の当たり判定情報削除
					if (OtherInfo != NULL)
					{
						delete[] OtherInfo;
						OtherInfo = NULL;
					}

					//杭に当たっていないなら乗っていない
					if (type == OBJECT_TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				//死亡判定
				bool bDeath = false;

				// 種類ごとに関数分け
				switch (type)
				{
				case OBJECT_TYPE::BLOCK:	 s_pColli->Block(&Self, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::FILLBLOCK: s_pColli->FillBlock(&Self, colliInfo.Rot, &Player.side, &bDeath); break;
				case OBJECT_TYPE::TRAMPOLINE:s_pColli->Trampoline(&Self, &colliInfo, (CTrampoline *)stageObj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::SPIKE:	 s_pColli->Spike(&Self, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::MOVE_BLOCK:s_pColli->MoveBlock(&Self, (CMoveBlock *)stageObj, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::METEOR:	 s_pColli->Meteor(&Self, &colliInfo, &Player.side, &bDeath); break;
				case OBJECT_TYPE::LASER:	 s_pColli->Laser(&Self, (CRoadTripLaser *)stageObj,&colliInfo, NULL, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::EXTEND_DOG:s_pColli->Dog(&Self, (CExtenddog *)stageObj, &colliInfo, NULL, &Player.side, &bDeath); break;
				case OBJECT_TYPE::GOALGATE:	 s_pColli->GoalGate(&Self, &colliInfo, obj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::PARTS:	 s_pColli->Parts(&Self, (CParts *)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::ROCKET:	 s_pColli->Rocket(&Self, (CRocket *)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::PILE:		 s_pColli->Pile(&Self, &colliInfo, (CPile *)stageObj, &Player.side, &bDeath); break;
				}

				//別の当たり判定情報削除
				if (OtherInfo != NULL)
				{
					delete[] OtherInfo;
					OtherInfo = NULL;
				}

				// 死亡判定ON
				if (bDeath)
				{
					Death(Player, type, &nColliRot[nCntPlayer]);
					break;
				}

				//情報代入
				Player.pos = Self.pos;
				Player.move = Self.move;
			}

			//当たり判定の事後処理
			CollisionAfter(stageObj, type, &nColliRot[0]);
		}
	}
}

//----------------------------
// 各プレイヤーの当たり判定が終わった後の処理
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, int *pColliRot)
{
	// 種類ごとに関数分け
	switch (type)
	{
		//ブロックのリアクションフラグ
		case CStageObject::TYPE::BLOCK:
		{
			typedef CCollision::ROT ColRot;		 //衝突方向の別名
			Info *pInfo = &m_aInfo[0];			 //プレイヤー情報のポインタ
			CBlock *pBlock = (CBlock *)pStageObj;//ブロックへキャスト

			for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++, pInfo++, pColliRot++)
			{
				pBlock->IsReaction_HitsRot(*pColliRot);	//当たった方向を代入
				pBlock->IsReaction_Move(D3DXVec3Length(&pInfo->move) != 0.0f);
				
				//着地したかどうか
				if (((ColRot)*pColliRot == ColRot::OVER  && pInfo->side == WORLD_SIDE::FACE && pInfo->pos.y < pInfo->posOld.y) ||
					((ColRot)*pColliRot == ColRot::UNDER && pInfo->side == WORLD_SIDE::BEHIND && pInfo->pos.y > pInfo->posOld.y))
					pBlock->IsReaction_Land(true);
			}
			break;
		}

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