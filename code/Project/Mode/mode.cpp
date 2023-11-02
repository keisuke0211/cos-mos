//========================================
// 
// モードの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "mode.h"
#include "mode_logo.h"
#include "mode_title.h"
#include "mode_game.h"
#include "mode_result.h"
#include "../../_RNLib/RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CModeクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CMode::CMode(void) {
	m_type     = TYPE::NONE;
	m_state    = 0;
	m_stateCtr = 0;
}

//========================================
// デストラクタ
//========================================
CMode::~CMode(void) {

}

//========================================
// 初期化処理
//========================================
void CMode::Init(void) {

}

//========================================
// 終了処理
//========================================
void CMode::Uninit(void) {
	
}

//========================================
// 更新処理
//========================================
void CMode::Update(void) {

}

//========================================
// 生成処理
//========================================
CMode* CMode::Create(TYPE type) {

	// 種類に応じてオブジェクト生成
	CMode* pMode = NULL;
	switch (type) {
	case TYPE::LOGO:pMode = new CMode_Logo; break;
	case TYPE::TITLE:pMode = new CMode_Title; break;
	case TYPE::GAME:pMode = new CMode_Game; break;
	case TYPE::RESULT:pMode = new CMode_Result; break;
	default:
		assert(false);
		break;
	}

	// 種類設定
	pMode->SetType(type);
	
	// 初期化処理
	pMode->Init();

	return pMode;
}

//========================================
// 状態設定処理
//========================================
void CMode::SetState(int nState) {

	// 状態終了処理
	ProcessState(PROCESS::UNINIT);

	// 状態を設定し、カウンター初期化
	m_state = nState;
	m_stateCtr = 0;
	
	// 状態初期処理
	ProcessState(PROCESS::INIT);
}