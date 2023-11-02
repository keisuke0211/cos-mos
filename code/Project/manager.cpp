//========================================
// 
// マネージャー処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "System\words\object\font-object.h"
#include "main.h"

//****************************************
// 名前空間
//****************************************
namespace {
	//========== [[[ 変数宣言 ]]]
	CMode*      m_modeObj;
	CMode::TYPE m_reserveModeType;
	CStageObjectMgr   m_blockMgr;
	CEffectMgr   m_effectMgr;
	CStageEditor m_StgEd;
	CFont m_Font;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Managerの関数
//----------|---------------------------------------------------------------------
//================================================================================
CStageObjectMgr* Manager::BlockMgr(void) { return &m_blockMgr; }
CEffectMgr* Manager::EffectMgr(void) { return &m_effectMgr; }
CStageEditor* Manager::StgEd(void) { return &m_StgEd; }
CFont* Manager::Font(void) { return &m_Font; }

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Init(CMode::TYPE mode) {

	// モード設定
	SetMode(mode);

	//使用するコントローラーの数を指定する
	RNLib::Input().SetJoyPadNum(2);
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Uninit(void) {

	// モードオブジェクトの終了処理
	if (m_modeObj != NULL) {
		m_modeObj->Uninit();
		RNLib::Memory().Release(&m_modeObj);
	}

	// フォントオブジェクトの終了処理
	CFontObject::UpdateAll();
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Update(void) {

	m_blockMgr.Update();

	// 予約されている時、遷移がモード設定待ちならモードを設定する
	if (m_reserveModeType != CMode::TYPE::NONE) {
		if (RNLib::Transition().GetState() == CTransition::STATE::WAIT_SET_MODE) {
			SetMode(m_reserveModeType);
		}
	}

	// モードオブジェクトが在る時、更新処理
	if (m_modeObj != NULL) {
		m_modeObj->Update();

		if(m_modeObj != NULL)
		m_modeObj->ProcessState(PROCESS::UPDATE);
	}

	// フォントオブジェクトの更新処理
	CFontObject::UpdateAll();
}

//========================================
// 読み込み処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Load(void) {
	
}

//========================================
// 書き込み処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Save(void) {
	
}

//========================================
// モードの設定処理
// Author:RIKU NISHIMURA
//========================================
void Manager::SetMode(CMode::TYPE newMode) {

	// シーンを終了
	RNLib::UninitScene();

	// モードオブジェクトの終了処理
	if (m_modeObj != NULL) {
		m_modeObj->Uninit();
		RNLib::Memory().Release(&m_modeObj);
	}
	// モードオブジェクトを新たに生成
	m_modeObj = CMode::Create(newMode);

	// 予約モードを無しにする
	m_reserveModeType = CMode::TYPE::NONE;
}

//========================================
// モードの取得処理
// Author:RIKU NISHIMURA
//========================================
CMode::TYPE Manager::GetMode(void) {

	if (m_modeObj != NULL)
		return m_modeObj->GetType();

	return CMode::TYPE::NONE;
}

//========================================
// 遷移処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Transition(CMode::TYPE newMode, CTransition::TYPE transType) {

	// モードを予約する
	m_reserveModeType = newMode;

	// 遷移設定
	RNLib::Transition().Set(CTransition::STATE::CLOSE, transType);
}