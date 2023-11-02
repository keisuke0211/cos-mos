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

const int   CRocket::s_AnimeMax = 120;		// 初期微動アニメーションの最大数
const int   CRocket::s_RideAnimeMax = 25;	// 乗り込みアニメーションの最大数
const float CRocket::s_RideAnimeMag = 1.3f;	// 大きさ1.0を基準にそこから加算される大きさ	
const float CRocket::s_RideAnimeShrink = 20;// 乗り込みアニメーションの縮む倍率
const float CRocket::s_RotAdd = 0.02f;		// 向きの増加量
const int   CRocket::s_RotAnimeMax = 4;		// 小刻みアニメーションの最大 
const float CRocket::s_MoveMag = 1.05f;		// 移動量の倍率
const float CRocket::s_MoveAdd = 0.01f;		// 移動量の増加量
const int   CRocket::s_FadeModeCountMax = 120;	// フェードのモードのカウント最大
int         CRocket::s_nCountPlayer = 0;	// プレイヤーのカウント

//========================================
// コンストラクタ
//========================================
CRocket::CRocket(void)
{
	Manager::BlockMgr()->AddList(this);

	s_nCountPlayer = 0;

	m_type = TYPE::ROCKET;
	m_width = SIZE_OF_1_SQUARE*3;
	m_height = SIZE_OF_1_SQUARE*3;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.col = INITD3DCOLOR;
	m_Info.scale = Scale3D(1.0f,1.0f,1.0f);
	m_Info.nFlyAnimeCounter = 0;
	m_Info.SmallSpeed = 0.0f;
	m_Info.fScaleMag = 1.0f;
	m_Info.Animstate = CRocket::ANIME_STATE::NONE;
	m_Info.nRideAnimeCounter = 0;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Rocket_Body.x");
}

//========================================
// デストラクタ
//========================================
CRocket::~CRocket()
{
	s_nCountPlayer = 0;
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
	//乗るアニメーションに移動
	if (RNLib::Input().GetKeyPress(DIK_R))
	{
		m_Info.Animstate = CRocket::ANIME_STATE::RIDE;
		Ride();
	}

	switch (m_Info.Animstate)
	{
	case CRocket::ANIME_STATE::NONE:
		break;
	case CRocket::ANIME_STATE::RIDE:
		UpdateState_Ride();		// 飛び出し準備状態の更新
		break;
	case CRocket::ANIME_STATE::FLY:
		UpdateState_Fly();		// 飛び出し準備状態の更新
		break;

	}

	RNLib::Model().Put(m_pos, m_rot, m_Info.scale * m_Info.fScaleMag, m_Info.nModelIdx, false)
		->SetOutLine(true);

	RNLib::Text2D().PutDebugLog(CreateText("プレイヤーカウンター：%d", s_nCountPlayer));
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

		m_Info.Animstate = CRocket::ANIME_STATE::NONE;	// なしに変更	
		m_Info.nRideAnimeCounter = 0;					// 乗るアニメーションカウンターを初期化
	}
}
//========================================
// 飛ぶ状態更新
//========================================
void CRocket::UpdateState_Fly(void)
{
	int nCounter;

	m_Info.nFlyAnimeCounter++;	// アニメーションの増加
	nCounter = m_Info.nFlyAnimeCounter % s_RotAnimeMax;	// 割合の計算

	// モードの切り替え	
	if (m_Info.nFlyAnimeCounter >= s_FadeModeCountMax)
	{
		int stage = Manager::StgEd()->GetType()->nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

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
// 描画
//========================================
void CRocket::Draw(void)
{

}
//========================================
// 乗ってる状態
//========================================
void CRocket::Ride(void)
{
	s_nCountPlayer++;												// プレイヤーの乗った人数の増加
	m_Info.fScaleMag = s_RideAnimeMag;								// スケール倍率の設定
	m_Info.SmallSpeed = (m_Info.fScaleMag - 1.0f) / s_RideAnimeMax;	// 小さくなる速度の設定
	m_Info.nRideAnimeCounter = 0;									// 乗るアニメーションカウンターを初期化
	m_Info.nFlyAnimeCounter = 0;									// 飛ぶアニメーションカウンターを初期化

	m_Info.Animstate = ANIME_STATE::RIDE;		// 乗る状態に移行
	if (s_nCountPlayer == CPlayer::NUM_PLAYER)
	{// プレイヤーが全員乗ったら
		m_Info.Animstate = ANIME_STATE::FLY;	// 飛ぶ状態に移行
	}
}