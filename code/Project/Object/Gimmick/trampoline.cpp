//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "trampoline.h"
#include "../../main.h"
#include "../../Character/player.h"


#define MAX_COUNT		(12)	//最大カウント数
#define PLAYER_FLAME	(6)		//プレイヤーの高さ到達カウント
#define RADIUS_WIDTH	(0.5f)	//横半径
#define RADIUS_HEIGHT	(0.5f)	//縦半径
#define CORRECT_WIDTH	(8.0f)	//高さ補正
#define CORRECT_HEIGHT	(6.0f)	//高さ補正

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampolineクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CTrampoline::CTrampoline(void) {
	Manager::BlockMgr()->AddList(this);

	//初期状態
	m_type = TYPE::TRAMPOLINE;
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\Spring_Body.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\Spring_Footing.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\Spring_Spring.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\Spring_Eye.x");
	m_fJamp = 8.0f;
	m_nCnt = 1;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_SpringPos[nCnt] = INITD3DXVECTOR3;
	}
}

//========================================
// デストラクタ
//========================================
CTrampoline::~CTrampoline(void) {

}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Init(void) {
	m_SpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
	m_SpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
}

//========================================
// 終了処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Uninit(void) {

}

//========================================
// 更新処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Update(void) {

	//土台モデル
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);

	//目玉モデル
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x + CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);
	RNLib::Model().Put(D3DXVECTOR3(m_pos.x - CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
		->SetCol(m_color)
		->SetOutLine(true);

	if (m_nCnt > 0) 
	{
		m_nCnt--;
		if (m_nCnt == 0) {
			m_state = STATE::NONE;
			m_SpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
			m_SpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
		}
	}

	if (m_state != STATE::NONE)
	{//トランポリンが作動している

		//割合計算
		float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		
		if (m_state == STATE::UP_LAND)
		{
			//キノコ
			RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
				->SetOutLine(true);

			m_SpringPos[1].y = m_pos.y - CORRECT_HEIGHT - m_fJamp * fCountRate;
			RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[1], false)
				->SetOutLine(true);

			//ばね
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y - m_fJamp * fCountRate, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[2], false)
				->SetOutLine(true);
		}
		else if (m_state == STATE::DOWN_LAND)
		{
			//キノコ
			m_SpringPos[0].y = m_pos.y + CORRECT_HEIGHT + m_fJamp * fCountRate;
			RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
				->SetOutLine(true);

			RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f,D3DX_PI), m_modelIdx[1], false)
				->SetOutLine(true);

			//ばね
			RNLib::Model().Put(D3DXVECTOR3(m_pos.x, m_pos.y + m_fJamp * fCountRate, m_pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[2], false)
				->SetOutLine(true);
		}
	}
	else if (m_state == STATE::NONE)
	{//トランポリンが作動していない
		RNLib::Model().Put(m_SpringPos[0], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[1], false)
			->SetOutLine(true);
		RNLib::Model().Put(m_SpringPos[1], D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), m_modelIdx[1], false)
			->SetOutLine(true);
	}

	//当たり判定
	Collision();
}
//========================================
// 描画処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Draw(void) {

}
//========================================
// 当たり判定処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Collision(void) {

	//プレイヤー情報取得
	CPlayer::Info *Player[2];
	CPlayer *pPlayer = CMode_Game::GetPlayer();
	if (pPlayer == NULL)
		return;
	pPlayer->GetInfo(Player[0], Player[1]);

	float width, height, SIZE_HEIGHT;

	width = m_width * RADIUS_WIDTH;
	height = m_height * 1.5f;
	SIZE_HEIGHT = CPlayer::SIZE_HEIGHT * 1.5f;

	//**************************************
	//トランポリン当たり判定
	//**************************************
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (Player[nCnt]->bGround == false && Player[nCnt]->TramColliRot == CPlayer::COLLI_ROT::NONE
			&& ((Player[nCnt]->pos.x + CPlayer::SIZE_WIDTH >= m_SpringPos[0].x - width && Player[nCnt]->pos.x - CPlayer::SIZE_WIDTH <= m_SpringPos[0].x + width
				&& Player[nCnt]->pos.y - SIZE_HEIGHT <= m_SpringPos[0].y + m_height && Player[nCnt]->pos.y + SIZE_HEIGHT >= m_SpringPos[0].y - m_height)
			|| (Player[nCnt]->pos.x + CPlayer::SIZE_WIDTH >= m_SpringPos[1].x - width && Player[nCnt]->pos.x - CPlayer::SIZE_WIDTH <= m_SpringPos[1].x + width
				&& Player[nCnt]->pos.y - SIZE_HEIGHT <= m_SpringPos[1].y + m_height && Player[nCnt]->pos.y + SIZE_HEIGHT >= m_SpringPos[1].y - m_height)))
		{// ばねの範囲内に着地で入った時
			int idx = nCnt + 1;

			// ループ制御
			IntLoopControl(&idx, 2, 0);

			//相手がばねに乗っているか
			if (Player[idx]->TramColliRot == CPlayer::COLLI_ROT::UNDER && Player[idx]->side == CPlayer::WORLD_SIDE::BEHIND
				&& Player[idx]->pos.x + CPlayer::SIZE_WIDTH >= m_SpringPos[1].x - width && Player[idx]->pos.x - CPlayer::SIZE_WIDTH <= m_SpringPos[1].x + width
				&& Player[idx]->pos.y - CPlayer::SIZE_HEIGHT <= m_SpringPos[1].y + m_height && Player[idx]->pos.y + CPlayer::SIZE_HEIGHT >= m_SpringPos[1].y - m_height)
			{
				pPlayer->SetTrampolineJump(Player[idx], Player[nCnt]->fMaxHeight);
			}
			else if (Player[idx]->TramColliRot == CPlayer::COLLI_ROT::OVER && Player[idx]->side == CPlayer::WORLD_SIDE::FACE
				&& Player[idx]->pos.x + CPlayer::SIZE_WIDTH >= m_SpringPos[0].x - width && Player[idx]->pos.x - CPlayer::SIZE_WIDTH <= m_SpringPos[0].x + width
				&& Player[idx]->pos.y - CPlayer::SIZE_HEIGHT <= m_SpringPos[0].y + m_height && Player[idx]->pos.y + CPlayer::SIZE_HEIGHT >= m_SpringPos[0].y - m_height)
			{
				pPlayer->SetTrampolineJump(Player[idx], Player[nCnt]->fMaxHeight);
			}

			if (m_state == STATE::NONE
				&& Player[nCnt]->side == CPlayer::WORLD_SIDE::BEHIND)
			{
				m_state = STATE::DOWN_LAND;
				m_nCnt = MAX_COUNT;
			}
			else if (m_state == STATE::NONE
				&& Player[nCnt]->side == CPlayer::WORLD_SIDE::FACE)
			{//トランポリンが作動していない

				m_state = STATE::UP_LAND;
				m_nCnt = MAX_COUNT;
			}
		}
	}
}