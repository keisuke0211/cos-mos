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

// スワップインターバル
const int	CPlayer::SWAP_INTERVAL = 20;	// スワップインターバル
const float CPlayer::GUIDE_WIDTH   = 10.0f; // ガイドの幅
const float CPlayer::GUIDE_HEIGHT  = 14.0f; // ガイドの高さ

int                CPlayer::s_nSwapInterval = 0; // 残りスワップインターバル
bool               CPlayer::s_bSwapAnim = false; //スワップアニメーション中かどうか
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//アニメーション構成

int CPlayer::s_nGoalInterval = 0;//ゴール後の余韻カウンター
int CPlayer::s_zoomUpCounter = 0;// ズームアップカウンター

const float CPlayer::SIZE_WIDTH = 7.0f; // 横幅
const float CPlayer::SIZE_HEIGHT = 8.0f;// 高さ

const float CPlayer::MOVE_SPEED = 0.3f;     // 移動量
const float CPlayer::MAX_MOVE_SPEED = 2.3f; // 最大移動量

const float CPlayer::JUMP_POWER = 5.0f;     // 基本ジャンプ量
const float CPlayer::GRAVITY_POWER = -0.3f; // 基本重力加速度

const int CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// スワップマーク
	"data\\TEXTURE\\Effect\\eff_Star_000.png",  // スワップパーティクル00
	"data\\TEXTURE\\Effect\\ink001.png",        // スワップパーティクル01
	"data\\TEXTURE\\Effect\\ink002.png",        // スワップパーティクル02
	"data\\TEXTURE\\Effect\\swap_guide.png",    // スワップガイド
	"data\\TEXTURE\\player.PNG",                // プレイヤーアイコン
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// 死亡マーク
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // 死亡パーティクル
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // ゴール・ロケット乗車時のエフェクト
};
int CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE     CPlayer::s_SE = {};	//サウンド用構造体
CPlayer::Motion CPlayer::s_motion[2] = {};	//モーション用構造体
CCollision     *CPlayer::s_pColli = NULL;
bool            CPlayer::ms_bSwapEnd = false;
UShort          CPlayer::ms_guideCounter = 0;

bool  CPlayer::s_bAimPlayer = false;
int   CPlayer::s_nAimNo = 0;
float CPlayer::s_fCorrWidth = 0.0f;
float CPlayer::s_fCorrHeight = 0.0f;
float CPlayer::s_fAimWorkSpeed = 0.05f;

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
		Player.StartPos = INITD3DXVECTOR3;     // 開始位置
		Player.doll = NULL;
		Player.pos = INITD3DXVECTOR3;          // 位置
		Player.posOld = INITD3DXVECTOR3;       // 前回位置
		Player.rot = INITD3DXVECTOR3;          // 向き
		Player.move = INITD3DXVECTOR3;         // 移動量
		Player.color = INITCOLOR;              // 色
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA; // スワップマークのα値
		Player.fSwapPosY = 0.0f;               // スワップ先のＹ座標
		Player.fSwapMoveY = 0.0f;              // スワップ移動時の速度
		Player.fGuideTexVPos = 0.0f;           // ガイドのテクスチャＶ座標
		Player.fGuideTexVSize = 0.0f;          // ガイドのテクスチャＶサイズ
		Player.fGuideMoveSpeed = 0.0f;         // ガイドのテクスチャ移動スピード
		Player.bGround = true;                // 地面に接しているか
		Player.bGroundOld = true;             // 地面に接しているか(過去)
		Player.landingCounter = false;
		Player.bJump = false;                  // ジャンプ
		Player.bRide = false;                  // ロケットに乗っているかどうか
		Player.bGoal = false;                  // ゴールしたかどうか
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
	ms_bSwapEnd = false;
	ms_guideCounter = 0;

	// １Ｐ初期情報
	if (m_aInfo[0].doll != NULL)
		delete m_aInfo[0].doll;
	m_aInfo[0].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"));
	m_aInfo[0].rot = Rot3D(0.0f, D3DX_PI, 0.0f);
	m_aInfo[0].color = Color{255, 155, 59, (int)m_aInfo[0].nSwapAlpha };

	// ２Ｐ初期情報
	if (m_aInfo[1].doll != NULL)
		delete m_aInfo[1].doll;
	m_aInfo[1].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Eye.txt"));
	m_aInfo[1].rot = CStageObject::INVERSEVECTOR3;
	m_aInfo[1].color = Color{65, 233, 210, (int)m_aInfo[1].nSwapAlpha };

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
	s_SE.dog[0]	= s_SE.pSound->Load("data\\SOUND\\SE\\heron2.wav");		// 押す
	s_SE.dog[1]	= s_SE.pSound->Load("data\\SOUND\\SE\\shrink.wav");		// 縮む
	s_SE.dog[2]	= s_SE.pSound->Load("data\\SOUND\\SE\\extend.wav");		// 伸びる
	s_SE.dog[3]	= s_SE.pSound->Load("data\\SOUND\\SE\\vibration.wav");	// 震える
	s_SE.Swaping = s_SE.pSound->Load("data\\SOUND\\SE\\swap_now.wav");
	s_SE.SwapEnd = s_SE.pSound->Load("data\\SOUND\\SE\\swap_end.wav");
	s_SE.expand = s_SE.pSound->Load("data\\SOUND\\SE\\death_expand.wav");
	s_SE.explosion = s_SE.pSound->Load("data\\SOUND\\SE\\death_explosion.wav");

	for (int cnt = 0; cnt < 2; cnt++) {
		if (cnt == 0) {
			s_motion[cnt].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Default.txt");
			s_motion[cnt].walk    = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Walk.txt");
			s_motion[cnt].jump    = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Jump.txt");
			s_motion[cnt].fall    = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Fall.txt");
			s_motion[cnt].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Landing.txt");
		}
		else {
			s_motion[cnt].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Default.txt");
			s_motion[cnt].walk    = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Walk.txt");
			s_motion[cnt].jump    = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Jump.txt");
			s_motion[cnt].fall    = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Fall.txt");
			s_motion[cnt].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Landing.txt");
		}
	}

	InitInfo();

	// 初期値設定
	// ※ 来れないとステージ入る前に一瞬着地SEがなる
	m_aInfo[0].bJump = m_aInfo[1].bJump = false;

	if(s_pColli == NULL)
	s_pColli = new CCollision;

	s_zoomUpCounter = ZOOM_UP_TIME;

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
		Player.isDeath = false;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.swapWaitBalloonCounter = 0;
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
	RNLib::Text2D().PutDebugLog(CreateText("インターバル:%d", s_nSwapInterval));

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
	if (!m_aInfo[0].bGoal && !m_aInfo[1].bGoal)
	{
		// スワップ
		Swap();
	}
	else if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
	{
		if (++s_nGoalInterval >= GOAL_INTERVAL)
		{
			CCoin::AddNumAll();
			const int planet = Manager::StgEd()->GetPlanetIdx();
			const int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
			Manager::StgEd()->SwapStage(stage + 1);
		}
	}

	// 当たり判定まとめ
	CollisionToStageObject();

	// 情報更新
	UpdateInfo();
}

//----------------------------
// 情報更新処理
//----------------------------
void CPlayer::UpdateInfo(void)
{
	//----------------------------------------
	// ガイドテキスト
	//----------------------------------------
	bool isSwapGuide = false;
	if (!s_bSwapAnim) {
		if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath) {
			const int planet = Manager::StgEd()->GetPlanetIdx();

			if (planet == 0) {
				if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
					if (s_zoomUpCounter == 0) {
						if (++ms_guideCounter > 30)
							ms_guideCounter = 30;
						float rate = (float)ms_guideCounter / 30;
						if (ms_bSwapEnd) {

							if (ms_guideCounter == 1) {
								RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], CSound::CATEGORY::SE, 1.0f, false);
							}
							RNLib::Text3D().Put(PRIORITY_UI, "OK!", CText::ALIGNMENT::CENTER, 0, INITMATRIX)
								->SetSize(Size2D(32.0f * rate, 32.0f * rate))
								->SetZTest(false)
								->SetBillboard(true);
						}
						else {
							if (s_nSwapInterval == 0) {
								RNLib::Text3D().Put(PRIORITY_UI, "SWAPしてみよう!", CText::ALIGNMENT::CENTER, 0, INITMATRIX)
									->SetSize(Size2D(24.0f * rate, 24.0f * rate))
									->SetZTest(false)
									->SetBillboard(true);
								isSwapGuide = true;
							}
						}
					}
				}
				else if (Manager::StgEd()->GetType()[0].nStageIdx == 3) {
					if (++ms_guideCounter > 30)
						ms_guideCounter = 30;
					float rate = (float)ms_guideCounter / 30;
					if (CParts::GetDispNum() == 0) {
						if (ms_guideCounter == 1) {
							RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], CSound::CATEGORY::SE, 1.0f, false);
						}
						RNLib::Text3D().Put(PRIORITY_UI, "OK!", CText::ALIGNMENT::CENTER, 0, INITMATRIX)
							->SetSize(Size2D(32.0f * rate, 32.0f * rate))
							->SetZTest(false)
							->SetBillboard(true);
					}
					else {
						RNLib::Text3D().Put(PRIORITY_UI, "ロケットのパーツをあつめて!", CText::ALIGNMENT::CENTER, 0, INITMATRIX)
							->SetSize(Size2D(24.0f * rate, 24.0f * rate))
							->SetZTest(false)
							->SetBillboard(true);
					}
				}
			}
		}
	}

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
				const int count = RNLib::Count().GetCount() % 40;
				float sizeRate = (float)(count - ((count % 20) * (count / 20)) * 2) / 20;
				RNLib::Text3D().Put(PRIORITY_UI, "X", CText::ALIGNMENT::CENTER, 0, CMatrix::ConvPosToMtx(putPos))
					->SetSize(Size2D(16.0f * (0.5f + sizeRate * 0.25f), 16.0f * (0.5f + sizeRate * 0.25f)))
					->SetZTest(false)
					->SetBillboard(true);
			}
		}

		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal)
			continue;

		if(!Player.isDeath){
			// スワップ先のマークを描画する位置
			D3DXVECTOR3 MarkPos = Player.pos;
			MarkPos.y *= -1.0f;
			MarkPos.z = -8.0f;

			//マークのテクスチャＵＶ座標
			const float TexULeft = nCntPlayer * 0.5f;
			const float TexURight = TexULeft + 0.5f;
			const float TexVOver = Player.side == WORLD_SIDE::BEHIND ? 0.0f : 1.0f;
			const float TexVUnder = 1.0f - TexVOver;

			//スワップ先のマーク描画
			RNLib::Polygon3D().Put(PRIORITY_EFFECT, MarkPos, INITD3DXVECTOR3)
				->SetSize(16.0f, 16.0f)
				->SetBillboard(true)
				->SetZTest(false)
				->SetTex(GetParticleIdx(PARTI_TEX::SWAP_MARK))
				->SetCol(Color{ 255, 255, 255, (int)Player.nSwapAlpha })
				->SetLighting(false)
				->SetTexUV(GetParticleIdx(PARTI_TEX::CHARACTER),
						   Pos2D(TexULeft, TexVOver), Pos2D(TexURight, TexVOver),
						   Pos2D(TexULeft, TexVUnder), Pos2D(TexURight, TexVUnder));

			//スワップ先までの中心座標
			const Pos3D Center = Pos3D(Player.pos.x, 0.0f, MarkPos.z);
			const float BottomPosV = Player.fGuideTexVPos + Player.fGuideTexVSize;

			//ガイドサイズを設定
			const int YDiff = (-Player.pos.y - Player.pos.y) * 100;
			const float fSize = (YDiff / (int)GUIDE_HEIGHT) / 100.0f;
			Player.fGuideTexVSize = fabsf(fSize);

			//ガイドのスピードを設定
			Player.fGuideMoveSpeed = fSize / 100.0f;
			Player.fGuideTexVPos += Player.fGuideMoveSpeed;

			if (Player.fGuideTexVPos >= Player.fGuideTexVSize)
				Player.fGuideTexVPos = 0.0f;

			//スワップガイドの描画
			RNLib::Polygon3D().Put(PRIORITY_EFFECT, Center, INITD3DXVECTOR3)
				->SetSize(GUIDE_WIDTH, fabsf(Player.pos.y) * 1.5f)
				->SetBillboard(true)
				->SetZTest(false)
				->SetCol(Color{ Player.color.r,Player.color.g,Player.color.b, (UShort)Player.nSwapAlpha })
				->SetLighting(false)
				->SetTexUV(GetParticleIdx(PARTI_TEX::SWAP_GUIDE),
						   Pos2D(0.0f, Player.fGuideTexVPos), Pos2D(1.0f, Player.fGuideTexVPos),
						   Pos2D(0.0f, BottomPosV), Pos2D(1.0f, BottomPosV));
		}

		// 最高Ｙ座標更新
		switch (Player.side) {
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
		RNLib::Text2D().PutDebugLog(CreateText("%dP最高Y座標：%f    Y:%f X:%f", nCntPlayer + 1, Player.fMaxHeight, Player.pos.y, Player.pos.x));
	}
}

//----------------------------
// 死亡更新処理
//----------------------------
void CPlayer::UpdateDeath(Info& info, const int& count) {

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
				CStageObject* stageObj = (CStageObject*)obj;

				switch (stageObj->GetType())
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
		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, counter, DEATH_TIME2 / 2);
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

		addVec += CGeometry::GetRandomVec();
		Manager::GetMainCamera()->SetMotionBlurPower(isReturn ? 0.25f + rate * 0.25f : rate * 0.5f);
		Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.02f));
		Manager::GetMainCamera()->SetPosVAndPosR(
			((Manager::GetMainCamera()->GetPosV() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posVTemp * (1.0f - rate2)),
			((Manager::GetMainCamera()->GetPosR() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posRTemp * (1.0f - rate2)));
	}
	// 膨らみカウンター＆演出
	else if (info.expandCounter > 0) {
		if (--info.expandCounter == 0) {
			RNLib::Sound().Play(s_SE.explosion, CSound::CATEGORY::SE, 1.0f, false);
			Manager::GetMainCamera()->SetVib(5.0f);

			const int NUM_PARTICLE = 8;
			Pos3D rot = INITVECTOR3D;
			for (int ParCnt = 0; ParCnt < NUM_PARTICLE; ParCnt++)
			{
				rot.z = -D3DX_PI + D3DX_PI_DOUBLE * fRand();
				CEffect_Death* pEff = Manager::EffectMgr()->DeathParticleCreate(NONEDATA, info.pos, INITVECTOR3D, rot, INITVECTOR3D, 0.0f, Color{ 255, 155, 59,255 }, CEffect_Death::TYPE::BALL);

				const CEffect_Death::BALL_SIZE_LV Lv = (CEffect_Death::BALL_SIZE_LV)(rand() % (int)(CEffect_Death::BALL_SIZE_LV::MAX));
				pEff->SetBallSize(Lv);
			}
			info.deathCounter = DEATH_TIME;
		}
		info.scale.x =
		info.scale.y =
		info.scale.z = 1.0f + (1.0f - CEase::Easing(CEase::TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 0.2f;
		info.pos.z = -(1.0f - CEase::Easing(CEase::TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 8.0f;
	}
}

//----------------------------
// 操作処理
//----------------------------
void CPlayer::ActionControl(void)
{
	// プレイヤー番号
	int nIdxPlayer = -1;

	// [[[ カメラ制御 ]]]
	if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath) 
	{// どちらも死んでいない
		bool isZoomUp = false;
		if (s_zoomUpCounter > 0) {
			if (Manager::StgEd()->GetPlanetIdx() == 0) {
				if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
					isZoomUp = true;
				}
			}
		}

		Pos3D targetPosV = Manager::StgEd()->GetCameraPos();
		Pos3D targetPosR = (m_aInfo[0].pos + m_aInfo[1].pos) * 0.5f;
		targetPosR.x *= 0.25f;
		targetPosR.y = 0.0f;

		if (isZoomUp) {
			s_zoomUpCounter--;

			Pos3D basePosRMain = m_aInfo[0].pos + Pos3D(0.0f, -16.0f, 0.0f);
			Pos3D basePosRSub  = m_aInfo[1].pos + Pos3D(0.0f, 16.0f, 0.0f);
			Pos3D basePosVMain = basePosRMain + Pos3D(0.0f, 0.0f, -100.0f);
			Pos3D basePosVSub  = basePosRSub  + Pos3D(0.0f, 0.0f, -100.0f);
			float rate = 1.0f;
			if (s_zoomUpCounter < 60) {
				rate = CEase::Easing(CEase::TYPE::IN_SINE, s_zoomUpCounter, 60);
			}
			float rateOpp = 1.0f - rate;
			Manager::GetMainCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVMain * rate, targetPosR * rateOpp + basePosRMain * rate);
			Manager::GetSubCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVSub * rate, targetPosR * rateOpp + basePosRSub * rate);
			return;
		}
		else{
			CInput *pInput = &RNLib::Input();
			if (pInput->GetKeyTrigger(DIK_B))
			{//視点切替
				if (!s_bAimPlayer)
					s_bAimPlayer = !s_bAimPlayer;
				else if (s_nAimNo == 0)
					s_nAimNo = 1;
				else
					s_bAimPlayer = false;
			}

			if (s_bAimPlayer)
			{
				if (pInput->GetKeyTrigger(DIK_V)) s_fCorrWidth -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_N)) s_fCorrWidth += 0.1f;
				if (pInput->GetKeyTrigger(DIK_G)) s_fCorrHeight -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_H)) s_fCorrHeight += 0.1f;

				targetPosV.x = targetPosR.x = m_aInfo[s_nAimNo].pos.x + s_fCorrWidth;
				targetPosV.y = targetPosR.y = m_aInfo[s_nAimNo].pos.y + s_fCorrHeight;
				targetPosV.z =- 100.0f;

				//本来の当たり判定範囲
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,255,255,255 })
					->SetSize(SIZE_WIDTH * 2.0f, SIZE_HEIGHT * 2.0f);

				//半分の当たり判定範囲
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,100,100,255 })
					->SetSize(SIZE_WIDTH * 1.0f, SIZE_HEIGHT * 1.0f);

				RNLib::Text2D().PutDebugLog(CreateText("横の調整量:%f  縦の調整量:%f", s_fCorrWidth, s_fCorrHeight));
			}

			Manager::GetMainCamera()->SetPosVAndPosR(targetPosV, targetPosR);
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
			CGoalGate::EntrySub();
			CRocket::RideOff();
			Player.bRide = false;
			Player.bGoal = false;
			Player.move.x *= -2.0f;
		}

		// ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		// ジャンプ入力（空中じゃない）
		if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bGround = false;            // 地面から離れた
			Player.move.y = Player.fJumpPower; // ジャンプ量代入
			Player.bJump = true;               // ジャンプした
			PlaySE(SE_LABEL::JUMP);            // SE再生
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
				Player.doll->OverwriteMotion(s_motion[nIdxPlayer].fall);
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

		// スワップ入力
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP)) {
			Player.nSwapAlpha = 255;
			if (++Player.swapWaitBalloonCounter > SWAP_WAIT_BALLOON_TIME)
				Player.swapWaitBalloonCounter = SWAP_WAIT_BALLOON_TIME;
		}
		//スワップ非入力
		else {
			Player.nSwapAlpha = NORMAL_SWAP_ALPHA;
			if (--Player.swapWaitBalloonCounter < 0)
				Player.swapWaitBalloonCounter = 0;
		}

		{// 吹き出しの表示
			Pos3D putPos = Player.pos;
			putPos.y += GetPlusMinus(Player.pos.y) * 8.0f;
			CPolygon3D::CRegistInfo* polygon3D = RNLib::Polygon3D().Put(PRIORITY_UI, putPos, Rot3D(0.0f,0.0f, -0.1f + (CEase::Easing(CEase::TYPE::INOUT_SINE, GetTurnNum(RNLib::Count().GetCount(), 30), 30)) * 0.2f))
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_WAITBUBBLE], Player.pos.y < 0.0f, 2, 1)
				->SetCol(Color(255, 255, 255, 255 * ((float)Player.swapWaitBalloonCounter / SWAP_WAIT_BALLOON_TIME)))
				->SetZTest(false);
			
			Size2D size = Size2D(16.0f, 32.0f * ((float)Player.swapWaitBalloonCounter / SWAP_WAIT_BALLOON_TIME));
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
			Player.swapWaitBalloonCounter = 0;
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

	RNLib::Polygon2D().Put(PRIORITY_SWAP_CURTAIN)
		->SetPos(INITPOS2D)
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

		const int nTex = rand() % 2 + 2;

		Manager::EffectMgr()->ParticleCreate(GetParticleIdx((PARTI_TEX)nTex), Player.pos, Vector3D(16.0f, 16.0f, 0.0f), setCol, CParticle::TYPE::TYPE_NORMAL, 300, D3DXVECTOR3(0.0f, 0.0f, (float)(rand() % 629 - 314) / 100.0f), 8, CDrawState::ALPHA_BLEND_MODE::NORMAL);
	}
}

//*************************************************
//プロローグ処理
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, const int nIdxPlayer)
{
	//次のインターバルへ
	if (s_nSwapInterval > 0 || nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_MIDDLE_INTERVAL;
	s_AnimState = SWAP_ANIM::MIDDLE;
}

//*************************************************
//中間処理
//*************************************************
void CPlayer::SwapAnim_Middle(Info& Player, const int nIdxPlayer)
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
void CPlayer::SwapAnim_Epilogue(Info& Player, const int nIdxPlayer)
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

//----------------------------
// 死亡処理
//----------------------------
void CPlayer::Death(Info& Player, const OBJECT_TYPE type, const int *pColliRot)
{
	if (Player.isDeath)
		return;

	switch (type)
	{
		case OBJECT_TYPE::BLOCK:
		case OBJECT_TYPE::FILLBLOCK:
		case OBJECT_TYPE::TRAMPOLINE:
		case OBJECT_TYPE::MOVE_BLOCK:
		case OBJECT_TYPE::EXTEND_DOG:
		case OBJECT_TYPE::GOALGATE:
		case OBJECT_TYPE::PARTS:
		case OBJECT_TYPE::ROCKET:
		case OBJECT_TYPE::PILE:
			if (s_nSwapInterval == 0) return;
			break;
	}

	Player.isDeath = true;
	Player.expandCounter = EXPAND_TIME;
	RNLib::Sound().Play(s_SE.expand, CSound::CATEGORY::SE, 1.0f, false);
}

//----------------------------
// 移動処理
//----------------------------
void CPlayer::Move(VECTOL vec, int cntPlayer)
{
	if (m_aInfo[0].isDeath || m_aInfo[1].isDeath) {
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
		// 過去の位置を設定
		Player.posOld.x = Player.pos.x;

		// 慣性処理
		Player.move.x += (0.0f - Player.move.x) * 0.12f;

		// Ⅹの移動量を修正
		if(s_bAimPlayer && s_nAimNo == cntPlayer)
			FloatControl(&Player.move.x, s_fAimWorkSpeed, -s_fAimWorkSpeed);
		else
			FloatControl(&Player.move.x, MAX_MOVE_SPEED, -MAX_MOVE_SPEED);

		// 位置更新
		Player.pos.x += Player.move.x;
		break;

		// 重力処理
	case VECTOL::Y:
		// 過去の位置を設定
		Player.posOld.y = Player.pos.y;

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

				int nColliRot[NUM_PLAYER] = {};

				// 取得したオブジェクトをキャスト
				CStageObject* stageObj = (CStageObject*)obj;

				// 種類取得
				const OBJECT_TYPE type = stageObj->GetType();

				// オブジェクトの当たり判定情報を設定
				CCollision::ColliInfo colliInfo;
				colliInfo.pos = stageObj->GetPos();
				colliInfo.posOld = colliInfo.pos;
				colliInfo.fWidth = stageObj->GetWidth() * 0.5f;
				colliInfo.fHeight = stageObj->GetHeight() * 0.5f;

				// プレイヤーの近くにオブジェクトがあるか判定
				// ※特定オブジェクトを除く
				if (type != OBJECT_TYPE::TRAMPOLINE &&
					type != OBJECT_TYPE::LASER &&
					type != OBJECT_TYPE::EXTEND_DOG) {

					if (D3DXVec3Length(&(colliInfo.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(colliInfo.fWidth + SIZE_WIDTH, colliInfo.fHeight + SIZE_HEIGHT)))
						continue;
				}

				//別の当たり判定情報
				CCollision::ColliInfo* OtherInfo = NULL;

				// 移動するオブジェクトは、
				// 当たり判定位置に前回位置を設定する
				switch (type) {
					// ブロック
				case OBJECT_TYPE::BLOCK: {
					CBlock* pBlock = (CBlock*)stageObj;
					if (!pBlock->GetCollision())
						continue;
					else if (// 見た目の種類による当たり判定の除外
						pBlock->GetLooksType() == CBlock::LOOKS_TYPE::BAOBAB_TREE ||
						pBlock->GetLooksType() == CBlock::LOOKS_TYPE::PALMTREE    ||
						false)
						continue;
				}break;

					// ゴールゲート
				case OBJECT_TYPE::GOALGATE: {
					CGoalGate* pGoalGate = (CGoalGate*)stageObj;
					if (pGoalGate->GetStartGate())
						continue;
				}break;

					// 移動床
				case OBJECT_TYPE::MOVE_BLOCK: {
					CMoveBlock* pBlock = (CMoveBlock*)stageObj;
					colliInfo.posOld = pBlock->GetPosOld();
				}
											break;

											// 隕石
				case OBJECT_TYPE::METEOR: {
					CMeteor* pMeteor = (CMeteor*)stageObj;
					colliInfo.posOld = pMeteor->GetPosOld();
				}
										break;

										// レーザー
				case OBJECT_TYPE::LASER: {
					CRoadTripLaser* pLaser = (CRoadTripLaser*)stageObj;

					OtherInfo = new CCollision::ColliInfo;

					OtherInfo->pos = pLaser->GetLaserPos();
					OtherInfo->posOld = pLaser->GetPosOld();
					OtherInfo->fWidth = pLaser->GetLaserSize().x * 0.5f;
					OtherInfo->fHeight = pLaser->GetLaserSize().y * 0.5f;

					// 当たった方向を格納
					OtherInfo->Rot = s_pColli->IsBoxToBoxCollider(Self, *OtherInfo, vec);
				}
									   break;

									   // ヌイ
				case OBJECT_TYPE::EXTEND_DOG: {
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
					//	OtherInfo[nCnt].ColliRot = s_pColli->IsBoxToBoxCollider(*pSelf, colliInfo, vec);

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
					CPile* pPile = (CPile*)stageObj;

					colliInfo.pos = pPile->GetPos();
					colliInfo.posOld = pPile->GetPosOld();
				}
				break;
				}

				// 当たった方向を格納
				colliInfo.Rot = s_pColli->IsBoxToBoxCollider(Self, colliInfo, vec);
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
				case OBJECT_TYPE::BLOCK:	 s_pColli->Block(&Self, &colliInfo, Player, (CBlock*)stageObj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::FILLBLOCK: s_pColli->FillBlock(&Self, colliInfo.Rot, &Player.side, &bDeath); break;
				case OBJECT_TYPE::TRAMPOLINE:s_pColli->Trampoline(&Self, &colliInfo, (CTrampoline*)stageObj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::SPIKE:	 s_pColli->Spike(&Self, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::MOVE_BLOCK:s_pColli->MoveBlock(&Self, (CMoveBlock*)stageObj, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::METEOR:	 s_pColli->Meteor(&Self, &colliInfo, &Player.side, &bDeath); break;
				case OBJECT_TYPE::LASER:	 s_pColli->Laser(&Self, (CRoadTripLaser*)stageObj, &colliInfo, NULL, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::EXTEND_DOG:s_pColli->Dog(&Self, (CExtenddog*)stageObj, &colliInfo, NULL, &Player.side, &bDeath); break;
				case OBJECT_TYPE::GOALGATE:	 s_pColli->GoalGate(&Self, &colliInfo, obj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::PARTS:	 s_pColli->Parts(&Self, (CParts*)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::ROCKET:	 s_pColli->Rocket(&Self, (CRocket*)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::PILE:		 s_pColli->Pile(&Self, &colliInfo, (CPile*)stageObj, &Player.side, &bDeath); break;
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
					//s_pColli->IsBoxToBoxCollider(Self, colliInfo, vec);
					Death(Player, type, &nColliRot[nCntPlayer]);
					break;
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
				CollisionAfter(stageObj, type, &nColliRot[0]);
			}
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
			CFloat CaveInPos = pPile->GetPosCaveIn().y;
			CFloat Height = pPile->GetHeight() * 0.5f;

			for each (Info &Player in m_aInfo)
			{
				if(!Player.bLandPile) continue;

				switch (Player.side)
				{
					case WORLD_SIDE::FACE:	Player.pos.y = CaveInPos + Height + SIZE_HEIGHT;	break;
					case WORLD_SIDE::BEHIND:Player.pos.y = CaveInPos - Height - SIZE_HEIGHT;	break;
				}

				Player.move.y = 0.0f;

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
		case CPlayer::SE_LABEL::JUMP:   s_SE.pSound->Play(s_SE.jump,    CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_00: s_SE.pSound->Play(s_SE.dog[0],  CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_01: s_SE.pSound->Play(s_SE.dog[1],  CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_02: s_SE.pSound->Play(s_SE.dog[2],  CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_03: s_SE.pSound->Play(s_SE.dog[3],  CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPING:s_SE.pSound->Play(s_SE.Swaping, CSound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPEND:s_SE.pSound->Play(s_SE.SwapEnd, CSound::CATEGORY::SE, 1.0f, false); break;
	}
}