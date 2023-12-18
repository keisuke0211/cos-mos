//========================================
// 
// モード:リザルト処理
// Author:KEISUKE OTONO
// 
//========================================
#include "../main.h"
#include "mode_result.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Resultクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Result::CMode_Result(void) {

}

//========================================
// デストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Result::~CMode_Result(void) {

}

//========================================
// 初期化処理
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Init(void) {
	CMode::Init();

	// 遷移設定
	Manager::Transition().Open(CTransition::TYPE::NUI, 60);

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);
}

//========================================
// 終了処理
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Uninit(void) {
	CMode::Uninit();

}

//========================================
// 更新処理
// Author:KEISUKE OTONO
//========================================
void CMode_Result::Update(void) {
	CMode::Update();

	if (RNLib::Input().GetKeyTrigger(DIK_SPACE) && Manager::Transition().GetState() == CTransition::STATE::NONE)
		Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::NUI);

}

//========================================
// 更新処理(状態)
// Author:KEISUKE OTONO
//========================================
void CMode_Result::ProcessState(const PROCESS process) {
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