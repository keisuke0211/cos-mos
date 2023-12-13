//========================================
// 
// モード:ゲームの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "../main.h"
#include "../Character/player.h"
#include "../System/StageEditor.h"
#include "../System/words/font-text.h"
#include "../Mode/mode_title.h"
#include "../UI/partsUI.h"
#include "../UI/coinUI.h"
#include "../System/BG-Editor.h"
#include "../Sound/stage-sound-player.h"
#include "../resource.h"
#include "../stage.h"
#include "../UI/MenuUi.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Gameクラス
//----------|---------------------------------------------------------------------
//================================================================================
CMenuUI *CMode_Game::m_MenuUI = NULL;
static const int s_PlanetMaxSummon = 8;		// 出現する位置の最大数
static const int s_StarMaxSummon = 10;		// 出現する位置の最大数

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::CMode_Game(void) {

}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::~CMode_Game(void) {
	if (m_MenuUI != NULL){
		delete m_MenuUI;
		m_MenuUI = NULL;
	}
}

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Init(void) {
	CMode::Init();

	// ステージ開始処理
	Stage::StartStage();

	// 遷移設定
	RNLib::Transition().Open(CTransition::TYPE::FADE, 30);

	// 状態設定
	SetState((int)STATE::NONE);

	// ステージオブジェクトの読み込み
	Manager::StageObjectMgr()->Load();

	// メニュー生成
	m_MenuUI = CMenuUI::Create(CMode::TYPE::GAME);
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Uninit(void) {
	CMode::Uninit();

	// ステージ終了処理
	Stage::EndStage();
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Update(void) {
	CMode::Update();

	// ステージ更新処理
	Stage::UpdateStage();

	// [[[ 非ポーズ時の処理 ]]]
	if (m_state != (int)STATE::PAUSE) {

		// ポーズ
		if (!CPlayer::GetSwapAnim() && !CPlayer::GetDeath() && CPlayer::GetZoomUpCounter() == 0) {
			if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START)) {
				SetState((int)STATE::PAUSE);
			}
		}
	}
}

//========================================
// 更新処理(状態)
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::ProcessState(const PROCESS process) {
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
	case STATE::PAUSE: {
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {

			m_MenuUI->MenuCreate();

			// ステージをポーズ状態に
			Stage::SetPause(true);
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {

			// ステージを非ポーズ状態に
			Stage::SetPause(false);
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, 0.0f), 0.0f, false)
				->SetSize(RNLib::Window().GetCenterX() * 2, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 0,0,0,120 });

			m_MenuUI->Update();

			bool MenuEnd = m_MenuUI->m_MenuEnd;

			if (MenuEnd) {
				m_MenuUI->TextRelease(CMenuUI::TEXT_ALL);
				ProcessState(PROCESS::UNINIT);

				if (RNLib::Transition().GetState() == CTransition::STATE::NONE) {
					SetState((int)STATE::NONE);
				}
			}
		}break;
		}
	}break;
	}
}