//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "goalgate.h"
#include "../../main.h"
#include "../../Character/player.h"
#include "../../resource.h"

//マクロ定義
#define MAX_COUNT		(90)						//最大カウント数
#define MAX_ROT_SPEED	(30.0f / (float) MAX_COUNT)	//最大回転速度
#define MAX_COLOR		(200)						//最大カラー値
#define ADDSUB_COLOR	(10)						//和差カラー値

int CGoalGate::s_num = 0;
int CGoalGate::s_numEntry = 0;
int CGoalGate::s_modelIdx = NONEDATA;
int CGoalGate::s_TexIdx[2] = {NONEDATA, NONEDATA};
int CGoalGate::s_nEscapeGuideTexID = NONEDATA;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CGoalGateクラス
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
	s_num++;
	m_bStartGate = false;
	m_bCloseGate = false;
	m_nEntryNo = NONEDATA;
	m_nEntryCounter = 0;
	m_GuideAlpha = 0; // ガイドUIのα値

	//モデルとテクスチャ
	if(s_modelIdx          == NONEDATA)s_modelIdx = RNLib::Model().Load("data\\MODEL\\GoalGate.x");
	if(s_TexIdx[0]         == NONEDATA)s_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");
	if(s_TexIdx[1]         == NONEDATA)s_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\effect000.jpg");
	if(s_nEscapeGuideTexID == NONEDATA)s_nEscapeGuideTexID = RNLib::Texture().Load("data\\TEXTURE\\PressBotton01.png");
}

//========================================
// デストラクタ
//========================================
CGoalGate::~CGoalGate(void) {
	s_num--;
}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Init(void) {

	if ((s_numEntry % 2) != 0)
	{
		s_numEntry = 0;
	}

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
	EscapeGuide();

	float fCountRateX, fCountRateY, fCountRateZ;
	CountRate(&fCountRateX, &fCountRateY,&fCountRateZ);
	
	Color setCol = m_col;
	setCol *= 2;
	if (setCol.r > 255)setCol.r = 255;
	if (setCol.g > 255)setCol.g = 255;
	if (setCol.b > 255)setCol.b = 255;
	if (setCol.a > 255)setCol.a = 255;

	//モデル配置
	RNLib::Model().Put(PRIORITY_OBJECT, s_modelIdx, m_pos, m_rot, Scale3D(m_scale.x * fCountRateX, m_scale.y * fCountRateY, m_scale.z * fCountRateZ), false)
		->SetCol(setCol);

	if (!CPlayer::GetSwapAnim()) {
		if (!m_bCloseGate) {
			const int planet = Manager::StgEd()->GetPlanetIdx();
			if (planet == 0) {
				if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
					if (CPlayer::GetSwapEnd()) {
						Pos3D putPos = m_pos;
						putPos.y += (m_pos.y / fabsf(m_pos.y)) * 20.0f;

						RNLib::Text3D().Put(PRIORITY_UI, "GOAL", _RNC_Text::ALIGNMENT::CENTER, 0, RNLib::Matrix().ConvPosToMtx(putPos))
							->SetSize(Size2D(8.0f, 8.0f))
							->SetZTest(false)
							->SetBillboard(true)
							->SetCol(setCol);
					}
				}
			}
		}
	}
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

	if (s_num == s_numEntry || m_bCloseGate == true)
	{
		m_state = STATE::MAX;

		float CountRateRot = MAX_ROT_SPEED * RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		float CntEffRate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCnt, MAX_COUNT);

		if (m_pos.y > 0)
			m_rot.z += 0.6f - CountRateRot;

		else if(m_pos.y < 0)
			m_rot.z -= 0.6f - CountRateRot;

		if (m_nCnt > 0)
		{
			if (!m_bStartGate)
			{
				Manager::EffectMgr()->ParticleCreate(s_TexIdx[1], m_pos, INIT_EFFECT_SCALE * CntEffRate, m_RainbowCol[rand() % 6], CParticle::TYPE::TYPE_SPIN, 30, m_rot);
				Manager::EffectMgr()->ParticleCreate(s_TexIdx[1], m_pos, INIT_EFFECT_SCALE * CntEffRate, m_RainbowCol[rand() % 6], CParticle::TYPE::TYPE_SPIN, 30, D3DXVECTOR3(m_rot.x, m_rot.y, m_rot.z + D3DX_PI));
			}

			m_nCnt--;
		}
		else
		{
			m_state = STATE::NONE;

			if (m_bStartGate == false)
			{
				s_numEntry--;
			}

			for (int ParCnt = 0; ParCnt < 16; ParCnt++)
			{
				Manager::EffectMgr()->ParticleCreate(s_TexIdx[0], m_pos, INIT_EFFECT_SCALE, m_RainbowCol[rand() % 6]);
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
			{
				CPlayer::Pop();
				m_bCloseGate = true;
			}
				
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
		*CountRateX = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCntEtrX, ETR_CNT);
		*CountRateY = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCntEtrY, ETR_CNT);

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
		*CountRateX = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCnt, MAX_COUNT);
		*CountRateY = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCnt, MAX_COUNT);
	}

	*CountRateZ = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_nCnt, MAX_COUNT);
}

//========================================
// 脱出ガイド表示処理
// Author:HIRASAWA SHION
//========================================
void CGoalGate::EscapeGuide(void)
{
	//一人だけ入っている
	if (0 <= m_nEntryNo && m_nEntryNo < s_numEntry && s_numEntry != CPlayer::NUM_PLAYER)
	{
		//表示時間になった
		if (++m_nEntryCounter >= ESCAPE_GUIDE_POPUP_TIME)
		{
			//表示時間×２の時間になったら、表示時間に直す
			if (m_nEntryCounter >= ESCAPE_GUIDE_POPUP_TIME * 2)
				m_nEntryCounter = ESCAPE_GUIDE_POPUP_TIME;

			//α値加算　超えたら修正
			m_GuideAlpha += 10;
			if (m_GuideAlpha > 255) m_GuideAlpha = 255;

			//テクスチャ切り替え判定（int型
			//この判定を使ってテクスチャアニメーションを行う
			CInt IsChange = m_nEntryCounter / (ESCAPE_GUIDE_POPUP_TIME * 1.5f);
			CFloat LeftTexU = 0.5f * IsChange;
			CFloat size = 32.0f;

			//ポリゴン配置
			RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITPOS3D)
				->SetSize(size, size)
				->SetBillboard(true)
				->SetZTest(false)
				->SetCol(Color{ 255,255,255,(int)m_GuideAlpha })
				->SetTex(s_nEscapeGuideTexID,
						   Pos2D(LeftTexU, 0.0f), Pos2D(LeftTexU + 0.5f, 0.0f),
						   Pos2D(LeftTexU, 1.0f), Pos2D(LeftTexU + 0.5f, 1.0f));
		}
	}
	else
	{//誰も入っていない
		m_nEntryCounter = 0;   // カウンタークリア
		m_nEntryNo = NONEDATA; // エントリーNoクリア
		m_GuideAlpha = 0;      // α値クリア
	}
}

//========================================
// エントリー設定処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::SetEntry(bool bEntry)
{
	m_bEntry = bEntry;

	if (bEntry) {
		m_nEntryNo = s_numEntry++;
		m_nCntEtrX = ETR_CNT;
		m_nCntEtrY = ETR_CNT * 0.5;

		if (s_num == s_numEntry)
		{
			RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::GATE_CLOSE], _RNC_Sound::CATEGORY::SE, 1.0f, false);
		}

		for (int ParCnt = 0; ParCnt < 16; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(s_TexIdx[0], m_pos, INIT_EFFECT_SCALE, COLOR_WHITE);
		}
	}
}