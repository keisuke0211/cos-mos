//========================================
// 
// 伸びる犬の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
#include "../../../_RNLib/Basis/input.h"
#include "../../Character/player.h"


#define MAX_COUNT		(40)	//最大カウント数
#define RADIUS_WIDTH	(0.5f)	//横半径
#define RADIUS_HEIGHT	(0.5f)	//縦半径
#define CORRECT_HEIGHT	(15.0f)	//高さ補正

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampolineクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CExtenddog::CExtenddog(void) {
	Manager::BlockMgr()->AddList(this);

	//初期状態
	m_type = TYPE::TRAMPOLINE;
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLand = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_House.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHip.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHead.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[4] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_nCntExtend = 0;
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
}

//========================================
// デストラクタ
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Init(void) {

}

//========================================
// 終了処理
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// 更新処理
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Update(void) {

	//土台モデル
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);


	if (m_state != STATE::NONE)
	{//伸びる犬が作動している

		if (m_state == STATE::DOWN_LAND)
		{
			m_nCntShrink = 0;	// 初期化

			m_nCntExtend++;		// 増加

								//割合計算
			float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntExtend, MAX_COUNT);


			if (fCountRate >= 1.0f)
			{
				fCountRate = 1.0f;
			}

			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
				->SetOutLine(true);
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y + (m_fcurrenty - fCountRate * (m_fcurrenty)), m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
				->SetOutLine(true);
		}

	}
	else if (m_state == STATE::NONE)
	{//伸びる犬が作動していない

		m_nCntExtend = 0;	// 初期化

		m_nCntShrink++;		// 増加

							//割合計算
		float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntShrink, MAX_COUNT);

		if (fCountRate >= 1.0f)
		{
			fCountRate = 1.0f;
		}

		//y座標の更新
		m_fcurrenty = m_pos.y + (CORRECT_HEIGHT + fCountRate * (CORRECT_HEIGHT * 2));

		RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - 10.0f, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
			->SetOutLine(true);
		RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_fcurrenty, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
			->SetOutLine(true);
	}

	//当たり判定
	Collision();
}
//========================================
// 描画処理
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Draw(void) {

}
//========================================
// 当たり判定処理
// Author:RYUKI FUJIWARA
//========================================
void CExtenddog::Collision(void) {

	// 状態の設定
	m_state = STATE::NONE;

	//プレイヤー情報取得
	CPlayer::Info *p1, *p2;
	CPlayer *pPlayer = CMode_Game::GetPlayer();
	if (pPlayer == NULL)
		return;
	pPlayer->GetInfo(p1, p2);

	float width, height;

	width = m_width * RADIUS_WIDTH;
	height = m_height * RADIUS_HEIGHT;

	//**************************************
	//1p伸びる犬当たり判定
	//**************************************
	if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width && p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height)
	{//土台の範囲内に着地で入った

		if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - height
			&& p2->side == CPlayer::WORLD_SIDE::BEHIND)
		{//2pが乗っているか


		}
		else if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + height
			&& p2->side == CPlayer::WORLD_SIDE::FACE)
		{

		}

		if (p1->side == CPlayer::WORLD_SIDE::BEHIND)
		{//伸びる犬が作動していない

			m_state = STATE::DOWN_LAND;
		}


	}
	//**************************************
	//2p伸びる犬当たり判定
	//**************************************
	if (p2->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p2->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
		&& p2->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + m_height && p2->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - m_height)
	{//土台の範囲内に着地で入った

		if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p1->pos.y - CPlayer::SIZE_HEIGHT <= m_pos.y + height
			&& p1->side == CPlayer::WORLD_SIDE::FACE)
		{//1pが乗っているか


		}
		else if (p1->pos.x + CPlayer::SIZE_WIDTH >= m_pos.x - width&& p1->pos.x - CPlayer::SIZE_WIDTH <= m_pos.x + width
			&& p1->pos.y + CPlayer::SIZE_HEIGHT >= m_pos.y - height
			&& p1->side == CPlayer::WORLD_SIDE::BEHIND)
		{

		}

		if (p2->side == CPlayer::WORLD_SIDE::BEHIND)
		{//伸びる犬が作動していない

			m_state = STATE::DOWN_LAND;
		}


	}
}