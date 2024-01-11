//========================================
// 
// マネージャー処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "System\words\object\font-object.h"
#include "Sound/stage-sound-player.h"
#include "main.h"
#include "resource.h"
#include "stage.h"

//****************************************
// マクロ定義
//****************************************
#define LIGHT3D_MAX (3)

//****************************************
// 無名空間
//****************************************
namespace {
	//========== [[[ 変数宣言 ]]]
	CMode*          m_modeObj;
	CMode::TYPE     m_reserveModeType;
	CStageObjectMgr m_blockMgr;
	CObjectMgr      m_BGMgr("BGMgr");
	CEffMgr         m_effectMgr;
	CStageEditor    m_StgEd;
	CFont           m_Font;
	CCamera*        m_camera;
	CCamera*        m_subCamera;
	CTransition     m_transition;
	CLight3D*       m_light3D[LIGHT3D_MAX];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Managerの関数
//----------|---------------------------------------------------------------------
//================================================================================
CCamera* Manager::GetMainCamera(void) { return m_camera; }
CCamera* Manager::GetSubCamera(void) { return m_subCamera; }
CStageObjectMgr* Manager::StageObjectMgr(void) { return &m_blockMgr; }
CObjectMgr* Manager::BGMgr(void) { return &m_BGMgr; }
CEffMgr* Manager::EffectMgr(void) { return &m_effectMgr; }
CStageEditor* Manager::StgEd(void) { return &m_StgEd; }
CFont* Manager::Font(void) { return &m_Font; }
CTransition& Manager::Transition(void) { return m_transition; }
short& Manager::GetLightIdx(UShort lightIdx) { return m_light3D[lightIdx]->GetID(); }

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Init(CMode::TYPE mode) {

	// リソースの読み込み
	CResources::Load();

	// ライト3Dの生成
	for (int cnt = 0; cnt < LIGHT3D_MAX; cnt++) {
		switch (cnt) {
		case 0:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\Title.txt"); break;
		case 1:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\World1.txt"); break;
		case 2:m_light3D[cnt] = new CLight3D("data\\LIGHT3D\\World2.txt"); break;
		default:
			m_light3D[cnt] = NULL;
			assert(false);
			break;
		}
	}

	// カメラの生成
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetLightID(m_light3D[0]->GetID());
	m_subCamera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_subCamera->SetLightID(m_light3D[0]->GetID());

	// モード設定
	SetMode(mode);

	// ステージ初期化処理
	Stage::Init();

	// 使用するコントローラーの数を指定する
	RNLib::Input().SetJoyPadNum(2);

	// 標準エフェクトの優先度設定
	RNLib::StandardEffect3D().SetPriority(PRIORITY_EFFECT);

	// 遷移の初期化
	m_transition.Init();
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

	// カメラの破棄
	RNLib::Memory().Release(&m_camera);
	RNLib::Memory().Release(&m_subCamera);

	// ステージ終了処理
	Stage::Uninit();

	// 遷移の終了処理
	m_transition.Uninit();
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Update(void) {

	{// [[[ カメラの描画 ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();

		RNLib::Polygon2D().Put(0, Pos3D(windowCenterPos.x, windowCenterPos.y, 0.0f), 0.0f, true)
			->SetTex(m_camera)
			->SetSize(windowWidth, windowHeight)
			->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
	}

	if (RNSystem::GetMode() == RNSystem::MODE::DEBUG) {
		if (RNLib::Input().GetKeyTrigger(DIK_F9)) {
			RNLib::DrawMgr().SetIsDrawPolygon2D(false, !RNLib::DrawMgr().GetIsDrawPolygon2D(false));
		}
	}

	// 予約されている時、遷移がモード設定待ちならモードを設定する
	if (m_reserveModeType != CMode::TYPE::NONE) {
		if (m_transition.GetState() == CTransition::STATE::OPEN_WAIT) {
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

	// 遷移の更新処理
	m_transition.Update();
}

//========================================
// 描画処理
// Author:RIKU NISHIMURA
//========================================
void Manager::Draw(void) {

	CFontObject::DrawAll();
}

//========================================
// モードの設定処理
// Author:RIKU NISHIMURA
//========================================
void Manager::SetMode(CMode::TYPE newMode) {

	// シーンを終了
	RNSystem::EndScene();

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
	m_transition.Close(transType, COLOR_WHITE, 60);
}