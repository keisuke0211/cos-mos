//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "trampoline.h"
#include "../../main.h"
#include "../../Character/player.h"

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
	Manager::StageObjectMgr()->AddList(this);

	//初期状態
	m_type = TYPE::TRAMPOLINE;
	m_width = SIZE_OF_1_SQUARE * 2.0f;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_modelIdx[Parts_BASE] = RNLib::Model().Load("data\\MODEL\\Spring_Body.x");
	m_modelIdx[Parts_MASH] = RNLib::Model().Load("data\\MODEL\\Spring_Footing.x");
	m_modelIdx[Parts_SPRING] = RNLib::Model().Load("data\\MODEL\\Spring_Spring.x");
	m_modelIdx[Parts_EYE] = RNLib::Model().Load("data\\MODEL\\Spring_Eye.x");
	m_fJamp = 8.0f;
	m_nSpringCounter = 0;
	m_pSpringPos = NULL;
	m_fSpringForce = 0.0f;
}

//========================================
// デストラクタ
//========================================
CTrampoline::~CTrampoline(void) {

	//ばね位置格納変数開放
	if (m_pSpringPos != NULL) {
		delete[] m_pSpringPos;
		m_pSpringPos = NULL;
	}
}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CTrampoline::Init(void) {
	//ばねの位置を保存する変数を確保
	if (m_pSpringPos == NULL)
		m_pSpringPos = new D3DXVECTOR3[(int)STATE::MAX];

	for (int nCnt = 0; nCnt < (int)STATE::MAX; nCnt++)
	{
		m_pSpringPos[nCnt] = INITD3DXVECTOR3;
	}

	m_pSpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
	m_pSpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
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

	if (m_nSpringCounter > 0)
	{
		m_nSpringCounter--;
		if (m_nSpringCounter == 0) {
			m_state = STATE::NONE;
			m_pSpringPos[0] = D3DXVECTOR3(m_pos.x, m_pos.y + CORRECT_HEIGHT, m_pos.z);
			m_pSpringPos[1] = D3DXVECTOR3(m_pos.x, m_pos.y - CORRECT_HEIGHT, m_pos.z);
		}
	}

	//割合計算
	const float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nSpringCounter, MAX_COUNT);

	if (m_state == STATE::UP_LAND)
	{
		m_pSpringPos[1].y = m_pos.y - CORRECT_HEIGHT - m_fJamp * fCountRate;
	}
	else if (m_state == STATE::DOWN_LAND)
	{
		m_pSpringPos[0].y = m_pos.y + CORRECT_HEIGHT + m_fJamp * fCountRate;
	}

	//モデル配置
	PutModel();
}

//========================================
// モデル配置処理
// Author:HIRASAWA SHION
//========================================
void CTrampoline::PutModel(void)
{
	//土台モデル
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_BASE], m_pos, INITD3DXVECTOR3,  false)
		->SetCol(m_color)
		->SetOutLineIdx(true);

	//キノコ
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_MASH], m_pSpringPos[0], INITD3DXVECTOR3, false)
		->SetOutLineIdx(true);
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_MASH], m_pSpringPos[1], INVERSEVECTOR3,  false)
		->SetOutLineIdx(true);

	//ばね
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_SPRING], m_pSpringPos[0], INVERSEVECTOR3,false)
		->SetOutLineIdx(true);
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_SPRING], m_pSpringPos[1], INITD3DXVECTOR3,  false)
		->SetOutLineIdx(true);

	//目玉モデル
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_EYE], D3DXVECTOR3(m_pos.x + CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), INITD3DXVECTOR3,  false)
		->SetCol(m_color)
		->SetOutLineIdx(true);
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[Parts_EYE], D3DXVECTOR3(m_pos.x - CORRECT_WIDTH, m_pos.y, m_pos.z - CORRECT_HEIGHT), INITD3DXVECTOR3, false)
		->SetCol(m_color)
		->SetOutLineIdx(true);
}