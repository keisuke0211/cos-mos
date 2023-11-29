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
#define MAX_COUNT		(90)						//最大カウント数
#define MAX_ROT_SPEED	(30.0f / (float) MAX_COUNT)	//最大回転速度
#define MAX_COLOR		(200)						//最大カラー値
#define ADDSUB_COLOR	(10)						//和差カラー値

int CGoalGate::m_num = 0;
int CGoalGate::m_numEntry = 0;
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
	m_bEntry = false;
	m_bScale = false;
	m_col = Color{ 200,0,0,255 };
	m_Rainbow = RAINBOW::RED;
	m_num++;
	m_bStartGate = false;
	m_bCloseGate = false;

	//モデルとテクスチャ
	m_modelIdx = RNLib::Model().Load("data\\MODEL\\GoalGate.x");
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\effect000.jpg");
}

//========================================
// デストラクタ
//========================================
CGoalGate::~CGoalGate(void) {
	m_num--;
}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Init(void) {

	m_state = STATE::SMALL;
	m_Rainbow = RAINBOW::RED;

	m_RainbowCol[(int)RAINBOW::RED] =		{ 255,0,0,255 };
	m_RainbowCol[(int)RAINBOW::PURPLE] =	{ 255,0,255,255 };
	m_RainbowCol[(int)RAINBOW::BLUE] =		{ 0,0,255,255 };
	m_RainbowCol[(int)RAINBOW::LIGHT_BLUE] ={ 0,255,255,255 };
	m_RainbowCol[(int)RAINBOW::GREEN] =		{ 0,255,0,255 };
	m_RainbowCol[(int)RAINBOW::YELLOW] =	{ 255,255,0,255 };
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
void CGoalGate::Update(void)
{

	StateUpdate();
	ColUpdate();

	float fCountRateX, fCountRateY, fCountRateZ;
	CountRate(&fCountRateX, &fCountRateY,&fCountRateZ);
	
	//モデル配置
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx, m_pos, m_rot, Scale3D(m_scale.x * fCountRateX, m_scale.y * fCountRateY, m_scale.z * fCountRateZ), false)
		->SetCol(m_col);
}
//========================================
// 描画処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Draw(void) 
{

}
//========================================
// 状態更新処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::StateUpdate(void)
{
	if (m_state != STATE::MAX)
	{
		if (m_pos.y > 0)
			m_rot.z += 0.05f;
		
		else if (m_pos.y < 0)
			m_rot.z -= 0.05f;
	}

	if (m_num == m_numEntry || m_bCloseGate == true)
	{
		m_state = STATE::MAX;

		float CountRateRot = MAX_ROT_SPEED * CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		float CntEffRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);

		if (m_pos.y > 0)
			m_rot.z += 0.6f - CountRateRot;

		else if(m_pos.y < 0)
			m_rot.z -= 0.6f - CountRateRot;

		if (m_nCnt > 0)
		{
			if (!m_bStartGate)
			{
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[1], m_pos, INIT_EFFECT_SCALE * CntEffRate, m_RainbowCol[rand() % 6], CParticle::TYPE::TYPE_SPIN, 30, m_rot);
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[1], m_pos, INIT_EFFECT_SCALE * CntEffRate, m_RainbowCol[rand() % 6], CParticle::TYPE::TYPE_SPIN, 30, D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z + D3DX_PI));
			}

			m_nCnt--;
		}
		else
		{
			m_state = STATE::NONE;

			if (m_bStartGate == false)
			{
				m_numEntry--;
			}

			for (int ParCnt = 0; ParCnt < 16; ParCnt++)
			{
				Manager::EffectMgr()->ParticleCreate(m_TexIdx[0], m_pos, INIT_EFFECT_SCALE, m_RainbowCol[rand() % 6]);
			}

			Delete();
		}
	}
	else if (m_state == STATE::SMALL )
	{
		m_nCnt--;

		if (m_nCnt < MAX_COUNT * 0.8)
			m_state = STATE::GROW;
	}
	else if (m_state == STATE::GROW)
	{
		m_nCnt++;

		if (m_nCnt > MAX_COUNT)
		{
			m_state = STATE::SMALL;

			if (m_bStartGate)
				m_bCloseGate = true;
		}	
	}
}
//========================================
// 色更新処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::ColUpdate(void)
{
	switch (m_Rainbow)
	{
	case RAINBOW::RED:
		if (m_col.b < MAX_COLOR)
			m_col.b += ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::PURPLE;
		break;

	case RAINBOW::PURPLE:
		if (m_col.r > 0)
			m_col.r -= ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::BLUE;
		break;

	case RAINBOW::BLUE:
		if (m_col.g < MAX_COLOR)
			m_col.g += ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::LIGHT_BLUE;
		break;

	case RAINBOW::LIGHT_BLUE:
		if (m_col.b > 0)
			m_col.b -= ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::GREEN;
		break;

	case RAINBOW::GREEN:
		if (m_col.r < MAX_COLOR)
			m_col.r += ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::YELLOW;
		break;

	case RAINBOW::YELLOW:
		if (m_col.g > 0)
			m_col.g -= ADDSUB_COLOR;
		else
			m_Rainbow = RAINBOW::RED;
		break;
	}
}
//========================================
// カウントレート処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::CountRate(float *CountRateX, float *CountRateY, float *CountRateZ)
{
	if (m_state == STATE::MAX)
		m_bEntry = false;

	if (m_bEntry == true && m_bStartGate == false)
	{
		//割合計算
		*CountRateX = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntEtrX, ETR_CNT);
		*CountRateY = CEase::Easing(CEase::TYPE::IN_SINE, m_nCntEtrY, ETR_CNT);

		if (m_bScale == false)
		{
			m_nCntEtrX--;
			m_nCntEtrY++;

			if (m_nCntEtrX <= ETR_CNT * 0.5 && m_nCntEtrY >= ETR_CNT)
				m_bScale = true;
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
		*CountRateX = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		*CountRateY = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
	}

	*CountRateZ = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
}
//========================================
// エントリー設定処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::SetEntry(bool bEntry) 
{
	m_bEntry = bEntry;

	if (bEntry == true) {
		m_numEntry++;
		m_nCntEtrX = ETR_CNT;
		m_nCntEtrY = ETR_CNT * 0.5;

		for (int ParCnt = 0; ParCnt < 16; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(m_TexIdx[0], m_pos, INIT_EFFECT_SCALE, INITCOLOR);
		}
	}
}