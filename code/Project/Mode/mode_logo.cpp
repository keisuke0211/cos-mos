//========================================
// 
// モード:ロゴ
// Author:KEISUKE OTONO
// 
//========================================
#include "../main.h"
#include "mode_logo.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Logoクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 静的変数
//========================================
int CMode_Logo::m_TexLogo = 0;
int CMode_Logo::m_TexBg = 0;

//========================================
// コンストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Logo::CMode_Logo(void) {
	m_TexLogo = 0;
	m_TexBg = 0;
	m_nStateCtr = TIME;
	m_state = STATE::NONE;
}

//========================================
// デストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Logo::~CMode_Logo(void) {

}

//========================================
// 初期化処理
// Author:KEISUKE OTONO
//========================================
void CMode_Logo::Init(void) {
	CMode::Init();

	m_TexLogo = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\TeamLogo.PNG");

	// 状態設定
	SetState((int)STATE::NONE);
	// 遷移設定
	Manager::Transition().Open(CTransition::TYPE::FADE, 60);

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);

	m_state = STATE::OPEN_WAIT;
}

//========================================
// 終了処理
// Author:KEISUKE OTONO
//========================================
void CMode_Logo::Uninit(void) {
	CMode::Uninit();

}

//========================================
// 更新処理
// Author:KEISUKE OTONO
//========================================
void CMode_Logo::Update(void) {
	CMode::Update();

	//===== [[[ ローカル関数宣言 ]]]
	struct LocalFunc {
		static void FillScreen(const float& fRate) {
			RNLib::Polygon2D().Put(PRIORITY_UI, RNLib::Window().GetCenterPos(), 0.0f)
				->SetCol(Color{ 255,255,255,(int)(255 * fRate) })
				->SetSize(960, 240)
				->SetTex(m_TexLogo);
		}
	};

	if (m_state == STATE::NONE)
	{
		return;
	}
	else if (m_state == STATE::OPEN_WAIT) {
		if (++m_nStateCtr >= WAIT_TIME) {
			m_nStateCtr = TIME;
			m_state = STATE::OPEN;
		}
		LocalFunc::FillScreen(0.0f);
		return;
	}
	else if (m_state == STATE::CLOSE_WAIT)
	{
		if (++m_nStateCtr >= WAIT_TIME) {
			m_nStateCtr = 0;

			m_state = STATE::CLOSE;
		}
		LocalFunc::FillScreen(1.0f);
		return;
	}
	else if (m_state == STATE::OPEN)
	{
		if (--m_nStateCtr <= 0)
		{
			m_state = STATE::CLOSE_WAIT;
			m_stateCtr = WAIT_TIME;
		}
	}
	else if (m_state == STATE::CLOSE)
	{
		if (++m_nStateCtr >= TIME)
		{
			if (Manager::Transition().GetState() == CTransition::STATE::NONE)
			{
				Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
				m_state = STATE::NONE;
			}
		}
	}

	float fRate = (float)m_nStateCtr / TIME;
	float fRateOpp = 1.0 - fRate;

	LocalFunc::FillScreen(fRateOpp);

}

//========================================
// 更新処理(状態)
// Author:KEISUKE OTONO
//========================================
void CMode_Logo::ProcessState(const PROCESS process) {
	switch ((STATE)m_state) {
		//----------------------------------------
		// 無し
		//----------------------------------------
	case STATE::NONE: {
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {

		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {

		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {

		}break;
		}
	}break;
	}
}