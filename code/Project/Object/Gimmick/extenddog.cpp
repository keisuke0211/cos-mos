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
	m_type = TYPE::EXTEND_DOG;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bLand = false;
	m_bShrink = false;
	m_bInversion = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_House.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHip.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_FeetHead.x");
	m_modelIdx[3] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[4] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_modelIdx[5] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Body.x");
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
	m_nHeight = 0;
	m_HeadPos = INITD3DXVECTOR3;
	m_HeadPosOid = INITD3DXVECTOR3;
	m_BodyPos = INITD3DXVECTOR3;
	m_HipPos = INITD3DXVECTOR3;
}

//========================================
// デストラクタ
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Init(void) {
	m_HeadPos.y = m_nHeight * SIZE_OF_1_SQUARE;
	m_BodyPos = m_HeadPos;
}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Update(void) {

	m_HeadPosOid = m_HipPos;

	//土台モデル
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[0], false)
		->SetCol(m_color)
		->SetOutLine(true);

	if (m_state == STATE::DOWN_LAND)
	{//伸びる犬が作動している

		if (m_bShrink == false)
		{
			// 縮むカウント
			m_nCntShrink--;
			if (m_nCntShrink <= 0)
				m_nCntShrink = 0;
		}
		else
		{
			// 縮むカウント
			m_nCntShrink++;
			if (m_nCntShrink >= MAX_COUNT)
				m_nCntShrink = MAX_COUNT;
		}
	}
	else if (m_state == STATE::RETURN)
	{//伸びる犬が作動していない

		if (m_bShrink == false)
		{
			// 縮むカウント
			m_nCntShrink++;
			if (m_nCntShrink >= MAX_COUNT){
				m_nCntShrink = MAX_COUNT;
				m_state = STATE::NONE;
			}
		}
		else
		{
			// 縮むカウント
			m_nCntShrink--;
			if (m_nCntShrink <= 0) {
				m_nCntShrink = 0;
				m_state = STATE::NONE;

			}
		}
	}

	// 割合計算 
	float fCountRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);
	if (m_bInversion == false)
	{
		//y座標の更新
		float fDowncurrenty = m_pos.y + (SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		// 尻
		m_HipPos.y = m_pos.y - HIP_POS;
		RNLib::Model().Put(m_HipPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
			->SetOutLine(true);

		// 頭
		m_HeadPos.y = fDowncurrenty;
		RNLib::Model().Put(m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
			->SetOutLine(true);

		// 体
		m_BodyPos.y = m_pos.y + fDowncurrenty * 0.5f + SIZE_OF_1_SQUARE * 0.4f;
		RNLib::Model().Put(m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Scale3D(1.0f, fDowncurrenty * 9, 1.0f), m_modelIdx[5], false)
			->SetOutLine(true);
	}
	else
	{// 反転状態の時
		//y座標の更新
		float fDowncurrenty = m_pos.y - (SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		// 尻
		m_HipPos.y = m_pos.y + HIP_POS;
		RNLib::Model().Put(m_HipPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[3], false)
			->SetOutLine(true);

		// 頭
		m_HeadPos.y = fDowncurrenty;
		RNLib::Model().Put(m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_modelIdx[4], false)
			->SetOutLine(true);

		// 体
		m_BodyPos.y = m_pos.y - fDowncurrenty * 0.5f - SIZE_OF_1_SQUARE * 0.4f;
		RNLib::Model().Put(m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Scale3D(1.0f, fDowncurrenty * 9, 1.0f), m_modelIdx[5], false)
			->SetOutLine(true);
	}
}
//========================================
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Draw(void) {

}