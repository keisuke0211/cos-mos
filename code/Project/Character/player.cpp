//================================================================================================
//
//プレイヤー処理[player.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../main.h"
#include "player.h"
#include "../../_RNLib/Basis/input.h"
#include "../../_RNLib/Basis/Calculation/number.h"

//スワップインターバル
const int	CPlayer::SWAP_INTERVAL = 30;	//スワップインターバル
int			CPlayer::s_nSwapInterval = 0;	//残りスワップインターバル

const float CPlayer::SIZE_WIDTH = 8.0f;	//横幅
const float CPlayer::SIZE_HEIGHT = 8.0f;//高さ

const float CPlayer::MOVE_SPEED = 0.5f;		//移動量
const float CPlayer::MAX_MOVE_SPEED = 2.7f;	//最大移動量

const float CPlayer::JUMP_POWER = 5.0f;		//基本ジャンプ量
const float CPlayer::GRAVITY_POWER = -0.3f;	//基本重力加速度

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

int			CPlayer::s_nNumGetParts = 0;	//取得したパーツの数
bool		CPlayer::s_bRideRocket = false;	//ロケットに乗れるかどうか

int CPlayer::s_nSwapMarkTex = 0;  //スワップ先のマークテクスチャ番号
int CPlayer::s_nSwapParticle = 0; //スワップ時のパーティクルテクスチャ番号

//=======================================
//コンストラクタ
//=======================================
CPlayer::CPlayer()
{
	s_nSwapMarkTex = 0;		//スワップ先のマークテクスチャ番号
	s_nSwapParticle = 0;	//スワップ時のパーティクルテクスチャ番号
	m_jumpSEIdx = 0;		//ジャンプ時のSE番号
	m_landingSEIdx = 0;		//着地時のSE番号
	s_nSwapInterval = 0;	//残りスワップインターバル
	s_nNumGetParts = 0;		//取得したパーツの数
	s_bRideRocket = false;	//ロケットに乗れるかどうか

	for each(Info &Player in m_aInfo)
	{
		Player.StartPos = INITD3DXVECTOR3;		//開始位置
		Player.pos = INITD3DXVECTOR3;			//位置
		Player.posOLd = INITD3DXVECTOR3;		//前回位置
		Player.rot = INITD3DXVECTOR3;			//向き
		Player.move = INITD3DXVECTOR3;			//移動量
		Player.color = INITCOLOR;				//色
		Player.bGround = false;					//地面に接しているか
		Player.bJump = false;					//ジャンプ
		Player.bRide = false;					//ロケットに乗っているかどうか
		Player.bGoal = false;					//ゴールしたかどうか
		Player.fJumpPower = 0.0f;				//ジャンプ量
		Player.fGravity = 0.0f;					//重力
		Player.fMaxHeight = 0.0f;				//最高Ｙ座標
		Player.nTramJumpCounter = 0;			//トランポリンによって跳ね上がる最高到達地点
		Player.fTramTargetPosY = 0.0f;			//トランポリン用の目標位置
		Player.bTramJump = false;				//トランポリン用の特殊ジャンプ
		Player.TramColliRot = COLLI_ROT::NONE;	//トランポリン用の判定
		Player.bExtendDog = false;				//ヌイ用の接触フラグ
		Player.nModelIdx = NONEDATA;			//モデル番号
		Player.side = WORLD_SIDE::FACE;			//どちらの世界に存在するか
	}

	pOthColli = NULL;		// 他パーツの当たり判定情報
	bPluralColli = false;	// パーツ種類　単体か複数か
}

//=======================================
//デストラクタ
//=======================================
CPlayer::~CPlayer()
{
	if (pOthColli != NULL) {
		delete[] pOthColli;
		pOthColli = NULL;
	}
}

//=======================================
//生成処理
//=======================================
CPlayer *CPlayer::Create(void)
{
	//プレイヤーインスタンスの生成
	CPlayer *pPlayer = new CPlayer;

	//初期化処理
	pPlayer->Init();

	//プレイヤーを返す
	return pPlayer;
}

//=====================================================================================================================
//初期化処理
//=====================================================================================================================
HRESULT CPlayer::Init(void)
{
	//１Ｐ初期情報
	m_aInfo[0].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Human.x");

	//２Ｐ初期情報
	m_aInfo[1].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Octopus.x");
	m_aInfo[1].rot.z = D3DX_PI;

	//キーコンフィグ初期化
	InitKeyConfig();

	s_nSwapMarkTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Circle_005.png");
	s_nSwapParticle = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");

	m_jumpSEIdx = RNLib::Sound().Load("data\\SOUND\\SE\\jamp_01.wav");
	m_landingSEIdx = RNLib::Sound().Load("data\\SOUND\\SE\\jamp.wav");

	m_dogSEIdx[0] = RNLib::Sound().Load("data\\SOUND\\SE\\heron2.wav");	//押す
	m_dogSEIdx[1] = RNLib::Sound().Load("data\\SOUND\\SE\\shrink.wav");	//縮む
	m_dogSEIdx[2] = RNLib::Sound().Load("data\\SOUND\\SE\\extend.wav");	//伸びる
	m_dogSEIdx[3] = RNLib::Sound().Load("data\\SOUND\\SE\\vibration.wav");	//震える

	//初期情報設定
	Death(NULL);

	//初期化成功
	return S_OK;
}

//=======================================
//各プレイヤーのキーボード・ジョイパッドのキーコンフィグ初期化設定
//=======================================
void CPlayer::InitKeyConfig(void)
{
	//どちらの世界でも共通のキー配置設定
	for (int nCntSide = 0; nCntSide < (int)WORLD_SIDE::MAX; nCntSide++)
	{
		//１Ｐのキーボード配置
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_A;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_D;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		//２Ｐのキーボード配置
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_LEFTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_RIGHTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		//世界で変わるキー配置設定
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

	//ジョイパッドの設定は両者共通
	for each(Info &Player in m_aInfo)
	{
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = CInput::BUTTON::LEFT;  //左移動
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = CInput::BUTTON::RIGHT; //右移動
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = CInput::BUTTON::A;     //ジャンプ
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = CInput::BUTTON::Y;     //スワップ
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = CInput::BUTTON::A;     //決定
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = CInput::BUTTON::START; //ポーズ
	}
}

//=====================================================================================================================
//終了処理
//=====================================================================================================================
void CPlayer::Uninit(void)
{

}

//=======================================
//前回位置更新処理
//=======================================
void CPlayer::SetPosOld(void)
{
	//プレイヤーの前回位置更新
	for each (Info &Player in m_aInfo)
	{
		//ロケットに乗っていないなら　or ゴールしていないなら
		if (!Player.bRide || Player.bGoal)
		Player.posOLd = Player.pos;
	}
}

//=====================================================================================================================
//更新処理
//=====================================================================================================================
void CPlayer::Update(void)
{
	//前回位置更新
	SetPosOld();

	//操作処理
	ActionControl();

	//両者ともにゴールしてなかったら
	if (!m_aInfo[0].bGoal && !m_aInfo[1].bGoal)
	{
		//スワップ
		Swap();
	}
	else if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
	{
		int planet = Manager::StgEd()->GetPlanetIdx();
		int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

	//当たり判定まとめ
	WholeCollision();

	//情報更新
	UpdateInfo();

	RNLib::Text2D().PutDebugLog(CreateText("FPS:%d", RNLib::GetFPSCount()));
}

//----------------------------
//情報更新処理
//----------------------------
void CPlayer::UpdateInfo(void)
{
	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;
		//ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		//位置設定
		RNLib::Model().Put(Player.pos, Player.rot, Player.nModelIdx, false)
			->SetOutLine(true)
			->SetCol(Player.color);

		//スワップ先のマークを描画する位置
		D3DXVECTOR3 MarkPos = Player.pos;
		MarkPos.z = -10.0f;
		MarkPos.y *= -1.0f;

		RNLib::Polygon3D().Put(MarkPos, INITD3DXVECTOR3)
			->SetSize(20.0f, 20.0f)
			->SetBillboard(true)
			->SetTex(s_nSwapMarkTex)
			->SetCol(Color{ Player.color.r, Player.color.g, Player.color.b, 100 })
			->SetPriority(1);

		//最高Ｙ座標更新
		switch (Player.side)
		{
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
		RNLib::Text2D().PutDebugLog(CreateText("%dP最高Y座標：%f", nCntPlayer, Player.fMaxHeight));
	}
}

//----------------------------
//操作処理
//----------------------------
void CPlayer::ActionControl(void)
{
	//プレイヤー番号
	int nIdxPlayer = -1;

	for each (Info &Player in m_aInfo)
	{
		//次のプレイヤー番号へ
		nIdxPlayer++;

		// 出る
		if ((Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bRide = false;
			Player.bGoal = false;
			Player.pos.x = Player.pos.x - 20.0f;
		}

		//ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		//ジャンプ入力（空中じゃない）
		if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bGround = false;				//地面から離れた
			Player.move.y = Player.fJumpPower;	//ジャンプ量代入
			Player.bJump = true;				//ジャンプした

												//SE再生
			RNLib::Sound().Play(m_jumpSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
		}

		//右に移動
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer))
			Player.move.x += MOVE_SPEED;

		//左に移動
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer))
			Player.move.x -= MOVE_SPEED;

		//スワップ入力
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
			Manager::EffectMgr()->ParticleCreate(s_nSwapParticle, Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
	}
}

//############################
//スワップ処理
//############################
void CPlayer::Swap(void)
{
	//インターバルがあれば減少させて終了
	if (s_nSwapInterval > 0)
	{
		s_nSwapInterval--;
		return;
	}


	//両者ともにスワップボタンを押しているまたはどちらかがロケットに乗っている
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) || m_aInfo[0].bRide) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) || m_aInfo[1].bRide))
	{
		//インターバル設定
		s_nSwapInterval = SWAP_INTERVAL;

		for each (Info &Player in m_aInfo)
		{
			//ロケットに乗ってたらスキップ
			if (Player.bRide) continue;

			for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(s_nSwapParticle, Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}

			//位置・重力加速度・ジャンプ量・存在する世界を反転
			Player.pos.y *= -1.0f;
			Player.fGravity *= -1.0f;
			Player.fJumpPower *= -1.0f;
			Player.side = (WORLD_SIDE)(((int)Player.side + 1) % (int)WORLD_SIDE::MAX);

			if (Player.side == WORLD_SIDE::FACE)
				Player.rot.z = 0.0f;
			else if (Player.side == WORLD_SIDE::BEHIND)
				Player.rot.z = D3DX_PI;
		}

		//前回位置更新
		SetPosOld();
	}
}

//----------------------------
//死亡処理
//----------------------------
void CPlayer::Death(D3DXVECTOR3 *pDeathPos)
{
	int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	if (pDeathPos != NULL)
	{
		Manager::EffectMgr()->EffectCreate(EffTex, *pDeathPos, INIT_EFFECT_SCALE, Color{ 255,0,255,255 });

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(ParTex, *pDeathPos, INIT_EFFECT_SCALE * 0.5f, Color{ 255,0,0,255 });
		}
	}

	//１Ｐ用初期情報
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;

	//２Ｐ用初期情報
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;

	//両者共通初期情報
	for each (Info &Player in m_aInfo)
	{
		Player.posOLd = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
	}
}

//----------------------------
//移動処理
//----------------------------
void CPlayer::Move(COLLI_VEC vec)
{
	//プレイヤーの位置更新
	for each (Info &Player in m_aInfo)
	{
		//ロケットに乗ってたら　or ゴールしていたらスキップ
		if (Player.bRide || Player.bGoal) continue;

		//移動量反映
		switch (vec)
		{
		case COLLI_VEC::X:
			//慣性処理
			Player.move.x += (0.0f - Player.move.x) * 0.1f;

			//Ⅹの移動量を修正
			FloatControl(&Player.move.x, MAX_MOVE_SPEED, -MAX_MOVE_SPEED);

			//位置更新
			Player.pos.x += Player.move.x;
			break;

			//重力処理
		case COLLI_VEC::Y:

			//トランポリンによる特殊ジャンプ中
			if (Player.bTramJump)
			{//カウンターを減らして、０になったら特殊ジャンプ終了
				
				float diff = Player.fTramTargetPosY - Player.pos.y;

				if (--Player.nTramJumpCounter <= 0)
				{
					Player.bTramJump = false;
					Player.move.y = diff / 1;
				}
				else
				{
					Player.move.y = diff / Player.nTramJumpCounter;
				}
			}
			//通常時なら、重力処理でＹの移動量を計算
			else Player.move.y += Player.fGravity;

			//位置更新
			Player.pos.y += Player.move.y;

			break;
		}
	}
}

//----------------------------
//当たり判定まとめ
//----------------------------
void CPlayer::WholeCollision(void)
{
	//一旦両プレイヤーともにジャンプ不可
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++) {

		//衝突ベクトルをキャスト
		const COLLI_VEC vec = (COLLI_VEC)nCntVec;

		//移動処理
		Move(vec);

		//オブジェクトのポインタを格納
		CObject *obj = NULL;

		//オブジェクトを取得
		while (Manager::BlockMgr()->ListLoop(&obj)) {
			//取得したオブジェクトをキャスト
			CStageObject* stageObj = (CStageObject*)obj;

			//当たり判定の情報
			Colli m_aColli;

			//オブジェクトの当たり判定情報取得
			m_aColli.pos = stageObj->GetPos();
			m_aColli.fWidth = stageObj->GetWidth() * 0.5f;
			m_aColli.fHeight = stageObj->GetHeight() * 0.5f;

			//オブジェクトの最小・最大位置
			m_aColli.MinPos = D3DXVECTOR3(m_aColli.pos.x - m_aColli.fWidth, m_aColli.pos.y - m_aColli.fHeight, 0.0f);
			m_aColli.MaxPos = D3DXVECTOR3(m_aColli.pos.x + m_aColli.fWidth, m_aColli.pos.y + m_aColli.fHeight, 0.0f);

			for each(Info& Player in m_aInfo)
			{
				//ロケットに乗ってたら　or ゴールしていたらスキップ
				if (Player.bRide || Player.bGoal) continue;

				//種類取得
				const CStageObject::TYPE type = stageObj->GetType();

				//プレイヤーの近くにオブジェクトがあるか判定
				//複数オブジェクト以外の判定
				if (type != CStageObject::TYPE::TRAMPOLINE && type != CStageObject::TYPE::LASER && type != CStageObject::TYPE::EXTEND_DOG) {
					if (D3DXVec3Length(&(m_aColli.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(m_aColli.fWidth + SIZE_WIDTH, m_aColli.fHeight + SIZE_HEIGHT))) continue;
				}

				//前回位置
				m_aColli.posOLd = m_aColli.pos;

				//他パーツの当たり判定フラグ
				bool bOtherColl = false;

				//移動するオブジェクトは、前回位置を特別に設定
				switch (type)
				{
					//移動床
				case CStageObject::TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock *)stageObj;
					m_aColli.pos = pBlock->GetPosOld();
				}
				break;

				// トランポリン
				case CStageObject::TYPE::TRAMPOLINE:{
					CTrampoline *pTrampoline = (CTrampoline *)stageObj;

					OthColliDelete();
					bPluralColli = true;
					pOthColli = new Colli[OBJ_TRAMPOLINE];

					for (int nCnt = 0; nCnt < OBJ_TRAMPOLINE; nCnt++) {

						pOthColli[nCnt].pos = pTrampoline->GetSpringPos(nCnt);
						pOthColli[nCnt].posOLd = pOthColli[nCnt].pos;
						pOthColli[nCnt].fWidth = pTrampoline->GetWidth() * 1.0f;
						pOthColli[nCnt].fHeight = pTrampoline->GetHeight() * 1.0f;
						pOthColli[nCnt].ColliRot = COLLI_ROT::NONE;

						if (D3DXVec3Length(&(pOthColli[nCnt].pos - Player.pos)) >
							D3DXVec2Length(&D3DXVECTOR2(pOthColli[nCnt].fWidth + SIZE_WIDTH, pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

						pOthColli[nCnt].fWidth = pTrampoline->GetWidth() * 0.5f;
						pOthColli[nCnt].fHeight = pTrampoline->GetHeight() * 0.5f;

						//オブジェクトの最小・最大位置
						pOthColli[nCnt].MinPos = D3DXVECTOR3(pOthColli[nCnt].pos.x - pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y - pOthColli[nCnt].fHeight, 0.0f);
						pOthColli[nCnt].MaxPos = D3DXVECTOR3(pOthColli[nCnt].pos.x + pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y + pOthColli[nCnt].fHeight, 0.0f);

						//当たった方向を格納
						COLLI_ROT ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli[nCnt].pos, pOthColli[nCnt].posOLd, pOthColli[nCnt].fWidth, pOthColli[nCnt].fHeight, vec);

						if (ColliRot != COLLI_ROT::NONE && !bOtherColl) {
							bOtherColl = true;
							Player.TramColliRot = ColliRot;
							pOthColli[nCnt].ColliRot = ColliRot;
						}
						else if (ColliRot == COLLI_ROT::NONE && !bOtherColl){
							Player.TramColliRot = COLLI_ROT::NONE;
						}
					}
				}
				break;

				//隕石
				case CStageObject::TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					m_aColli.pos = pMeteor->GetPosOld();
				}
				break;

				//レーザー
				case CStageObject::TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					OthColliDelete();
					pOthColli = new Colli;
					bPluralColli = false;

					pOthColli->pos = pLaser->GetLaserPos();
					pOthColli->posOLd = pOthColli->pos;
					pOthColli->fWidth = pLaser->GetLaserSize().x * 0.5f;
					pOthColli->fHeight = pLaser->GetLaserSize().y * 0.5f;

					if (D3DXVec3Length(&(pOthColli->pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(pOthColli->fWidth + SIZE_WIDTH, pOthColli->fHeight + SIZE_HEIGHT))) continue;

					//オブジェクトの最小・最大位置
					pOthColli->MinPos = D3DXVECTOR3(pOthColli->pos.x - pOthColli->fWidth, pOthColli->pos.y - pOthColli->fHeight, 0.0f);
					pOthColli->MaxPos = D3DXVECTOR3(pOthColli->pos.x + pOthColli->fWidth, pOthColli->pos.y + pOthColli->fHeight, 0.0f);

					//当たった方向を格納
					pOthColli->ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli->pos, pOthColli->posOLd, pOthColli->fWidth, pOthColli->fHeight, vec);

					if (pOthColli->ColliRot != COLLI_ROT::NONE && !bOtherColl) {
						bOtherColl = true;
					}
				}
				break;

				//ヌイ
				case CStageObject::TYPE::EXTEND_DOG:{
					CExtenddog *pDog = (CExtenddog *)stageObj;

					OthColliDelete();
					pOthColli = new Colli[OBJ_EXTENDDOG];
					bPluralColli = true;

					for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
						switch (nCnt){
						case 0:
							pOthColli[nCnt].pos = pDog->GetHeadPos();
							pOthColli[nCnt].posOLd = pDog->GetHeadPosOid();
							break;
						case 1:
							pOthColli[nCnt].pos = pDog->GetBodyPos();
							pOthColli[nCnt].posOLd = pDog->GetBodyPos();
							break;
						case 2:
							pOthColli[nCnt].pos = pDog->GetHipPos();
							pOthColli[nCnt].posOLd = pDog->GetHipPos();
							break;
						}

						pOthColli[nCnt].fWidth = pDog->GetWidth() * 0.5f;
						pOthColli[nCnt].fHeight = pDog->GetHeight() * 0.5f;

						if (D3DXVec3Length(&(pOthColli[nCnt].pos - Player.pos)) >
							D3DXVec2Length(&D3DXVECTOR2(pOthColli[nCnt].fWidth + SIZE_WIDTH, pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

						//オブジェクトの最小・最大位置
						pOthColli[nCnt].MinPos = D3DXVECTOR3(pOthColli[nCnt].pos.x - pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y - pOthColli[nCnt].fHeight, 0.0f);
						pOthColli[nCnt].MaxPos = D3DXVECTOR3(pOthColli[nCnt].pos.x + pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y + pOthColli[nCnt].fHeight, 0.0f);

						//当たった方向を格納
						pOthColli[nCnt].ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli[nCnt].pos, pOthColli[nCnt].posOLd, pOthColli[nCnt].fWidth, pOthColli[nCnt].fHeight, vec);

						if (pOthColli[nCnt].ColliRot != COLLI_ROT::NONE && !bOtherColl){
							bOtherColl = true;
						}
					}

					CExtenddog::STATE state = pDog->GetState();
					if (pOthColli[2].ColliRot != COLLI_ROT::OVER && Player.bExtendDog && state == CExtenddog::STATE::DOWN_LAND) {
						Player.bExtendDog = false;
					}
				}
				break;
				}

				//当たった方向を格納
				m_aColli.ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, m_aColli.pos, m_aColli.pos, m_aColli.fWidth, m_aColli.fHeight, vec);

				//当たっていなければスキップ
				if (m_aColli.ColliRot == COLLI_ROT::NONE && !bOtherColl) continue;

				//種類ごとに関数分け
				switch (type)
				{
				case CStageObject::TYPE::BLOCK:			CollisionBlock(&Player, &m_aColli);	break;
				case CStageObject::TYPE::FILLBLOCK:		CollisionFillBlock(&Player, m_aColli.ColliRot); break;
				case CStageObject::TYPE::TRAMPOLINE:	CollisionTrampoline(&Player, &m_aColli, pOthColli);	break;
				case CStageObject::TYPE::SPIKE:			CollisionSpike(&Player, &m_aColli);	break;
				case CStageObject::TYPE::MOVE_BLOCK:	CollisionMoveBlock(&Player, (CMoveBlock *)stageObj, &m_aColli);	break;
				case CStageObject::TYPE::METEOR:		CollisionMeteor(&Player, &m_aColli); break;
				case CStageObject::TYPE::LASER:			CollisionLaser(&Player, (CRoadTripLaser *)stageObj,&m_aColli, pOthColli);	break;
				case CStageObject::TYPE::EXTEND_DOG:	CollisionDog(&Player, (CExtenddog *)stageObj, &m_aColli, pOthColli); break;
				case CStageObject::TYPE::GOALGATE:		CollisionGoalGate(&Player, &m_aColli);	break;
				case CStageObject::TYPE::PARTS:			CollisionParts(&Player, (CParts *)stageObj); break;
				case CStageObject::TYPE::ROCKET:		CollisionRocket(&Player, (CRocket *)stageObj); break;
				}

				//当たれば即死のオブジェクトに当たっている
				if (type == CStageObject::TYPE::SPIKE || type == CStageObject::TYPE::METEOR || type == CStageObject::TYPE::LASER)
					break;

				OthColliDelete();
			}

			// ヌイの状態設定
			CExtenddog *pDog = (CExtenddog *)stageObj;
			CExtenddog::STATE state = pDog->GetState();
			if (!m_aInfo[0].bExtendDog && !m_aInfo[1].bExtendDog && state == CExtenddog::STATE::DOWN_LAND)
			{
				pDog->SetState(CExtenddog::STATE::RETURN);
			}
		}
	}
}

//----------------------------
//上からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	//位置・移動量修正
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
//下からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	//位置・移動量修正
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
//左からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	//位置・移動量修正
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
//右からの当たり判定による位置修正
//----------------------------
void CPlayer::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	//位置・移動量修正
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// ブロックの当たり判定処理
//----------------------------
void CPlayer::CollisionBlock(Info *pInfo, Colli *pColli)
{
	//当たった方向ごとに処理を切り替え
	switch (pColli->ColliRot)
	{
		//*********************************
		//上に当たった
		//*********************************
	case COLLI_ROT::OVER:
		//位置・移動量修正
		FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

		//表の世界のプレイヤー
		if (pInfo->side == WORLD_SIDE::FACE) {
			if (pInfo->bJump == true)
			{//着地した
				//SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//地面に接している
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pColli->MaxPos.y;//最高Ｙ座標設定pInfo->bJump = false
		}
		break;

		//*********************************
		//下に当たった
		//*********************************
	case COLLI_ROT::UNDER:
		//位置・移動量修正
		FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

		//裏の世界のプレイヤーならジャンプ可能
		if (pInfo->side == WORLD_SIDE::BEHIND) {
			if (pInfo->bJump == true)
			{//着地した
				//SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//地面に接している
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pColli->MinPos.y;//最高Ｙ座標設定
		}
		break;

		//*********************************
		//左に当たった
		//*********************************
	case COLLI_ROT::LEFT:
		//位置・移動量修正
		FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//右に当たった
		//*********************************
	case COLLI_ROT::RIGHT:
		//位置・移動量修正
		FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//埋まった
		//*********************************
	case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
	}
}

//----------------------------
// 穴埋めブロックの当たり判定処理
//----------------------------
void CPlayer::CollisionFillBlock(Info *pInfo,COLLI_ROT ColliRot)
{
	Death(&pInfo->pos);
}

//----------------------------
//トランポリンの当たり判定処理
//----------------------------
void CPlayer::CollisionTrampoline(Info *pInfo, Colli *pColli, Colli *pOthColli)
{
	//当たった方向ごとに処理を切り替え
	switch (pColli->ColliRot)
	{
		//*********************************
		//上に当たった
		//*********************************
	case COLLI_ROT::OVER:
		//位置・移動量修正
		FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y, pOthColli[0].fHeight);
		break;

		//*********************************
		//下に当たった
		//*********************************
	case COLLI_ROT::UNDER:
		//位置・移動量修正
		FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);
		break;
	}

	// ばね上
	switch (pOthColli[0].ColliRot)
	{
		//*********************************
		//上に当たった
		//*********************************
	case COLLI_ROT::OVER:
		//位置・移動量修正
		FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y,pOthColli[0].fHeight);

		//表の世界のプレイヤー
		if (pInfo->side == WORLD_SIDE::FACE) {
			if (pInfo->bJump == true)
			{//着地した

				//SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//最高Ｙ座標設定
		}
		break;

		//*********************************
		//左に当たった
		//*********************************
	case COLLI_ROT::LEFT:
		//位置・移動量修正
		FixPos_LEFT(&pInfo->pos.x, pOthColli[0].MinPos.x, &pInfo->move.x, pOthColli[0].fWidth);
		break;

		//*********************************
		//右に当たった
		//*********************************
	case COLLI_ROT::RIGHT:
		//位置・移動量修正
		FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].MaxPos.x, &pInfo->move.x, pOthColli[0].fWidth);
		break;
	}

	// ばね下
	switch (pOthColli[1].ColliRot)
	{
		//*********************************
		//下に当たった
		//*********************************
	case COLLI_ROT::UNDER:
		//位置・移動量修正
		FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);

		//裏の世界のプレイヤーならジャンプ可能
		if (pInfo->side == WORLD_SIDE::BEHIND) {
			if (pInfo->bJump == true)
			{//着地した
			 //SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pOthColli[1].MinPos.y;//最高Ｙ座標設定
		}
		break;

		//*********************************
		//左に当たった
		//*********************************
	case COLLI_ROT::LEFT:
		//位置・移動量修正
		FixPos_LEFT(&pInfo->pos.x, pOthColli[1].MinPos.x, &pInfo->move.x, pOthColli[1].fWidth);
		break;

		//*********************************
		//右に当たった
		//*********************************
	case COLLI_ROT::RIGHT:
		//位置・移動量修正
		FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].MaxPos.x, &pInfo->move.x, pOthColli[1].fWidth);
		break;
	}
}

//----------------------------
//トゲの当たり判定処理
//----------------------------
void CPlayer::CollisionSpike(Info *pInfo, Colli *pColli)
{
	//死亡処理
	Death(&pInfo->pos);
}

//----------------------------
// 移動床の当たり判定処理
//----------------------------
void CPlayer::CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, Colli *pColli)
{
	switch (pColli->ColliRot)
	{
		//*********************************
		//上に当たった
		//*********************************
	case COLLI_ROT::OVER:
		//位置・移動量修正
		FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

		//表の世界のプレイヤーの場合
		if (pInfo->side == WORLD_SIDE::FACE)
		{
			pInfo->move = pMoveBlock->GetMove();
			if (pInfo->bJump == true)
			{//着地した
			 //SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//地面に接している
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pColli->MaxPos.y;//最高Ｙ座標設定
		}
		break;

		//*********************************
		//下に当たった
		//*********************************
	case COLLI_ROT::UNDER:
		//位置・移動量修正
		FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

		//裏の世界のプレイヤーならジャンプ可能
		if (pInfo->side == WORLD_SIDE::BEHIND)
		{
			pInfo->pos = pMoveBlock->GetMove();
			if (pInfo->bJump == true)
			{//着地した
			 //SE再生
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//地面に接している
			pInfo->bJump = false;	//ジャンプ可能
			pInfo->fMaxHeight = pColli->MinPos.y;//最高Ｙ座標設定
		}
		break;

		//*********************************
		//左に当たった
		//*********************************
	case COLLI_ROT::LEFT:
		//位置・移動量修正
		FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//右に当たった
		//*********************************
	case COLLI_ROT::RIGHT:
		//位置・移動量修正
		FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
		break;

	case COLLI_ROT::UNKNOWN:
		//移動床 -> プレイヤーへの当たり判定処理を実行
		const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
		const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
		const float fWidth = pMoveBlock->GetWidth() * 0.5f;
		const float fHeight = pMoveBlock->GetHeight() * 0.5f;

		//移動床からの当たり判定
		for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++)
		{
			//プレイヤーのどの方向に当たっているか
			pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOLd, pColli->fWidth, pColli->fHeight, (COLLI_VEC)nCntVec);

			//それでも当たらないなら、スキップ
			if (pColli->ColliRot == COLLI_ROT::NONE || pColli->ColliRot == COLLI_ROT::UNKNOWN) continue;

			//当たった方向（上下・左右）を反転する
			{
				//当たった方向をint型に変換
				const int nRot = (int)pColli->ColliRot;

				pColli->ColliRot = (COLLI_ROT)(nRot - 1 + 2 * (nRot % 2));
			}

			//もう一度当たり判定
			CollisionMoveBlock(pInfo, pMoveBlock, pColli);
		}
		break;
	}
}

//----------------------------
// 隕石の当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionMeteor(Info *pInfo, Colli *pColli)
{
	//死亡処理
	Death(&pInfo->pos);
}

//----------------------------
// レーザーの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, Colli *pColli, Colli *pOthColli)
{
	// 本体
	{
		//当たった方向ごとに処理を切り替え
		switch (pColli->ColliRot)
		{
			//*********************************
			//上に当たった
			//*********************************
		case COLLI_ROT::OVER:
			//位置・移動量修正
			FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

			//表の世界のプレイヤーの場合
			if (pInfo->side == WORLD_SIDE::FACE)
			{
				pInfo->move = pRoadTripLaser->GetMove();
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pColli->MaxPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//下に当たった
			//*********************************
		case COLLI_ROT::UNDER:
			//位置・移動量修正
			FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

			//表の世界のプレイヤーの場合
			if (pInfo->side == WORLD_SIDE::FACE)
			{
				pInfo->move = pRoadTripLaser->GetMove();
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pColli->MinPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//左に当たった
			//*********************************
		case COLLI_ROT::LEFT:
			//位置・移動量修正
			FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//右に当たった
			//*********************************
		case COLLI_ROT::RIGHT:
			//位置・移動量修正
			FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//埋まった
			//*********************************
		case COLLI_ROT::UNKNOWN: 
			//レーザー -> プレイヤーへの当たり判定処理を実行
			const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
			const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
			const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
			const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;

			//移動床からの当たり判定
			for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++)
			{
				//プレイヤーのどの方向に当たっているか
				pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOLd, pColli->fWidth, pColli->fHeight, (COLLI_VEC)nCntVec);

				//それでも当たらないなら、スキップ
				if (pColli->ColliRot == COLLI_ROT::NONE || pColli->ColliRot == COLLI_ROT::UNKNOWN) continue;

				//当たった方向（上下・左右）を反転する
				{
					//当たった方向をint型に変換
					const int nRot = (int)pColli->ColliRot;

					pColli->ColliRot = (COLLI_ROT)(nRot - 1 + 2 * (nRot % 2));
				}

				//もう一度当たり判定
				CollisionLaser(pInfo, pRoadTripLaser, pColli, pOthColli);
			}
			break;
		}
	}

	// レーザー
	{
		//死亡処理
		Death(&pInfo->pos);
	}
}

//----------------------------
// ヌイの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionDog(Info *pInfo, CExtenddog *pExtenddog, Colli *pColli, Colli *pOthColli)
{
	//当たった方向ごとに処理を切り替え

	CExtenddog::STATE State = pExtenddog->GetState();

	// ハウス
	{
		switch (pColli->ColliRot)
		{
			//*********************************
			//上に当たった
			//*********************************
		case COLLI_ROT::OVER:
			//位置・移動量修正
			FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

			//表の世界のプレイヤー
			if (pInfo->side == WORLD_SIDE::FACE) {
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pColli->MinPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//下に当たった
			//*********************************
		case COLLI_ROT::UNDER:
			//位置・移動量修正
			FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

			//裏の世界のプレイヤーならジャンプ可能
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pColli->MinPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//左に当たった
			//*********************************
		case COLLI_ROT::LEFT:
			//位置・移動量修正
			FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//右に当たった
			//*********************************
		case COLLI_ROT::RIGHT:
			//位置・移動量修正
			FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fHeight);
			break;

			//*********************************
			//埋まった
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}

	// 頭
	{
		CExtenddog::STATE state = pExtenddog->GetState();
		switch (pOthColli[0].ColliRot)
		{
			//*********************************
			//上に当たった
			//*********************************
		case COLLI_ROT::OVER:
			//位置・移動量修正
			FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y, pOthColli[0].fHeight);

			//表の世界のプレイヤーの場合
			if (pInfo->side == WORLD_SIDE::FACE){
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//下に当たった
			//*********************************
		case COLLI_ROT::UNDER:
			//位置・移動量修正
			FixPos_UNDER(&pInfo->pos.y, pOthColli[0].MinPos.y, &pInfo->move.y, pOthColli[0].fHeight);

			//表の世界のプレイヤーの場合
			if (pInfo->side == WORLD_SIDE::FACE){
				if (pInfo->bJump == true)
				{//着地した
				 //SE再生
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//左に当たった
			//*********************************
		case COLLI_ROT::LEFT:
			//位置・移動量修正
			FixPos_LEFT(&pInfo->pos.x, pOthColli[0].MinPos.x, &pInfo->move.x, pOthColli[0].fWidth);
			break;

			//*********************************
			//右に当たった
			//*********************************
		case COLLI_ROT::RIGHT:

			if (state == CExtenddog::STATE::NONE) {
				//位置・移動量修正
				FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].MaxPos.x, &pInfo->move.x, pOthColli[0].fWidth);
			}
			break;

			//*********************************
			//埋まった
			//*********************************
		case COLLI_ROT::UNKNOWN: break;

		}
	}

	// 体
	{
		switch (pOthColli[1].ColliRot)
		{
			//*********************************
			//上に当たった
			//*********************************
		case COLLI_ROT::OVER:
			//位置・移動量修正
			FixPos_OVER(&pInfo->pos.y, pOthColli[1].MaxPos.y, &pInfo->move.y, pOthColli[1].fHeight);

			//表の世界のプレイヤー
			if (pInfo->side == WORLD_SIDE::FACE) {
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[1].MaxPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//下に当たった
			//*********************************
		case COLLI_ROT::UNDER:
			//位置・移動量修正
			FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);

			//裏の世界のプレイヤーならジャンプ可能
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				pInfo->bGround = true;
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[1].MinPos.y;//最高Ｙ座標設定
			}
			break;

			//*********************************
			//左に当たった
			//*********************************
		case COLLI_ROT::LEFT:
			//位置・移動量修正
			FixPos_LEFT(&pInfo->pos.x, pOthColli[1].MinPos.x, &pInfo->move.x, pOthColli[1].fWidth);
			break;

			//*********************************
			//右に当たった
			//*********************************
		case COLLI_ROT::RIGHT:
			//位置・移動量修正
			FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].MaxPos.x, &pInfo->move.x, pOthColli[1].fWidth);
			break;

			//*********************************
			//埋まった
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}

	// 尻
	{
		switch (pOthColli[2].ColliRot)
		{
			//*********************************
			//上に当たった
			//*********************************
		case COLLI_ROT::OVER:
			//位置・移動量修正
			FixPos_OVER(&pInfo->pos.y, pOthColli[2].MaxPos.y, &pInfo->move.y, pOthColli[2].fHeight);

			//表の世界のプレイヤー
			if (pInfo->side == WORLD_SIDE::FACE) {
				pInfo->bGround = true;	//地面に接している
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[2].MaxPos.y;//最高Ｙ座標設定
			}

			if (State == CExtenddog::STATE::NONE)
				//SE再生
				RNLib::Sound().Play(m_dogSEIdx[0], CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);

			pExtenddog->SetState(CExtenddog::STATE::UP_LAND);
			pInfo->bExtendDog = true;
			break;

			//*********************************
			//下に当たった
			//*********************************
		case COLLI_ROT::UNDER:
			//位置・移動量修正
			FixPos_UNDER(&pInfo->pos.y, pOthColli[2].MinPos.y, &pInfo->move.y, pOthColli[2].fHeight);

			//裏の世界のプレイヤーならジャンプ可能
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				pInfo->bGround = true;
				pInfo->bJump = false;	//ジャンプ可能
				pInfo->fMaxHeight = pOthColli[2].MinPos.y;//最高Ｙ座標設定
			}

			if (State == CExtenddog::STATE::NONE)
				//SE再生
				RNLib::Sound().Play(m_dogSEIdx[0], CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);

			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
			pInfo->bExtendDog = true;
			break;

			//*********************************
			//左に当たった
			//*********************************
		case COLLI_ROT::LEFT:
			//位置・移動量修正
			FixPos_LEFT(&pInfo->pos.x, pOthColli[2].MinPos.x, &pInfo->move.x, pOthColli[2].fWidth);
			break;

			//*********************************
			//右に当たった
			//*********************************
		case COLLI_ROT::RIGHT:
			//位置・移動量修正
			FixPos_RIGHT(&pInfo->pos.x, pOthColli[2].MaxPos.x, &pInfo->move.x, pOthColli[2].fWidth);
			break;

			//*********************************
			//埋まった
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}
}

//----------------------------
// ゴールゲートの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionGoalGate(Info *pInfo, Colli *pColli)
{
	if (!pInfo->bGoal)
	{
		pInfo->bGoal = true;

		int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
		int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
		}
	}
}

//----------------------------
//パーツの当たり判定処理
//----------------------------
void CPlayer::CollisionParts(Info *pInfo, CParts *pParts)
{
	if (!pParts->GetDisp()) return;

	//取得したので描画OFF
	pParts->DispSwitch(false);

	//取得数増加
	s_nNumGetParts++;

	//取得した数が全体数と同じなら、ロケット乗車可能
	if (s_nNumGetParts == CParts::GetNumAll()) s_bRideRocket = true;
}

//----------------------------
//ロケットの当たり判定処理
//----------------------------
void CPlayer::CollisionRocket(Info *pInfo, CRocket *pRocket)
{
	int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	if (!s_bRideRocket) return;

	//ロケットに搭乗
	pInfo->bRide = true;
	pRocket->Ride();

	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	{
		Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	}
}

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
CPlayer::COLLI_ROT CPlayer::IsBoxCollider(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetPosOld, float TargetWidth, float TargetHeight, COLLI_VEC value)
{
	//自分の現在の最小・最大位置
	const D3DXVECTOR2 MinPos = D3DXVECTOR2(pos.x - fWidth, pos.y - fHeight);
	const D3DXVECTOR2 MaxPos = D3DXVECTOR2(pos.x + fWidth, pos.y + fHeight);

	//対象の現在の最小・最大位置
	const D3DXVECTOR2 TARGET_MinPos = D3DXVECTOR2(TargetPos.x - fWidth, TargetPos.y - fHeight);
	const D3DXVECTOR2 TARGET_MaxPos = D3DXVECTOR2(TargetPos.x + fWidth, TargetPos.y + fHeight);

	//めり込んでいるか判定
	if (MinPos.x < TARGET_MaxPos.x && TARGET_MinPos.x < MaxPos.x &&
		MinPos.y < TARGET_MaxPos.y && TARGET_MinPos.y < MaxPos.y)
	{
		//自分の過去の最小・最大位置
		const D3DXVECTOR2 OLD_MINPOS = D3DXVECTOR2(posOld.x - fWidth, posOld.y - fHeight);
		const D3DXVECTOR2 OLD_MAXPOS = D3DXVECTOR2(posOld.x + fWidth, posOld.y + fHeight);

		//対象の前回の最小・最大位置
		const D3DXVECTOR2 TARGET_MinPosOld = D3DXVECTOR2(TargetPosOld.x - fWidth, TargetPosOld.y - fHeight);
		const D3DXVECTOR2 TARGET_MaxPosOld = D3DXVECTOR2(TargetPosOld.x + fWidth, TargetPosOld.y + fHeight);

		//衝突ベクトルで処理分け
		switch (value)
		{
		case COLLI_VEC::X:
			//前回は左からめり込んでいない（今はめり込んだ
			if (OLD_MAXPOS.x <= TARGET_MinPosOld.x)			return COLLI_ROT::LEFT;

			//前回は右からめり込んでいない（今はめり込んだ
			else if (OLD_MINPOS.x >= TARGET_MaxPosOld.x)	return COLLI_ROT::RIGHT;
			break;

		case COLLI_VEC::Y:
			//前回は上からめり込んでいない（今はめり込んだ
			if (OLD_MINPOS.y >= TARGET_MaxPosOld.y)			return COLLI_ROT::OVER;

			//前回は下からめり込んでいない（今はめり込んだ
			else if (OLD_MAXPOS.y <= TARGET_MinPosOld.y)	return COLLI_ROT::UNDER;
			break;
		}

		//当たった方向が分からない
		return COLLI_ROT::UNKNOWN;
	}

	//当たらなかった
	return COLLI_ROT::NONE;
}

//----------------------------
//プレイヤー情報設定
//指定された番号のプレイヤー情報を設定します。
//----------------------------
void CPlayer::SetInfo(const Info info, const int nNum)
{
	if (0 <= nNum && nNum < NUM_PLAYER)
	{
		//各プレイヤー情報設定
		m_aInfo[nNum] = info;
		m_aInfo[nNum].StartPos = info.pos;
	}
}

//----------------------------
//プレイヤー情報設定
//----------------------------
void CPlayer::SetInfo(Info p1, Info p2)
{
	//各プレイヤー情報設定
	m_aInfo[0] = p1;	m_aInfo[0].StartPos = p1.pos;
	m_aInfo[1] = p2;	m_aInfo[1].StartPos = p2.pos;
}

//----------------------------
//プレイヤーにトランポリン用のジャンプを設定
//----------------------------
void CPlayer::SetTrampolineJump(Info*& pInfo, float fMaxHeight)
{
	SetSwapInterval();

	fMaxHeight += 20.0f;

	//ジャンプ量を継承
	float diff = -fMaxHeight - pInfo->pos.y;
	pInfo->move.y = pInfo->move.y = diff / TRAMPOLINE_JUMP_COUNTER;

	pInfo->fTramTargetPosY = -fMaxHeight;
	pInfo->nTramJumpCounter = TRAMPOLINE_JUMP_COUNTER;
	pInfo->bTramJump = true;
	pInfo->bGround = false;
}

//----------------------------
//プレイヤー情報取得
//指定された世界にいるプレイヤーの情報を返します
//----------------------------
CPlayer::Info *CPlayer::GetInfo(WORLD_SIDE side)
{
	//１Ｐのいる世界と合致したら１Ｐ情報を返す
	if (m_aInfo[0].side == side) return &m_aInfo[0];

	//違うなら２Ｐ情報を返す
	else return &m_aInfo[1];
}

//----------------------------
//プレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//プレイヤーが指定されたキーコンフィグを使っているか
//----------------------------
bool CPlayer::IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//他の当たり判定情報のメモリ解放
//----------------------------
void CPlayer::OthColliDelete(void)
{
	if (pOthColli != NULL)
	{
		if (!bPluralColli)
		{
			delete pOthColli;
		}
		else if (bPluralColli)
		{
			delete[] pOthColli;
		}
		pOthColli = NULL;
	}
}
