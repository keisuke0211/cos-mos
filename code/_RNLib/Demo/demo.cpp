//========================================
// 
// デモの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| デモクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDemo::CDemo() {

	for (int cntPlayer = 0; cntPlayer < (int)PLAYER::MAX; m_players[cntPlayer] = NULL, cntPlayer++);
	m_controlPlayerCount = 0;
}

//========================================
// デストラクタ
//========================================
CDemo::~CDemo() {

}

//========================================
// 初期化処理
//========================================
void CDemo::Init(void) {

	// プレイヤーを生成
	m_players[(int)PLAYER::RED] = new CDemoPlayer(Pos3D(0.0f, 0.0f, -40.0f), Rot3D(0.0f, 0.0f, 0.0f), Color{ 255,0,0,255 }, Color{ 255,128,128,255 });
	m_players[(int)PLAYER::BLUE] = new CDemoPlayer(Pos3D(0.0f, 0.0f, 40.0f), Rot3D(0.0f, D3DX_PI, 0.0f), Color{ 0,0,255,255 }, Color{ 128,128,255,255 });

	// プレイヤーのクリッピングカメラIDを設定
	m_players[(int)PLAYER::RED]->SetClippingCameraID(m_players[(int)PLAYER::BLUE]->GetCamera().GetID());
	m_players[(int)PLAYER::BLUE]->SetClippingCameraID(m_players[(int)PLAYER::RED]->GetCamera().GetID());

	// 操作設定
	CDemoPlayer::SetControl(m_players[m_controlPlayerCount]);
}

//========================================
// 終了処理
//========================================
void CDemo::Uninit(void) {

	for (int cntPlayer = 0; cntPlayer < (int)PLAYER::MAX; cntPlayer++)
		CMemory::Release(&m_players[cntPlayer]);
}

//========================================
// 更新処理
//========================================
void CDemo::Update(void) {

	//----------------------------------------
	// 操作
	//----------------------------------------
	// 操作プレイヤー切り替え
	if (RNLib::Input().GetKeyTrigger(DIK_1)) {
		m_controlPlayerCount = (m_controlPlayerCount + 1) % (int)PLAYER::MAX;
		CDemoPlayer::SetControl(m_players[m_controlPlayerCount]);
	}

	// カメラのクリッピングオンオフ切り替え
	if (RNLib::Input().GetKeyTrigger(DIK_2)) {
		CDemoPlayer::SetCameraClipping(!CDemoPlayer::GetCameraClipping());
	}

	//----------------------------------------
	// 描画
	//----------------------------------------
	{// [[[ プレイヤーの視点描画 ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();
		const float windowHeightHalf  = windowHeight * 0.5f;
		const float windowHeightHalf2 = windowHeightHalf * 0.5f;

		// 赤(上)
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y - windowHeightHalf2, 0.0f), 0.0f, true)
			->SetTex_Camera(&m_players[(int)PLAYER::RED]->GetCamera())
			->SetSize(windowWidth, windowHeightHalf);

		// 青(下)
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y + windowHeightHalf2, 0.0f), 0.0f, true)
			->SetTex_Camera(&m_players[(int)PLAYER::BLUE]->GetCamera())
			->SetSize(windowWidth, windowHeightHalf);
	}

	//----------------------------------------
	// 描画オブジェクト
	// ◆ (オブジェクト名)
	//----------------------------------------
	{// ◆ クリッピングスフィア
		Pos3D basePos = INITPOS3D;

		// 赤
		RNLib::Model().Put(basePos + Pos3D(10.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 255, 0, 0, 255 })
			->SetOutLine(true)
			->SetClippingCamera(m_players[(int)PLAYER::BLUE]->GetCamera());	// ※青プレイヤーのカメラにのみ描画

		// 白
		RNLib::Model().Put(basePos + Pos3D(0.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 255, 255, 255, 255 })
			->SetOutLine(true);

		// 青
		RNLib::Model().Put(basePos + Pos3D(-10.0f, 5.0f, 0.0f), INITROT3D, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::SPHERE))
			->SetCol(Color{ 0, 0, 255, 255 })
			->SetOutLine(true)
			->SetClippingCamera(m_players[(int)PLAYER::RED]->GetCamera());	// ※赤プレイヤーのカメラにのみ描画

		// 床
		RNLib::Polygon3D().Put(basePos + Pos3D(0.0f, -5.0f, 0.0f), Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(100.0f, 100.0f)
			->SetCol(Color{ 128, 128, 128, 255 });
	}
}