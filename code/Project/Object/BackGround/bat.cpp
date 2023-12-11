//========================================
// 
// 蝙蝠の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "bat.h"
#include "../../main.h"
#include"../../Character/player.h"
#define	MOVE_UP		(D3DXVECTOR3(1.0f,0.3f,0.0f))
#define	MOVE_DOWN	(D3DXVECTOR3(1.0f,-0.3f,0.0f))
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CBatクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CBat::CBat(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::BACKGROUND;	// 種類の設定
	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;

	// 各情報の初期化
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;

	m_doll = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load("data/SETUP/Bat.txt"));
	m_State = CBat::STATE::STATE_FLY;
	m_nIdxMotion = 0;
	m_nFlyCounter = 0;
	m_nStayCounter = 0;
	m_bUpDown = false;
}

//========================================
// デストラクタ
//========================================
CBat::~CBat(void) {

	delete m_doll;
}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CBat::Init(void) {
	if (m_pos.y < 0.0f)
	{
		m_move = MOVE_DOWN;
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 3.16f);
	}
	else
	{
		m_move = MOVE_UP;
	}
}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CBat::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CBat::Update(void) {
	 
	//CPlayer *Player = CPlayer::GetInfo(CPlayer::WORLD_SIDE::FACE);

	// 状態
	switch (m_State)
	{
	case CBat::STATE::STATE_DEFAULT:
		if (m_nStayCounter == 0)
		{
			// モーションの設定
			m_nIdxMotion = RNLib::Motion3D().Load("data/MOTION/Bat/Default.txt");
			m_doll->SetMotion(m_nIdxMotion);
			m_nFlyCounter = 0;
		}

		m_nStayCounter++;	// アニメーションカウンターの増加
		break;
	case CBat::STATE::STATE_FLY:
		if (m_nFlyCounter == 0)
		{
			// モーションの設定
			m_nIdxMotion = RNLib::Motion3D().Load("data/MOTION/Bat/Fly.txt");
			m_doll->SetMotion(m_nIdxMotion);
			m_nStayCounter = 0;
		}
	
		if (m_nFlyCounter % 30 == 0)
		{
			if (m_bUpDown == false)
			{
				m_bUpDown = true;
			}
			else
			{
				m_bUpDown = false;
			}
		}

		if (m_bUpDown == true)
		{
			m_move.y = MOVE_DOWN.y;
		}
		else
		{
			m_move.y = MOVE_UP.y;

		}
		m_pos += m_move;	// 位置の増加

		m_nFlyCounter++;	// アニメーションカウンターの増加
		break;
	}

	m_doll->SetPos(m_pos);	// 位置の設定
}