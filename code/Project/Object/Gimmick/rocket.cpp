//========================================
// 
// ロケット
// Author:KOMURO HIROMU
// 
//========================================
// *** rocket.cpp ***
//========================================
#include "rocket.h"
#include "../../main.h"
#include"../../Character/player.h"
#include"../../resource.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CRocketクラス
//----------|---------------------------------------------------------------------
//================================================================================
const int   CRocket::s_AnimeMax = 120;		// 初期微動アニメーションの最大数
const int   CRocket::s_RideAnimeMax = 25;	// 乗り込みアニメーションの最大数
const float CRocket::s_RideAnimeMag = 1.3f;	// 大きさ1.0を基準にそこから加算される大きさ	
const float CRocket::s_RideAnimeShrink = 20;// 乗り込みアニメーションの縮む倍率
const float CRocket::s_RotAdd = 0.02f;		// 向きの増加量
const int   CRocket::s_RotAnimeMax = 4;		// 小刻みアニメーションの最大
const float CRocket::s_MoveMag = 1.05f;		// 移動量の倍率
const float CRocket::s_MoveAdd = 0.01f;		// 移動量の増加量
const float CRocket::s_HeightDis = 40.0f;	// 高さの距離
const int   CRocket::s_FadeModeCountMax = 240;	// フェードのモードのカウント最大
const int   CRocket::s_Firerate = 4;		// 炎の出現割合
const int   CRocket::s_Smokerate = 4;		// 煙の出現割合
int         CRocket::s_nCountPlayer = 0;	// プレイヤーのカウント
bool		CRocket::s_bReady = false;		// 乗る準備クリア

//========================================
// コンストラクタ
//========================================
CRocket::CRocket(void)
{
	Manager::StageObjectMgr()->AddList(this);

	ResetCounter();
	s_bReady = false;
	m_type = TYPE::ROCKET;
	m_width = SIZE_OF_1_SQUARE * 4;
	m_height = SIZE_OF_1_SQUARE * 7;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITD3DCOLOR;
	m_Info.scale = Scale3D(1.0f, 1.0f, 1.0f);
	m_Info.nFlyAnimeCounter = 0;
	m_Info.SmallSpeed = 0.0f;
	m_Info.fScaleMag = 1.0f;
	m_Info.Animstate = CRocket::ANIME_STATE::NONE;
	m_Info.nRideAnimeCounter = 0;
	m_Info.nEffectAnimCounter = 0;
	m_Info.bEffect = false;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Rocket_Body.x");

	m_Info.Firetex.TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_steam_000.png");
	m_Info.Smoketex.TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\Smoke.png");
	m_Info.Firetex.col = Color{ 255,100,0,255 };
	m_Info.Smoketex.col = Color{ 255,255,255,200 };
	m_Info.Firetex.pos = INITD3DXVECTOR3;
	m_Info.Smoketex.pos = INITD3DXVECTOR3;
	m_Info.Firetex.move = D3DXVECTOR3(rand() % 2 - 1 * 0.1f, -0.1f, 0.0f);
	m_Info.Smoketex.move = D3DXVECTOR3(rand() % 2 - 1 * 0.1f, -0.1f, 0.0f);
	m_Info.Firetex.rot = INITD3DXVECTOR3;
	m_Info.Smoketex.rot = INITD3DXVECTOR3;
}

//========================================
// デストラクタ
//========================================
CRocket::~CRocket()
{
	ResetCounter();
}

//========================================
// 初期化
//========================================
HRESULT CRocket::Init(void)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITD3DCOLOR;
	if (m_pos.y < 0)
	{// 下の世界にいるとき反転させる
		m_rot.z -= D3DX_PI;
	}
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_Info.Firetex.pos = m_pos;
		m_Info.Smoketex.pos = m_pos;
	}
	return S_OK;
}

//========================================
// 終了
//========================================
void CRocket::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CRocket::Update(void)
{
	switch (m_Info.Animstate)
	{
	case CRocket::ANIME_STATE::NONE:
		if (s_bReady)
			m_Info.Animstate = ANIME_STATE::RIDE;		// 乗る状態に移行
		break;
	case CRocket::ANIME_STATE::RIDE:
		m_Info.bEffect = true;
		UpdateState_Ride();		// 飛び出し準備状態の更新
		break;
	case CRocket::ANIME_STATE::FLY:
		UpdateState_Fly();		// 飛び出し状態の更新
		break;
	}

	if (m_Info.bEffect)
	{
		m_Info.nEffectAnimCounter++;

		D3DXVECTOR3 SmokeMove1 = D3DXVECTOR3(rand() % 6 - 3, rand() % 4 - 3, 0.0f);
		D3DXVECTOR3 SmokeMove2 = D3DXVECTOR3(rand() % 6 - 3, rand() % 4 - 3, 0.0f);
		D3DXVECTOR3 SmokeMove3 = D3DXVECTOR3(rand() % 6 - 3, rand() % 4 - 3, 0.0f);

		int SmokeLife1 = rand() % 10 + 40;
		int SmokeLife2 = rand() % 20 + 30;
		int SmokeLife3 = rand() % 20 + 30;

		if (m_Info.nEffectAnimCounter % s_Smokerate == 0)
		{
			// 煙のエフェクト
			Manager::EffectMgr()->EffectCreate(m_Info.Smoketex.TexIdx, D3DXVECTOR3(m_pos.x - 2.0f, m_pos.y - s_HeightDis, m_pos.z), Scale3D(20.0f, 20.0f, 20.0f), m_Info.Smoketex.col, SmokeLife1, D3DXVECTOR3(0.0f, 0.0f, 0.1f), SmokeMove1, false, D3DXVECTOR3(1.05f, 1.05f, 1.00f));
			Manager::EffectMgr()->EffectCreate(m_Info.Smoketex.TexIdx, D3DXVECTOR3(m_pos.x, m_pos.y - s_HeightDis, m_pos.z), Scale3D(15.0f, 15.0f, 15.0f), m_Info.Smoketex.col, SmokeLife2, D3DXVECTOR3(0.0f, 0.0f, 0.1f), SmokeMove2, false, D3DXVECTOR3(1.05f, 1.05f, 1.00f));
			Manager::EffectMgr()->EffectCreate(m_Info.Smoketex.TexIdx, D3DXVECTOR3(m_pos.x + 2.0f, m_pos.y - s_HeightDis, m_pos.z), Scale3D(10.0f, 10.0f, 10.0f), m_Info.Smoketex.col, SmokeLife3, D3DXVECTOR3(0.0f, 0.0f, 0.1f), SmokeMove3, false, D3DXVECTOR3(1.05f, 1.05f, 1.00f));
		}
		if (m_Info.nEffectAnimCounter % s_Firerate == 0 && m_Info.Animstate == CRocket::ANIME_STATE::FLY)
		{
			D3DXVECTOR3 move1 = D3DXVECTOR3(0.0f, rand() % 2 - 1, 0.0f);
			D3DXVECTOR3 move2 = D3DXVECTOR3(rand() % 2 - 1, rand() % 2 - 1, 0.0f);

			// 炎のエフェクト
			Manager::EffectMgr()->EffectCreate(m_Info.Firetex.TexIdx, D3DXVECTOR3(m_pos.x - 2.0f, m_pos.y - (s_HeightDis - 3.0f), m_pos.z), Scale3D(20.0f, 20.0f, 20.0f), m_Info.Firetex.col, 120, D3DXVECTOR3(0.0f, 0.0f, 0.1f), move1, false);
			Manager::EffectMgr()->EffectCreate(m_Info.Firetex.TexIdx, D3DXVECTOR3(m_pos.x - 2.0f, m_pos.y - (s_HeightDis - 3.0f), m_pos.z), Scale3D(15.0f, 15.0f, 15.0f), m_Info.Firetex.col, 120, D3DXVECTOR3(0.0f, 0.0f, 0.1f), move2, false);

		}
	}
	
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, m_rot + Rot3D(0.0f, D3DX_PI, 0.0f), m_Info.scale * m_Info.fScaleMag, false)
		->SetOutLineIdx(true);

	RNLib::Text2D().PutDebugLog(String("プレイヤーカウンター：%d", s_nCountPlayer));
}
//========================================
// 乗る状態更新
//========================================
void CRocket::UpdateState_Ride(void)
{
	m_Info.nRideAnimeCounter++;	// アニメーションの増加

	if (m_Info.nRideAnimeCounter <= 1)
	{// 大きくする
	}
	else if (m_Info.nRideAnimeCounter <= s_RideAnimeMax)
	{// 小さくする
		m_Info.fScaleMag -= m_Info.SmallSpeed;	// スケール倍率の減算
		if (m_Info.fScaleMag <= 1.0f)
		{
			m_Info.fScaleMag = 1.0f;
		}
	}
	else if (m_Info.nRideAnimeCounter <= (s_RideAnimeMax + s_RideAnimeShrink) * 4)
	{// アニメーションの移行

		if (s_nCountPlayer == CPlayer::NUM_PLAYER)
		{// プレイヤーが全員乗ったら
			m_Info.Animstate = ANIME_STATE::FLY;	// 飛ぶ状態に移行

			RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::ROCKET_DEPARTURE], _RNC_Sound::CATEGORY::SE, 0.5f, false);
			RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::ROCKET_FIRE], _RNC_Sound::CATEGORY::SE, 0.5f, false);
		}
		m_Info.nRideAnimeCounter = 0;					// 乗るアニメーションカウンターを初期化
	}
}
//========================================
// 飛ぶ状態更新
//========================================
void CRocket::UpdateState_Fly(void)
{
	m_Info.nFlyAnimeCounter++;	// アニメーションの増加
	CInt nCounter = m_Info.nFlyAnimeCounter % s_RotAnimeMax;	// 割合の計算

	// 向きを微動させる
	if (nCounter >= s_RotAnimeMax * 0.5f)
	{
		m_rot.z += s_RotAdd;
	}
	else
	{
		m_rot.z -= s_RotAdd;
	}

	if (m_Info.nFlyAnimeCounter >= s_AnimeMax)
	{
		m_Info.move.y *= s_MoveMag;	// 移動量に倍率をかける

		if (m_pos.y >= 0)
		{// 上の世界にいるとき
			m_Info.move.y += s_MoveAdd;
		}
		else
		{// 下の世界にいるとき
			m_Info.move.y -= s_MoveAdd;
		}
	}

	m_pos += m_Info.move;	// 位置に移動量の増加
}

//========================================
// 搭乗
//========================================
void CRocket::RideOn(void)
{
	if (!s_bReady) return;

	RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::ROCKET_RIDE], _RNC_Sound::CATEGORY::SE, 0.5f, false);

	s_nCountPlayer++;												// プレイヤーの乗った人数の増加
	if (s_nCountPlayer == CPlayer::NUM_PLAYER) s_bReady = true;		// 搭乗OK
	m_Info.fScaleMag = s_RideAnimeMag;								// スケール倍率の設定
	m_Info.SmallSpeed = (m_Info.fScaleMag - 1.0f) / s_RideAnimeMax;	// 小さくなる速度の設定
	m_Info.nRideAnimeCounter = 0;									// 乗るアニメーションカウンターを初期化
	m_Info.nFlyAnimeCounter = 0;									// 飛ぶアニメーションカウンターを初期化
}

//========================================
// 降機
//========================================
void CRocket::RideOff(void)
{
	s_nCountPlayer--;//搭乗者数減少
	if (s_nCountPlayer < 0) s_nCountPlayer = 0;
}