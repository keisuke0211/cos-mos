//========================================
// 
// 伸びる犬の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
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
	Manager::StageObjectMgr()->AddList(this);

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

	m_HeadPosOid = m_HeadPos;
	
	CObject *obj = NULL;

	while (Manager::StageObjectMgr()->ListLoop(&obj)) {
		//取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		TYPE type = stageObj->GetType();
		if (type == CStageObject::TYPE::BLOCK)
		{// 想定された種類の時

			D3DXVECTOR3 stagepos = stageObj->GetPos();
			D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());

			if (stagepos.x + (stagesize.x * 0.5f) >= m_HeadPos.x &&
				stagepos.x - (stagesize.x * 0.5f) <= m_HeadPos.x)
			{// 範囲内にいるとき
				if (m_pos.y <= m_HeadPos.y && m_bInversion == false)
				{
					if (m_pos.y <= stagepos.y)
						m_pos = stagepos;
				}
				if (m_pos.y >= m_HeadPos.y && m_bInversion == true)
				{
					if (m_pos.y >= stagepos.y)
						m_pos = stagepos;
				}
			}
		}
	}

	if (m_state == STATE::NONE)
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
		float fDowncurrenty = (SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		if (fDowncurrenty < 0.0f)
		{
			m_rot.z = 3.16f;
		}

		// 尻
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[3], m_HipPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetOutLineIdx(true);

		// 頭
		m_HeadPos.y = fDowncurrenty + m_pos.y;
		
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[4], m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetOutLineIdx(true);

		// 体
		m_BodyPos.y = (m_HeadPos.y + m_pos.y - SIZE_OF_1_SQUARE * 0.5f) / 2;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[5], m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight + (SIZE_OF_1_SQUARE * m_nHeight * (1.0f - fCountRate))) * 7, 1.0f),  false)
			->SetOutLineIdx(true); 
	}
	else
	{// 反転状態の時
		// y座標の更新
		float fDowncurrenty = -(SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

		if (fDowncurrenty > 0.0f)
		{
			m_rot.z = 3.16f;
		}

		// 尻
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[3], m_HipPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), false)
			->SetOutLineIdx(true);

		// 頭
		m_HeadPos.y = fDowncurrenty + m_pos.y;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[4], m_HeadPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI),  false)
			->SetOutLineIdx(true);

		// 体
		//m_BodyPos.y = (m_HeadPos.y + m_pos.y - SIZE_OF_1_SQUARE * 0.5f) * 0.5f;
		m_BodyPos.y = (m_HeadPos.y + m_pos.y + SIZE_OF_1_SQUARE * 0.5f) * 0.5f;
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[5], m_BodyPos, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI), Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight - (SIZE_OF_1_SQUARE * (m_nHeight - 1.5) * fCountRate)) * 11, 1.0f),  false)
			->SetOutLineIdx(true);
	}
}
//========================================
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Draw(void) {

}