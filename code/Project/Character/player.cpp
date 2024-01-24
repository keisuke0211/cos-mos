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
#include "../resource.h"
#include "../stage.h"

const Color CPlayer::P1_COLOR = Color{ 255, 155, 59, 255 };
const Color CPlayer::P2_COLOR = Color{ 65, 233, 210, 255 };

// スワップインターバル
CInt   CPlayer::SWAP_INTERVAL = 20;	// スワップインターバル
CFloat CPlayer::GUIDE_WIDTH   = 10.0f; // ガイドの幅
CFloat CPlayer::GUIDE_HEIGHT  = 14.0f; // ガイドの高さ
CFloat CPlayer::MAX_GUIDE_SPEED = 0.2f;// ガイドアニメーションの最大速度

int                CPlayer::s_nSwapInterval = 0; // 残りスワップインターバル
bool               CPlayer::s_bSwapAnim = false; //スワップアニメーション中かどうか
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//アニメーション構成

int CPlayer::s_nGoalInterval = 0;//ゴール後の余韻カウンター
int CPlayer::s_zoomUpCounter = 0;// ズームアップカウンター
int CPlayer::s_zoomUpFixedCounter = 0;// ズームアップカウンター

CFloat CPlayer::SIZE_WIDTH = 7.0f; // 横幅
CFloat CPlayer::SIZE_HEIGHT = 8.0f;// 高さ

CFloat CPlayer::MOVE_SPEED = 0.3f;     // 移動量
CFloat CPlayer::MAX_MOVE_SPEED = 2.3f; // 最大移動量

CFloat CPlayer::JUMP_POWER = 5.0f;     // 基本ジャンプ量
CFloat CPlayer::GRAVITY_POWER = -0.3f; // 基本重力加速度

CInt CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// スワップマーク
	"data\\TEXTURE\\Effect\\eff_Star_000.png",  // スワップパーティクル00
	"data\\TEXTURE\\Effect\\ink001.png",        // スワップパーティクル01
	"data\\TEXTURE\\Effect\\ink002.png",        // スワップパーティクル02
	"data\\TEXTURE\\Effect\\swap_guide.png",    // スワップガイド
	"data\\TEXTURE\\player.PNG",                // プレイヤーアイコン
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// 死亡マーク
	"data\\TEXTURE\\Effect\\ink001.png",        // 死亡インク
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // 死亡パーティクル
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // ゴール・ロケット乗車時のエフェクト
	"data\\TEXTURE\\Effect\\eff_Smoke_000.png", // 煙のテクスチャ
};
int CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE     CPlayer::s_SE = {};	//サウンド用構造体
CPlayer::Motion CPlayer::s_motion[2] = {};	//モーション用構造体
bool            CPlayer::ms_bSwapEnd = false;
UShort          CPlayer::ms_guideCounter = 0;

bool  CPlayer::s_bAimPlayer = false;
int   CPlayer::s_nAimNo = 0;
Pos3D CPlayer::s_addPosV = INITPOS3D;
Pos3D CPlayer::s_addPosR = INITPOS3D;
float CPlayer::s_fCorrWidth = 0.0f;
float CPlayer::s_fCorrHeight = 0.0f;
float CPlayer::s_fAimWorkSpeed = 1.0f;
CPlayer::Info CPlayer::m_aInfo[CPlayer::NUM_PLAYER];	// 各プレイヤーの情報

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
	s_SE.Swaping = 0;
	s_SE.SwapEnd = 0;
	for each(short &dog in s_SE.dog)
	{
		dog = 0;
	}

	int cntPlayer = 0;
	for each(Info &Player in m_aInfo)
	{
		Player.idx = cntPlayer;
		Player.expandCounter = 0;
		Player.isDeath = false;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.DeathType = 0;
		Player.doll = NULL;
		Player.pos = INITD3DXVECTOR3;          // 位置
		Player.posOld = INITD3DXVECTOR3;       // 前回位置
		Player.rot = INITD3DXVECTOR3;          // 向き
		Player.move = INITD3DXVECTOR3;         // 移動量
		Player.color = COLOR_WHITE;            // 色
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA; // スワップマークのα値
		Player.fSwapPosY = 0.0f;               // スワップ先のＹ座標
		Player.fSwapMoveY = 0.0f;              // スワップ移動時の速度
		Player.fGuideTexVPos = 0.0f;           // ガイドのテクスチャＶ座標
		Player.fGuideTexVSize = 0.0f;          // ガイドのテクスチャＶサイズ
		Player.fGuideMoveSpeed = 0.0f;         // ガイドのテクスチャ移動スピード
		Player.bGround = true;                 // 地面に接しているか
		Player.bGroundOld = true;              // 地面に接しているか(過去)
		Player.landingCounter = false;
		Player.bJump = false;                  // ジャンプ
		Player.bRide = false;                  // ロケットに乗っているかどうか
		Player.bGoal = false;                  // ゴールしたかどうか
		Player.pGoalGate = NULL;
		Player.fJumpPower = 0.0f;              // ジャンプ量
		Player.fGravity = 0.0f;                // 重力
		Player.fMaxHeight = 0.0f;              // 最高Ｙ座標
		Player.nTramJumpCounter = 0;           // トランポリンによって跳ね上がる最高到達地点
		Player.fTramTargetPosY = 0.0f;         // トランポリン用の目標位置
		Player.bTramJump = false;              // トランポリン用の特殊ジャンプ
		Player.bExtendDog = false;             // ヌイ用の接触フラグ
		Player.bLandPile = false;              // 杭に乗っているかどうか
		Player.side = WORLD_SIDE::FACE;        // どちらの世界に存在するか
		cntPlayer++;
	}
}

//=======================================
// デストラクタ
//=======================================
CPlayer::~CPlayer()
{
	delete m_aInfo[0].doll;
	delete m_aInfo[1].doll;
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
	ms_bSwapEnd = false;
	ms_guideCounter = 0;

	// １Ｐ初期情報
	if (m_aInfo[0].doll != NULL)
		delete m_aInfo[0].doll;
	m_aInfo[0].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"), TRUE);
	m_aInfo[0].rot = Rot3D(0.0f, D3DX_PI, 0.0f);
	m_aInfo[0].color = P1_COLOR;

	// ２Ｐ初期情報
	if (m_aInfo[1].doll != NULL)
		delete m_aInfo[1].doll;
	m_aInfo[1].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Eye.txt"), TRUE);
	m_aInfo[1].rot = CStageObject::INVERSEVECTOR3;
	m_aInfo[1].color = P2_COLOR;

	// キーコンフィグ初期化
	InitKeyConfig();

	//パーティクルテクスチャ読込
	_RNC_Texture *pTexture = &RNLib::Texture();
	for (int nCntTex = 0; nCntTex < (int)PARTI_TEX::MAX; nCntTex++)
	{
		s_ParticleTex[nCntTex] = pTexture->Load(PARTICLE_TEX_PATH[nCntTex]);
	}

	s_SE.pSound = &RNLib::Sound();
	s_SE.jump = s_SE.pSound->Load("data\\SOUND\\SE\\jamp_01.wav");
	s_SE.dog[0] = s_SE.pSound->Load("data\\SOUND\\SE\\heron2.wav");		// 押す
	s_SE.dog[1] = s_SE.pSound->Load("data\\SOUND\\SE\\shrink.wav");		// 縮む
	s_SE.dog[2] = s_SE.pSound->Load("data\\SOUND\\SE\\extend.wav");		// 伸びる
	s_SE.dog[3] = s_SE.pSound->Load("data\\SOUND\\SE\\vibration.wav");	// 震える
	s_SE.Swaping = s_SE.pSound->Load("data\\SOUND\\SE\\swap_now.wav");
	s_SE.SwapEnd = s_SE.pSound->Load("data\\SOUND\\SE\\swap_end.wav");
	s_SE.expand = s_SE.pSound->Load("data\\SOUND\\SE\\death_expand.wav");
	s_SE.explosion = s_SE.pSound->Load("data\\SOUND\\SE\\death_explosion.wav");

	s_motion[0].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Default.txt");
	s_motion[0].walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Walk.txt");
	s_motion[0].jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Jump.txt");
	s_motion[0].fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Fall.txt");
	s_motion[0].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Landing.txt");
	s_motion[0].dance = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Dance.txt");
	s_motion[0].getup = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\GetUp.txt");

	s_motion[1].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Default.txt");
	s_motion[1].walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Walk.txt");
	s_motion[1].jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Jump.txt");
	s_motion[1].fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Fall.txt");
	s_motion[1].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Landing.txt");
	s_motion[1].dance = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Dance.txt");
	s_motion[1].getup = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\GetUp.txt");

	InitInfo();

	// 初期値設定
	// ※ 来れないとステージ入る前に一瞬着地SEがなる
	m_aInfo[0].bJump = m_aInfo[1].bJump = false;

	s_zoomUpCounter = 0;
	s_zoomUpFixedCounter = 0;
	if (Manager::StgEd()->GetPlanetIdx() == 0) {
		if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
			s_zoomUpCounter = ZOOM_UP_TIME;
			s_zoomUpFixedCounter = ZOOM_UP_FIXED_TIME;
		}
	}

	//カットイン
	Stage::SetIsCutIn(false);

	//カウント
	nCnt = 60;

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
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = _RNC_Input::BUTTON::LEFT;  // 左移動
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = _RNC_Input::BUTTON::RIGHT; // 右移動
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = _RNC_Input::BUTTON::A;     // ジャンプ
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = _RNC_Input::BUTTON::X;     // スワップ
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = _RNC_Input::BUTTON::A;     // 決定
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = _RNC_Input::BUTTON::START; // ポーズ
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
		Player.isDeath = false;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.swapWaitCounter = 0;
		Player.nEscapeGoalInterval = 0;
	}

	CGoalGate::ResetEtr();
}

//=====================================================================================================================
// 終了処理
//=====================================================================================================================
void CPlayer::Uninit(void)
{

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
		UpdateInfo();
		return;
	}

	// 操作処理
	ActionControl();

	// 両者ともにゴールしてなかったら
	if ((!m_aInfo[0].bGoal || !m_aInfo[1].bGoal) &&
		(!m_aInfo[0].bRide || !m_aInfo[1].bRide))
	{
		// スワップ
		Swap();
	}
	else if ((m_aInfo[0].bGoal && m_aInfo[1].bGoal) ||
		     (m_aInfo[0].bRide && m_aInfo[1].bRide))
	{
		//ゴール演出
		GoalDirector();
	}

	// 当たり判定まとめ
	CollisionToStageObject();

	// 過去の位置を設定
	m_aInfo[0].posOld = m_aInfo[0].pos;
	m_aInfo[1].posOld = m_aInfo[1].pos;

	// 情報更新
	UpdateInfo();
}

//----------------------------
// 情報更新処理
//----------------------------
void CPlayer::UpdateInfo(void)
{
	// ガイドテキスト
	const bool isSwapGuide = SwapGuideText();

	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;

		UpdateDeath(Player, nCntPlayer);

		// 位置設定
		if (Player.deathCounter == 0 && Player.deathCounter2 == 0 && !Player.bGoal && !s_bSwapAnim && !Player.bRide) {
			Player.doll->SetPos(Player.pos - Pos3D(0.0f, (fabsf(Player.pos.y) / Player.pos.y) * SIZE_HEIGHT, 0.0f));
			Player.doll->SetRot(Player.rot);
			Player.doll->SetScale(Player.scale);
			Player.doll->SetIsShow(true);

			// 黒目の描画
			if (nCntPlayer == 1) {
				Matrix eyeMtx = RNLib::Matrix().ConvPosRotToMtx(Pos3D(0.0f, 6.0f, 6.85f), Rot3D(0.0f, D3DX_PI, 0.0f));

				static int eyeCounter = 0;
				static int eyeCounter2 = 0;
				static int eyeTime = 0;

				if (eyeCounter2 > 0) {
					eyeCounter2--;
				}
				else if (++eyeCounter >= eyeTime) {
					eyeCounter = 0;
					eyeCounter2 = 4;
					eyeTime = 5 + (rand() % 90);
				}

				RNLib::Polygon3D().Put(PRIORITY_OBJECT, RNLib::Matrix().MultiplyMtx(Player.doll->GetBoneState(0).GetWorldMtx(), eyeMtx))
					->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::CHR_BLACK_EYE], (eyeCounter2 > 0), 2, 1)
					->SetSize(4.0f, 4.0f);
			}
		}
		else {
			Player.doll->SetIsShow(false);
		}

		// スワップアニメーション中であれば折り返す
		if (s_bSwapAnim) {
			continue;
		}

		if (isSwapGuide && !IsKeyConfigPress(nCntPlayer, Player.side, KEY_CONFIG::SWAP)) {
			if (-156.0f <= Player.pos.x && 156.0f >= Player.pos.x) {
				Pos3D putPos = Player.pos;
				putPos.y += (Player.pos.y / fabsf(Player.pos.y)) * 24.0f;
				CInt count = RNLib::Count().GetCount() % 40;
				float sizeRate = (float)(count - ((count % 20) * (count / 20)) * 2) / 20;
				RNLib::Text3D().Put(PRIORITY_UI, "X", _RNC_Text::ALIGNMENT::CENTER, 0, RNLib::Matrix().ConvPosToMtx(putPos))
					->SetSize(Size2D(16.0f * (0.5f + sizeRate * 0.25f), 16.0f * (0.5f + sizeRate * 0.25f)))
					->SetZTest(false)
					->SetBillboard(true);
			}
		}

		//ロケット乗り降りインターバル減少
		if (Player.nEscapeGoalInterval > 0)
			Player.nEscapeGoalInterval--;

		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal)
			continue;

		//スワップガイド表示
		if (!Player.isDeath)
			SwapGuide(Player);

		// 最高Ｙ座標更新
		switch (Player.side) {
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
	}
}

//----------------------------
// 死亡更新処理
//----------------------------
void CPlayer::UpdateDeath(Info& info, CInt& count) {

	// 死亡カウンター&演出
	static Vector3D addVec = INITVECTOR3D;
	static Pos3D posVTemp = INITVECTOR3D;
	static Pos3D posRTemp = INITVECTOR3D;
	if (info.deathCounter > 0) {
		if (--info.deathCounter == 0) {
			info.deathCounter2 = DEATH_TIME2;
			addVec = INITVECTOR3D;
			posVTemp = Manager::GetMainCamera()->GetPosV();
			posRTemp = Manager::GetMainCamera()->GetPosR();

			// ミスのテキスト生成
			CMiss::Create();

			// ゴーストの生成
			CGhost::Create(info.pos, count);
		}

		float rate = (float)info.deathCounter / DEATH_TIME;
		float rateOpp = 1.0f - rate;
		Manager::GetMainCamera()->SetMotionBlurColor(Color(255,(int)(255 * rateOpp), (int)(255 * rateOpp),255));
		Manager::GetMainCamera()->SetMotionBlurPower(0.25f + rate * 0.25f);
		Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.1f));
	}
	// 死亡カウンター2&演出
	if (info.deathCounter2 > 0) {

		if (--info.deathCounter2 == 0) {
			//プレイヤー情報初期化　＋　特定のオブジェクトの初期化
			InitInfo();
			CPile::ResetTrunkHeightAll();

			//オブジェクトを取得
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {
				//取得したオブジェクトをキャスト
				CStageObject* pObj = (CStageObject*)obj;

				switch (pObj->GetType())
				{//オブジェクトを変換して削除
				case CStageObject::TYPE::MISS:  { CMiss*  Miss  = (CMiss*)obj;  Miss->Delete(); }break;
				case CStageObject::TYPE::GHOST: { CGhost* Ghost = (CGhost*)obj; Ghost->Delete();} break;
				}
			}
		}

		bool isReturn = false;
		int counter = info.deathCounter2;
		if (counter > DEATH_TIME2 / 2)
			counter -= DEATH_TIME2 / 2;
		float rate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, counter, DEATH_TIME2 / 2);
		if (rate > 0.5f) {
			rate = 0.5f + (0.5f - rate);
			isReturn = true;
		}
		rate /= 0.5f;

		float rate2 = ((float)info.deathCounter2 / DEATH_TIME2);
		if (rate2 > 0.5f) {
			rate2 = 0.5f + (0.5f - rate2);
		}
		rate2 /= 0.5f;

		addVec += RNLib::Geometry().GetRandomVec();
		Manager::GetMainCamera()->SetMotionBlurPower(isReturn ? 0.25f + rate * 0.25f : rate * 0.5f);
		Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.02f));
		Manager::GetMainCamera()->SetPosVAndPosR(
			((Manager::GetMainCamera()->GetPosV() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posVTemp * (1.0f - rate2)),
			((Manager::GetMainCamera()->GetPosR() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posRTemp * (1.0f - rate2)));
	}

	// 膨らみカウンター＆演出
	else if (info.expandCounter > 0) {
		if (--info.expandCounter == 0) {
			RNLib::Sound().Play(s_SE.explosion, _RNC_Sound::CATEGORY::SE, 1.0f, false);
			Manager::GetMainCamera()->SetVib(5.0f);
			RNLib::Input().SetVibration(2.0f, count);
			RNLib::Input().SetVibration(1.0f, !count);

			const CEffect_Death::TYPE type = (CEffect_Death::TYPE)info.DeathType;
			switch (type)
			{
				case CEffect_Death::TYPE::INK:
				{
					CEffect_Death* pEff = Manager::EffectMgr()->DeathParticleCreate(info.pos, INITVECTOR3D, INITVECTOR3D, INITVECTOR3D, CStageObject::SIZE_OF_1_SQUARE, info.color, type);
				}
					break;

				case CEffect_Death::TYPE::BALL:
				{
					CInt NUM_PARTICLE = 8;
					Pos3D rot = INITVECTOR3D;
					for (int ParCnt = 0; ParCnt < NUM_PARTICLE; ParCnt++)
					{
						rot.z = -D3DX_PI + D3DX_PI_DOUBLE * RNLib::Number().GetRandomFloat(1.0f);
						CEffect_Death* pEff = Manager::EffectMgr()->DeathParticleCreate(info.pos, INITVECTOR3D, rot, INITVECTOR3D, 0.0f, info.color, type);

						pEff->SetBallSize(CEffect_Death::BALL_SIZE_LV::SMALL);
					}
				}break;
			}
			info.deathCounter = DEATH_TIME;
		}
		info.scale.x =
		info.scale.y =
		info.scale.z = 1.0f + (1.0f - RNLib::Ease().Easing(EASE_TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 0.2f;
		info.pos.z = -(1.0f - RNLib::Ease().Easing(EASE_TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 8.0f;
	}
}

//----------------------------
// スワップガイドテキスト
//----------------------------
bool CPlayer::SwapGuideText(void)
{
	//スワップ中 or 最初の惑星ではない
	if (s_bSwapAnim || Manager::StgEd()->GetPlanetIdx() != 0) return false;

	if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath &&
		!m_aInfo[0].bGoal || !m_aInfo[1].bGoal) {

		if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
			if (s_zoomUpCounter == 0) {
				if (++ms_guideCounter > 30)
					ms_guideCounter = 30;
				CFloat rate = (float)ms_guideCounter / 30;
				if (ms_bSwapEnd) {

					if (ms_guideCounter == 1) {
						RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], _RNC_Sound::CATEGORY::SE, 1.0f, false);
					}
					if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
						RNLib::Text3D().Put(PRIORITY_UI, "OK!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
							->SetSize(Size2D(32.0f * rate, 32.0f * rate))
							->SetZTest(false)
							->SetBillboard(true);
					}
				}
				else {
					if (s_nSwapInterval == 0) {
						if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
							RNLib::Text3D().Put(PRIORITY_UI, "SWAPしてみよう!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
								->SetSize(Size2D(24.0f * rate, 24.0f * rate))
								->SetZTest(false)
								->SetBillboard(true);
							return true;
						}
					}
				}
			}
		}
		else if (Manager::StgEd()->GetType()[0].nStageIdx == 3) {
			if (++ms_guideCounter > 30)
				ms_guideCounter = 30;

			//カウンターが進むほどテキストを拡大させるためのカウンター割合
			CFloat rate = (float)ms_guideCounter / 30;
			if (CParts::GetDispNum() == 0) {
				if (ms_guideCounter == 1) 
					RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], _RNC_Sound::CATEGORY::SE, 1.0f, false);

				if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
					RNLib::Text3D().Put(PRIORITY_UI, "とびのれ!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
						->SetSize(Size2D(32.0f * rate, 32.0f * rate))
						->SetZTest(false)
						->SetBillboard(true);
				}
			}
			else if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
				RNLib::Text3D().Put(PRIORITY_UI, "ロケットのパーツをあつめて!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
					->SetSize(Size2D(24.0f * rate, 24.0f * rate))
					->SetZTest(false)
					->SetBillboard(true);
			}
		}
	}

	return false;
}

//----------------------------
// 操作処理
//----------------------------
void CPlayer::ActionControl(void)
{
	// プレイヤー番号
	int nIdxPlayer = -1;

	bool isZoomUp = false;
	if (s_zoomUpCounter > 0 &&
		Manager::StgEd()->GetPlanetIdx() == 0 &&
		Manager::StgEd()->GetType()[0].nStageIdx == 0) {
		isZoomUp = true;
	}

	// [[[ カメラ制御 ]]]
	if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath) 
	{// どちらも死んでいない
		Pos3D targetPosV = Manager::StgEd()->GetCameraPos();
		Pos3D targetPosR = (m_aInfo[0].pos + m_aInfo[1].pos) * 0.5f;
		targetPosR.x *= 0.25f;
		targetPosR.y = 0.0f;

		if (isZoomUp) 
		{// ズームアップ中
			if (s_zoomUpFixedCounter > 0) {
				s_zoomUpFixedCounter--;
				if (s_zoomUpFixedCounter <= ZOOM_UP_FIXED_TIME - 60) {
					if (s_zoomUpFixedCounter == ZOOM_UP_FIXED_TIME - 60) {
						m_aInfo[0].doll->OverwriteMotion(s_motion[0].jump);
						m_aInfo[1].doll->OverwriteMotion(s_motion[1].jump);
						m_aInfo[0].move.y += 2.5f;
						m_aInfo[1].move.y -= 2.5f;
					}
				}
				else {
					m_aInfo[0].doll->OverwriteMotion(s_motion[0].getup);
					m_aInfo[1].doll->OverwriteMotion(s_motion[1].getup);
				}
			}
			else {
				s_zoomUpCounter--;
			}
			
			if (nCnt > 0) {

				nCnt--;
				if (nCnt == 0) {

					CInt NumEffect = 40;
					for (int Cnt = 0; Cnt < NumEffect; Cnt++) {
						CInt Num = rand() % 2;
						const Pos3D TexPos = Pos3D(m_aInfo[Num].pos.x + (float)(rand() % (int)12.0f - 12.0f * 0.5), m_aInfo[Num].pos.y - 8.0f * cosf(m_aInfo[Num].rot.z), m_aInfo[Num].pos.z);
						const Rot3D TexRot = Pos3D(m_aInfo[Num].rot.x, m_aInfo[Num].rot.y, m_aInfo[Num].rot.z + ((rand() % 1570) - 785) * 0.001f);
						CFloat ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.9f) + INIT_EFFECT_SCALE.x * 0.1f);
						Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SMOKE_EFFECT), TexPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 600, TexRot, D3DXVECTOR3(80.0f, 80.0f, 0.0f), false, false, _RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
						Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(TexPos.x, TexPos.y + 1.0f * cosf(TexRot.y), TexPos.z), TexRot, INITSCALE3D * 0.05f, COLOR_WHITE);
					}
				}
			}

			Pos3D basePosRMain = m_aInfo[0].pos + Pos3D(0.0f, -16.0f, 0.0f);
			Pos3D basePosRSub  = m_aInfo[1].pos + Pos3D(0.0f, 16.0f, 0.0f);
			Pos3D basePosVMain = basePosRMain + Pos3D(0.0f, 0.0f, -100.0f);
			Pos3D basePosVSub  = basePosRSub  + Pos3D(0.0f, 0.0f, -100.0f);
			float rate = 1.0f;
			if (s_zoomUpCounter < 60) {
				rate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, s_zoomUpCounter, 60);
			}
			CFloat rateOpp = 1.0f - rate;
			Manager::GetMainCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVMain * rate, targetPosR * rateOpp + basePosRMain * rate);
			Manager::GetSubCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVSub * rate, targetPosR * rateOpp + basePosRSub * rate);

			//計測再開
			if (s_zoomUpCounter == 0)
			{
				CMode_Game::RestartTime();
			}
			return;
		}
		else{
			_RNC_Input *pInput = &RNLib::Input();
			if (RNSystem::GetMode() == RNSystem::MODE::DEBUG) {
				if (pInput->GetKeyTrigger(DIK_B))
				{//視点切替
					if (!s_bAimPlayer)
					{
						s_bAimPlayer = !s_bAimPlayer;
						s_nAimNo = 0;
					}
					else if (s_nAimNo == 0) {
						s_nAimNo = 1;
					}
					else
					{
						s_bAimPlayer = false;
						s_nAimNo = 0;
					}

					if (s_bAimPlayer) {
						s_addPosV.x = s_addPosR.x = m_aInfo[s_nAimNo].pos.x + s_fCorrWidth;
						s_addPosV.y = s_addPosR.y = m_aInfo[s_nAimNo].pos.y + s_fCorrHeight;
						s_addPosV.z = -100.0f;
					}
				}
			}

			if (s_bAimPlayer) {
				if (RNLib::Input().GetKeyPress(DIK_R)) {
					if (RNLib::Input().GetKeyPress(DIK_T)) { s_addPosR.z += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_G)) { s_addPosR.z -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_H)) { s_addPosR.x += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_F)) { s_addPosR.x -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_U)) { s_addPosR.y += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_J)) { s_addPosR.y -= 1.0f; }
				}
				else {
					if (RNLib::Input().GetKeyPress(DIK_T)) { s_addPosV.z += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_G)) { s_addPosV.z -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_H)) { s_addPosV.x += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_F)) { s_addPosV.x -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_U)) { s_addPosV.y += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_J)) { s_addPosV.y -= 1.0f; }
				}

				Manager::GetMainCamera()->SetPosVAndPosR(s_addPosV, s_addPosR);

#if 0
				if (pInput->GetKeyTrigger(DIK_V)) s_fCorrWidth -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_N)) s_fCorrWidth += 0.1f;
				if (pInput->GetKeyTrigger(DIK_G)) s_fCorrHeight -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_H)) s_fCorrHeight += 0.1f;
				
				//本来の当たり判定範囲
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,255,255,255 })
					->SetSize(SIZE_WIDTH * 2.0f, SIZE_HEIGHT * 2.0f);

				//半分の当たり判定範囲
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,100,100,255 })
					->SetSize(SIZE_WIDTH * 1.0f, SIZE_HEIGHT * 1.0f);

				RNLib::Text2D().PutDebugLog(String("横の調整量:%f  縦の調整量:%f", s_fCorrWidth, s_fCorrHeight));
#endif
			}
			else {
				Manager::GetMainCamera()->SetPosVAndPosR(targetPosV, targetPosR);
			}
		}
	}
	else 
	{// どちらかが死んでいる
		return;
	}

	for each (Info & Player in m_aInfo)
	{
		// 次のプレイヤー番号へ
		nIdxPlayer++;

		// 相方がゴールしていなければ出る
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer + 1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			//ロケット脱出のインターバル設定
			if (Player.bRide)
				Player.nEscapeGoalInterval = CRocket::ESCAPE_INTERVAL;

			//ゴールドア脱出のインターバル設定
			else if (Player.bGoal)
				Player.nEscapeGoalInterval = CGoalGate::ESCAPE_INTERVAL;

			CGoalGate::EntrySub();
			CRocket::RideOff();
			Player.bRide = false;
			Player.bGoal = false;

			if(Player.pGoalGate != NULL)
				Player.pGoalGate->LeaveDoor();
		}

		// ロケットに乗っている　or ゴールしている or ズームアップ or タイムオーバーの時スキップ
		if (Player.bRide || Player.bGoal || isZoomUp || Stage::GetIsTimeOver()) 
			continue;

		bool isMove = false;

		// スワップ入力
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP)) {
			Player.nSwapAlpha = 255;
			if (++Player.swapWaitCounter > SWAP_WAIT_BALLOON_TIME)
				Player.swapWaitCounter = SWAP_WAIT_BALLOON_TIME;
			Player.rot.y = D3DX_PI;
		}
		//スワップ非入力
		else {
			Player.nSwapAlpha = NORMAL_SWAP_ALPHA;
			if (--Player.swapWaitCounter < 0)
				Player.swapWaitCounter = 0;

			// ジャンプ入力（空中じゃない）
			if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
			{
				Player.bGround = false;            // 地面から離れた
				Player.move.y = Player.fJumpPower; // ジャンプ量代入
				Player.bJump = true;               // ジャンプした
				PlaySE(SE_LABEL::JUMP);            // SE再生
			}

			if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
				RNLib::Input().GetStickAnglePress(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::RIGHT, nIdxPlayer))
			{// 右に移動
				Player.move.x += MOVE_SPEED;
				Player.rot.y += RNLib::Geometry().FindAngleDifference(Player.rot.y, D3DX_PI * 0.7f) * 0.5f;
				isMove = true;

			}
			else if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
				RNLib::Input().GetStickAnglePress(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::LEFT, nIdxPlayer))
			{// 左に移動
				Player.move.x -= MOVE_SPEED;
				Player.rot.y += RNLib::Geometry().FindAngleDifference(Player.rot.y, -D3DX_PI * 0.7f) * 0.5f;
				isMove = true;
			}
		}

		if (Player.swapWaitCounter > 0) {
			Player.doll->OverwriteMotion(s_motion[nIdxPlayer].dance);
		}
		else if (!Player.bGround) {
			if (Player.bGroundOld) {
				Player.doll->OverwriteMotion(s_motion[0].jump);
			}
			else if(Player.doll->GetMotionCounter() > 10){
				if ((Player.pos.y > 0.0f && Player.move.y < 0.0f) || (Player.pos.y < 0.0f && Player.move.y > 0.0f)) {
					Player.doll->OverwriteMotion(s_motion[nIdxPlayer].fall);
				}
			}
		}
		else if (Player.landingCounter == 0) {
			if (isMove) {
				Player.doll->OverwriteMotion(s_motion[nIdxPlayer].walk);
			}
			else {
				Player.doll->OverwriteMotion(s_motion[nIdxPlayer].neutral);
			}
		}
		else {
			Player.landingCounter--;
		}

		{// 吹き出しの表示
			Pos3D putPos = Player.pos;
			putPos.y += RNLib::Number().GetPlusMinus(Player.pos.y) * 12.0f;
			_RNC_Polygon3D::CRegistInfo* polygon3D = RNLib::Polygon3D().Put(PRIORITY_UI, putPos, Rot3D(0.0f, 0.0f, -0.1f + (RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, RNLib::Number().GetTurnNum(RNLib::Count().GetCount(), 30), 30)) * 0.2f))
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_WAITBUBBLE], Player.pos.y < 0.0f, 2, 1)
				->SetCol(Color(255, 255, 255, 255 * ((float)Player.swapWaitCounter / SWAP_WAIT_BALLOON_TIME)))
				->SetZTest(false);
			
			Size2D size = Size2D(16.0f, 32.0f * ((float)Player.swapWaitCounter / SWAP_WAIT_BALLOON_TIME));
			if (Player.pos.y > 0.0f) {
				polygon3D->SetVtxPos(
					Pos3D(-size.x, size.y, 0.0f),
					Pos3D(size.x, size.y, 0.0f),
					Pos3D(-size.x, 0.0f, 0.0f),
					Pos3D(size.x, 0.0f, 0.0f));
			}
			else {
				polygon3D->SetVtxPos(
					Pos3D(-size.x, 0.0f, 0.0f),
					Pos3D(size.x, 0.0f, 0.0f),
					Pos3D(-size.x, -size.y, 0.0f),
					Pos3D(size.x, -size.y, 0.0f));
			}
		}
	}
}

//#################################################
// スワップ処理
//#################################################
void CPlayer::Swap(void)
{
	// どちらかが死んでいたら終了
	if (m_aInfo[0].isDeath || m_aInfo[1].isDeath)
		return;

	//インターバル中
	if (s_nSwapInterval != 0)
	{
		s_nSwapInterval--;
		return;
	}

	// 両者ともにスワップボタンを押している
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) && m_aInfo[0].swapWaitCounter > 0) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) && m_aInfo[1].swapWaitCounter > 0))
	{
		//ロケットに乗っていないときにサウンド再生
		if (CRocket::GetCounter() != NUM_PLAYER)
		{
			// スワップアニメーション設定
			s_AnimState = SWAP_ANIM::PROLOGUE;
			s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
			s_bSwapAnim = true;
			PlaySE(SE_LABEL::SWAPING);
		}

		for each (Info &Player in m_aInfo)
		{
			// ロケットに乗ってたらスキップ
			if (Player.bRide) continue;

			// 位置・重力加速度・ジャンプ量・存在する世界を反転
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
			Player.bTramJump = false;

			// 前回位置更新
			Player.posOld = Player.pos;
			Player.bGroundOld = Player.bGround;

			// 吹き出しカウンター初期化
			Player.swapWaitCounter = 0;
		}
	}
}

//*************************************************
// スワップアニメーション
//*************************************************
void CPlayer::SwapAnimation(void)
{
	//インターバル減少
	s_nSwapInterval--;

	RNLib::Polygon2D().Put(PRIORITY_SWAP_CURTAIN, INITPOS2D, 0.0f)
		->SetSize(10000.0f, 10000.0f)
		->SetCol(Color{ 0,0,0,100 });

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//プレイヤー情報参照（移動時だけパーティクルの位置を手前に出す
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer); break; //プロローグ
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;	//中間
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer); break;//エピローグ
		}

		//ゴールしてたらエフェクト非表示
		if (Player.bGoal || Player.bRide) continue;

		Color setCol;
		if (nCntPlayer == 0){
			setCol = Color{ (215 + rand() % 40),(135 + rand() % 40),(39 + rand() % 40),255 };
		}
		else{
			setCol = Color{ (45 + rand() % 40),(130 + rand() % 125),(130 + rand() % 125),255 };
		}

		CInt nTex = rand() % 2 + 2;

		Manager::EffectMgr()->ParticleCreate(GetParticleIdx((PARTI_TEX)nTex), Player.pos + Pos3D(0.0f,0.0f,0.0f), Vector3D(16.0f, 16.0f, 0.0f), setCol, CParticle::TYPE::TYPE_NORMAL, 300,D3DXVECTOR3(0.0f, 0.0f, (float)(rand() % 629 - 314) / 100.0f),D3DXVECTOR3(8.0f,8.0f,0.0f),false,false,_RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
	}
}

//*************************************************
//プロローグ処理
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, CInt nIdxPlayer)
{
	//次のインターバルへ
	if (s_nSwapInterval > 0 || nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_MIDDLE_INTERVAL;
	s_AnimState = SWAP_ANIM::MIDDLE;
}

//*************************************************
//中間処理
//*************************************************
void CPlayer::SwapAnim_Middle(Info& Player, CInt nIdxPlayer)
{
	//スワップ先へ移動
	Player.posOld.y = Player.pos.y;
	Player.pos.y += Player.fSwapMoveY;

	//次のインターバルへ
	if (s_nSwapInterval > 0) return;
	Player.posOld.y = Player.pos.y = Player.fSwapPosY;
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
void CPlayer::SwapAnim_Epilogue(Info& Player, CInt nIdxPlayer)
{
	//最後のプレイヤーのときにスワップアニメーション終了
	if (s_nSwapInterval > 0 || nIdxPlayer == 0) return;
	s_bSwapAnim = false;
	s_nSwapInterval = SWAP_INTERVAL;
	ms_bSwapEnd = (-156.0f <= m_aInfo[0].pos.x && 156.0f >= m_aInfo[0].pos.x && -156.0f <= m_aInfo[1].pos.x && 156.0f >= m_aInfo[1].pos.x);
	PlaySE(SE_LABEL::SWAPEND);

	if (Manager::StgEd()->GetPlanetIdx() == 0)
		if (Manager::StgEd()->GetType()[0].nStageIdx == 0)
			ms_guideCounter = 0;
}

//*************************************************
// ガイド表示
//*************************************************
void CPlayer::SwapGuide(Info& Player)
{
	// スワップ先のマークを描画する位置
	const Pos3D MarkPos = Pos3D(Player.pos.x, Player.pos.y * -1.0f, -8.0f);
	CFloat MarkSize = 16.0f;

	//マークのテクスチャＵＶ座標
	CFloat TexULeft = Player.idx * 0.5f;	CFloat TexURight = TexULeft + 0.5f;
	CFloat TexVOver = Player.side == WORLD_SIDE::BEHIND ? 0.0f : 1.0f;
	CFloat TexVUnder = 1.0f - TexVOver;

	//スワップ先のマーク描画
	if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
		RNLib::Polygon3D().Put(PRIORITY_EFFECT, MarkPos, INITD3DXVECTOR3)
			->SetSize(MarkSize, MarkSize)
			->SetBillboard(true)
			->SetZTest(false)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_MARK))
			->SetCol(Color{ 255, 255, 255, (int)Player.nSwapAlpha })
			->SetLighting(false)
			->SetTex(GetParticleIdx(PARTI_TEX::CHARACTER),
				Pos2D(TexULeft, TexVOver), Pos2D(TexURight, TexVOver),
				Pos2D(TexULeft, TexVUnder), Pos2D(TexURight, TexVUnder));
	}

	//スワップ先までの中心座標
	const Pos3D Center = Pos3D(Player.pos.x, 0.0f, MarkPos.z);
	CFloat BottomPosV = Player.fGuideTexVPos + Player.fGuideTexVSize;

	//ガイドサイズを設定
	CInt YDiff = -Player.pos.y * 200;
	CFloat fSize = (YDiff / (int)GUIDE_HEIGHT) / 100.0f;
	Player.fGuideTexVSize = fabsf(fSize);

	//ガイドのスピードを設定
	Player.fGuideMoveSpeed = fSize / 100.0f;
	RNLib::Number().Clamp(&Player.fGuideMoveSpeed, -MAX_GUIDE_SPEED, MAX_GUIDE_SPEED);//OK!
	Player.fGuideTexVPos += Player.fGuideMoveSpeed;

	if (Player.fGuideTexVPos >= Player.fGuideTexVSize)
		Player.fGuideTexVPos = 0.0f;

	//スワップガイドの描画
	if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
		RNLib::Polygon3D().Put(PRIORITY_EFFECT, Center, INITD3DXVECTOR3)
			->SetSize(GUIDE_WIDTH, (fabsf(Player.pos.y) - SIZE_HEIGHT) * 2.0f)
			->SetBillboard(true)
			->SetZTest(false)
			->SetCol(Color{ Player.color.r,Player.color.g,Player.color.b, (UShort)Player.nSwapAlpha })
			->SetLighting(false)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_GUIDE),
				Pos2D(0.0f, Player.fGuideTexVPos), Pos2D(1.0f, Player.fGuideTexVPos),
				Pos2D(0.0f, BottomPosV), Pos2D(1.0f, BottomPosV));
	}
}

//----------------------------
// 死亡処理
//----------------------------
void CPlayer::Death(Info& Player)
{
	if (Player.isDeath)
		return;

	Player.DeathType = (int)CEffect_Death::TYPE::INK;

	Player.isDeath = true;
	Player.expandCounter = EXPAND_TIME;
	Player.swapWaitCounter = 0;
	RNLib::Sound().Play(s_SE.expand, _RNC_Sound::CATEGORY::SE, 1.0f, false);
}

//----------------------------
// 移動処理
//----------------------------
void CPlayer::Move(VECTOL vec, int cntPlayer)
{
	if (m_aInfo[cntPlayer].isDeath) {
		return;
	}

	// プレイヤーの位置更新
	Info& Player = m_aInfo[cntPlayer];

	// ロケットに乗ってたら　or ゴールしていたらスキップ
	if (Player.bRide || Player.bGoal) return;

	// 移動量反映
	switch (vec)
	{
	case VECTOL::X:

		// 慣性処理
		Player.move.x += (0.0f - Player.move.x) * 0.12f;

		// Ⅹの移動量を修正
		RNLib::Number().Clamp(&Player.move.x, -MAX_MOVE_SPEED, MAX_MOVE_SPEED);//OK!
		
		// 位置更新
		Player.pos.x += Player.move.x;

		RNLib::Number().Clamp(&Player.pos.x, -360.0f, 360.0f);
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
}

//========================================
// 当たり判定処理
// Author:HIRASAWA SHION
// Arrange:RIKU NISHIMURA  OTONO KEISUKE
//========================================
void CPlayer::CollisionToStageObject(void)
{
	// 一旦両プレイヤーともにジャンプ不可
	m_aInfo[0].bGroundOld = m_aInfo[0].bGround;
	m_aInfo[1].bGroundOld = m_aInfo[1].bGround;
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++) {

		//死亡判定
		bool aDeath[2];
		aDeath[0] = false;
		aDeath[1] = false;

		// 衝突ベクトルに変換
		const VECTOL vec = (VECTOL)nCntVec;

		// プレイヤー毎に処理していく
		int nCntPlayer = NONEDATA;
		for each (Info & Player in m_aInfo) {
			nCntPlayer++;

			// 移動・位置制御処理
			Move(vec, nCntPlayer);

			// ロケットに乗ってるかゴールしていたら折り返す
			if (Player.bRide || Player.bGoal)
				continue;

			//自分の情報
			CCollision::SelfInfo Self;
			Self.pos = Player.pos;
			Self.posOld = Player.posOld;
			Self.move = Player.move;
			Self.fWidth = Self.fHeight = SIZE_HEIGHT;

			// オブジェクト1つ1つを見ていく
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {

				// 取得したオブジェクトをキャスト
				CStageObject* pObj = (CStageObject*)obj;

				// 種類取得
				const OBJECT_TYPE type = pObj->GetType();

				// オブジェクトの当たり判定情報を設定
				CCollision::ColliInfo colliInfo;
				colliInfo.pos = pObj->GetPos();
				colliInfo.posOld = colliInfo.pos;
				colliInfo.fWidth = pObj->GetWidth() * 0.5f;
				colliInfo.fHeight = pObj->GetHeight() * 0.5f;
				colliInfo.vec = nCntVec;

				// プレイヤーの近くにオブジェクトがあるか判定
				// ※特定オブジェクトを除く
				if (type != OBJECT_TYPE::TRAMPOLINE && type != OBJECT_TYPE::LASER &&
					type != OBJECT_TYPE::EXTEND_DOG && type != OBJECT_TYPE::PILE && 
					type != OBJECT_TYPE::ROCKET && CCollision::IsInRange(Self, colliInfo, true, true, false, false, s_nSwapInterval))
					continue;

				//独自の当たり判定設定
				//場合によってはここで判定終了
				if (!UniqueColliOpption(pObj, type, Player, &colliInfo.pos, &colliInfo.posOld, &colliInfo.fWidth, &colliInfo.fHeight)) continue;

				// 当たった方向を格納
				colliInfo.Rot = CCollision::IsBoxToBoxCollider(Self, colliInfo, vec);
				CInt nColliRot = (int)colliInfo.Rot;

				// 当たっていない
				if (colliInfo.Rot == CCollision::ROT::NONE)
				{
					//杭なら乗っていない
					if (type == OBJECT_TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				// 死亡しているか保存
				const bool deathOld = aDeath[nCntPlayer];

				// 種類ごとに関数分け
				switch (type)
				{
				case OBJECT_TYPE::BLOCK:     CCollision::Block(&Self, &colliInfo, Player, (CBlock*)pObj, &Player.side, &aDeath[nCntPlayer]);break;
				case OBJECT_TYPE::TRAMPOLINE:CCollision::Trampoline(&Self, &colliInfo, (CTrampoline*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::FILLBLOCK: CCollision::FillBlock(&Self, colliInfo.Rot, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::LAVA_BLOCK:aDeath[nCntPlayer] = true;	break;
				case OBJECT_TYPE::SPIKE:     CCollision::Spike(&Self, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::MOVE_BLOCK:CCollision::MoveBlock(&Self, (CMoveBlock*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::METEOR:    CCollision::Meteor(&Self, &colliInfo, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::LASER:     CCollision::Laser(&Self, (CRoadTripLaser*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::EXTEND_DOG:CCollision::Dog(&Self, (CExtenddog*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::GOALGATE:  CCollision::GoalGate(&Self, &colliInfo, obj, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::PARTS:     CCollision::Parts(&Self, (CParts*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::ROCKET:    CCollision::Rocket(&Self, (CRocket*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::PILE:      CCollision::Pile(&Self, &colliInfo, (CPile*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				}		

				//情報代入
				if (vec == VECTOL::X) {
					Player.pos.x = Self.pos.x;
					Player.move.x = Self.move.x;
				}
				else {
					Player.pos.y = Self.pos.y;
					Player.move.y = Self.move.y;
				}

				//当たり判定の事後処理
				CollisionAfter(pObj, type, &nColliRot, Player);
			}
		}

		// 死亡判定ON
		if ((aDeath[0] || aDeath[1]) && (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath)) {
			if (aDeath[0])
				Death(m_aInfo[0]);

			if (aDeath[1])
				Death(m_aInfo[1]);
		}
	}
}

//----------------------------
//独自のオブジェクトの当たり判定設定
//----------------------------
bool CPlayer::UniqueColliOpption(CStageObject *pObj, const OBJECT_TYPE type, Info &Player, Pos3D *pPos, Pos3D *pPosOld, float *pWidth, float *pHeight)
{
	switch (type) {
		// ブロック
		case OBJECT_TYPE::BLOCK: {
			CBlock* pBlock = (CBlock*)pObj;

			if (!pBlock->GetCollision())
				return false;

		}break;

			// ゴールゲート
		case OBJECT_TYPE::GOALGATE: {
			CGoalGate* pGoalGate = (CGoalGate*)pObj;
			if (pGoalGate->GetStartGate())
				//当たり判定終了
				return false;
		}break;

			// 移動床
		case OBJECT_TYPE::MOVE_BLOCK: {
			if (pPosOld != NULL)
			{
				CMoveBlock* pBlock = (CMoveBlock*)pObj;
				*pPosOld = pBlock->GetPosOld();
			}
		}break;

			// 隕石
		case OBJECT_TYPE::METEOR: {
			if (pPosOld != NULL)
			{
				CMeteor* pBlock = (CMeteor*)pObj;
				*pPosOld = pBlock->GetPosOld();
			}
		}break;

			// レーザー
		case OBJECT_TYPE::LASER: {
			CRoadTripLaser* pLaser = (CRoadTripLaser*)pObj;

			if (pPos    != NULL)*pPos    = pLaser->GetLaserPos();
			if (pPosOld != NULL)*pPosOld = pLaser->GetPosOld();
			if (pWidth  != NULL)*pWidth  = pLaser->GetLaserSize().x * 0.5f;
			if (pHeight != NULL)*pHeight = pLaser->GetLaserSize().y * 0.5f;
		}break;

			// ヌイ
		case OBJECT_TYPE::EXTEND_DOG: {
			CExtenddog *pDog = (CExtenddog *)pObj;
			if (pPos    != NULL)*pPos    = pDog->GetBodyPos();
			if (pPosOld != NULL)*pPosOld = pDog->GetBodyPosOld();
			if (pHeight != NULL)*pHeight = pDog->GetColliHeight();
		}break;

			//杭
		case OBJECT_TYPE::PILE:
		{
			if (pPos != NULL && pPosOld != NULL && pHeight != NULL)
			{
				CPile* pPile = (CPile*)pObj;
				pPile->GetCollisionInfo(*pPos, *pPosOld, *pHeight);

				if (D3DXVec3Length(&(*pPos - Player.pos)) >
					D3DXVec2Length(&D3DXVECTOR2(*pWidth + SIZE_WIDTH, *pHeight + SIZE_HEIGHT)))
					//当たり判定終了
					return false;
			}
		}break;
	}

	//当たり判定続行
	return true;
}

//----------------------------
// 各プレイヤーの当たり判定が終わった後の処理
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, CInt *pColliRot, Info& info)
{
	// 種類ごとに関数分け
	switch (type)
	{
		//ブロックのリアクションフラグ
		case OBJECT_TYPE::BLOCK:
		{
			typedef CCollision::ROT ColRot;		 //衝突方向の別名
			Info *pInfo = &m_aInfo[0];			 //プレイヤー情報のポインタ
			CBlock *pBlock = (CBlock *)pStageObj;//ブロックへキャスト

			for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++, pInfo++)
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
		case OBJECT_TYPE::EXTEND_DOG:
		{
			//ヌイに変換
			CExtenddog *pDog = (CExtenddog *)pStageObj;

			//お尻の方向と当たった方向が同じ
			if (pDog->GetHipRot() == *pColliRot) {
				pDog->SetState(CExtenddog::STATE::RETURN);
				
				if (!info.bGroundOld) {
					RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::DOG_00], _RNC_Sound::CATEGORY::SE, 1.0f, false);
				}
			}
			break;
		}

		//杭に乗っているプレイヤー
		case OBJECT_TYPE::PILE:
		{
			//杭の判定情報取得
			CPile *pPile = (CPile *)pStageObj;
			Pos3D Pos, null;float height;
			pPile->GetCollisionInfo(Pos, null, height);

			for each (Info &Player in m_aInfo)
			{
				if(!Player.bLandPile) continue;

				switch (Player.side)
				{
					case WORLD_SIDE::FACE:
					{
						CFloat LandPos = Pos.y + height + SIZE_HEIGHT;
						if (Player.pos.y < LandPos) Player.pos.y = LandPos;
					}break;

					case WORLD_SIDE::BEHIND:
					{
						CFloat LandPos = Pos.y - height - SIZE_HEIGHT;
						if (Player.pos.y > LandPos) Player.pos.y = LandPos;
					}break;
				}

				Player.move.y = 0.0f;

				//次の杭で判定しないよう初期化
				Player.bLandPile = false;
			}
			break;
		}

		//ゴールゲート
		case OBJECT_TYPE::GOALGATE:
		{
			if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
			{
				//計測終了
				CMode_Game::SetMeasureTime(false);
				s_nGoalInterval = -GOAL_INTERVAL;
			}
		}

		//ロケット
		case OBJECT_TYPE::ROCKET:
		{
			if (m_aInfo[0].bRide && m_aInfo[1].bRide)
			{
				//計測終了
				CMode_Game::SetMeasureTime(false);
				s_nGoalInterval = -GOAL_INTERVAL;
			}
		}
	}
}

//----------------------------
// プレイヤー情報設定
// 指定された番号のプレイヤー情報を設定します。
//----------------------------
void CPlayer::SetInfo(const Info info, CInt nNum)
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
	CFloat diff = -fMaxHeight - pInfo->pos.y;
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
bool CPlayer::IsKeyConfigTrigger(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// どちらかのプレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigTrigger(KEY_CONFIG KeyConfig)
{
	return 
		RNLib::Input().GetTrigger(m_aInfo[0].Keyborad[(int)m_aInfo[0].side][(int)KeyConfig], m_aInfo[0].JoyPad[(int)KeyConfig], 0) ||
		RNLib::Input().GetTrigger(m_aInfo[1].Keyborad[(int)m_aInfo[1].side][(int)KeyConfig], m_aInfo[1].JoyPad[(int)KeyConfig], 1);
}

//----------------------------
// プレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigPress(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// どちらかのプレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigPress(KEY_CONFIG KeyConfig)
{
	return
		RNLib::Input().GetPress(m_aInfo[0].Keyborad[(int)m_aInfo[0].side][(int)KeyConfig], m_aInfo[0].JoyPad[(int)KeyConfig], 0) ||
		RNLib::Input().GetPress(m_aInfo[1].Keyborad[(int)m_aInfo[1].side][(int)KeyConfig], m_aInfo[1].JoyPad[(int)KeyConfig], 1);
}

//----------------------------
//プレイヤー用SE再生
//----------------------------
void CPlayer::PlaySE(SE_LABEL label)
{
	switch (label)
	{
		case CPlayer::SE_LABEL::JUMP:   s_SE.pSound->Play(s_SE.jump,    _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_00: s_SE.pSound->Play(s_SE.dog[0],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_01: s_SE.pSound->Play(s_SE.dog[1],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_02: s_SE.pSound->Play(s_SE.dog[2],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_03: s_SE.pSound->Play(s_SE.dog[3],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPING:s_SE.pSound->Play(s_SE.Swaping, _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPEND:s_SE.pSound->Play(s_SE.SwapEnd, _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
	}
}

//----------------------------
//ゴール後の演出
//----------------------------
void CPlayer::GoalDirector(void)
{
	//時間加算
	s_nGoalInterval++;

	//クリアタイム取得
	static float BestTime;
	CFloat ClearTime = CMode_Game::GetPlayTime();
	CStageEditor *pEd = Manager::StgEd();
	CInt planet = pEd->GetPlanetIdx();
	CInt stage = pEd->GetType()[planet].nStageIdx;

	if (IsKeyConfigTrigger(KEY_CONFIG::DECIDE))
	{
		//次の演出時間に設定
		if (s_nGoalInterval < POP_CLEARTIME)
		{
			s_nGoalInterval = POP_CLEARTIME;
			BestTime = Stage::GetBestTime(planet, stage);
		}
		else if (s_nGoalInterval < GOAL_INTERVAL)
			s_nGoalInterval = GOAL_INTERVAL;

		//次のステージへ
		else
		{
			CCoin::AddNumAll();
			pEd->SwapStage(stage + 1);
			Stage::RegistTime(planet, stage, ClearTime);
			Stage::SetIsCutIn(false);
		}
	}
	
	if (s_nGoalInterval < 0) return;

	Stage::SetIsCutIn(true);
	Stage::SetIsGoal(true);

	const Pos2D Center = RNLib::Window().GetCenterPos();
	const Pos2D Size = RNLib::Window().GetSize();

	//画面を暗くする
	RNLib::Polygon2D().Put(PRIORITY_UI, Center, 0.0f)
		->SetSize(Size.x, Size.y)
		->SetCol(Color{ 0,0,0,150 });

	//ベストタイム取得
	if(s_nGoalInterval < POP_CLEARTIME) 
		BestTime = Stage::GetBestTime(planet, stage);

	//クリアタイム表示
	if (s_nGoalInterval >= POP_CLEARTIME)
	{
		if(ClearTime < BestTime)
			RNLib::Text2D().Put(PRIORITY_UI, String("New Record!!"), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(100.0f, 130.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f))
			->SetCol(Color{ 255,255,0,255 });

		RNLib::Text2D().Put(PRIORITY_UI, String("ベストタイム:%.1f秒", BestTime), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(100.0f, 160.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f));

		const Pos2D PopPos = Center + Pos2D(0.0f, 200.0f);
		RNLib::Text2D().Put(PRIORITY_UI, String("クリアタイム:%.1f秒", ClearTime), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(0.0f, 200.0f), 0.0f)
			->SetSize(Size2D(50.0f, 50.0f));
	}

	// 次のステージへ
	if (s_nGoalInterval >= GOAL_INTERVAL)
	{
		//ゴールインターバルからの超過時間
		CInt DiffInterval = s_nGoalInterval - GOAL_INTERVAL;

		//インターバル前半は表示
		if (DiffInterval < GOAL_INTERVAL / 2)
			RNLib::Text2D().Put(PRIORITY_UI, "NextStage: A ボタン or Enter", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Pos2D(Center.x + 100.0f, 600.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f));

		if (s_nGoalInterval >= GOAL_INTERVAL * 2)
			s_nGoalInterval = GOAL_INTERVAL;
	}

	RNLib::Text2D().PutDebugLog(String("ゴールインターバル:%d", s_nGoalInterval));
}