//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "goalgate.h"
#include "../../main.h"
#include "../../Character/player.h"

//マクロ定義
#define MAX_COUNT		(60)	//最大カウント数
#define ETR_CNT			(16)	//最大カウント数

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CGoalGateクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CGoalGate::CGoalGate(void) {
	Manager::StageObjectMgr()->AddList(this);

	//初期状態
	m_scale = Scale3D(3.0f,3.0f,6.0f);
	m_nCnt = MAX_COUNT;
	m_state = STATE::NONE;
	m_type = TYPE::GOALGATE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 2.0f;
	m_modelIdx = RNLib::Model().Load("data\\MODEL\\GoalGate.x");
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");
	m_bEntry = false;
	m_bScale = false;
}

//========================================
// デストラクタ
//========================================
CGoalGate::~CGoalGate(void) {

}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Init(void) {

	m_state = STATE::SMALL;
}

//========================================
// 終了処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Uninit(void) {

}

//========================================
// 更新処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Update(void) {

	if (m_state != STATE::MAX)
	{
		m_rot.z += 0.05f;
	}
	else
	{
		m_rot.z += 0.35f;
	}	

	if (RNLib::Input().GetKeyTrigger(DIK_U))
	{
		m_state = STATE::MAX;
	}
	if (RNLib::Input().GetKeyTrigger(DIK_I))
	{
		m_bEntry = true;
		m_nCntEtrX = ETR_CNT;
		m_nCntEtrY = ETR_CNT * 0.5;
	}

	if (m_state == STATE::SMALL)
	{
		m_nCnt--;

		if (m_nCnt < MAX_COUNT * 0.8)
		{
			m_state = STATE::GROW;
		}
	}
	else if (m_state == STATE::GROW)
	{
		m_nCnt++;

		if (m_nCnt > MAX_COUNT)
		{
			m_state = STATE::SMALL;
		}
	}
	else if (m_state == STATE::MAX)
	{
		if (m_nCnt > 0)
		{
			m_nCnt--;
		}
		else
		{
			m_state = STATE::NONE;

			for (int ParCnt = 0; ParCnt < 16; ParCnt++)
			{
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[0], m_pos, INIT_EFFECT_SCALE, INITCOLOR);
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[1], m_pos, INIT_EFFECT_SCALE, INITCOLOR);
			}	
		}
	}

	float fCountRateX,fCountRateY;

	if (m_bEntry == true)
	{
		//割合計算
		fCountRateX = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntEtrX, ETR_CNT);
		fCountRateY = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntEtrY, ETR_CNT);

		if (m_bScale == false)
		{
			m_nCntEtrX--;
			m_nCntEtrY++;

			if (m_nCntEtrX <= ETR_CNT * 0.5 && m_nCntEtrY >= ETR_CNT)
			{
				m_bScale = true;
			}
		}
		else
		{
			if (m_nCntEtrX < ETR_CNT)
			{
				m_nCntEtrX++;
				m_nCntEtrY--;
			}
			else
			{
				m_nCntEtrY++;
			}

			if (m_nCntEtrX == ETR_CNT && m_nCntEtrY == ETR_CNT)
			{
				m_nCnt = MAX_COUNT;
				m_bEntry = false;	
				m_bScale = false;
			}
		}
	}
	else
	{
		//割合計算
		fCountRateX = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		fCountRateY = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
	}
	
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx, m_pos, m_rot, Scale3D(m_scale.x * fCountRateX, m_scale.y * fCountRateY, m_scale.z), false);
}
//========================================
// 描画処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Draw(void) 
{

}