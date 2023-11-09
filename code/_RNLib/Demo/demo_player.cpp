//========================================
// 
// デモプレイヤーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "demo.h"
#include "demo_player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| デモプレイヤークラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
CDemoPlayer* CDemoPlayer::ms_control = NULL;
bool CDemoPlayer::ms_isCameraClipping = false;

//========================================
// コンストラクタ
//========================================
CDemoPlayer::CDemoPlayer(const Pos3D& pos, const Rot3D& rot, const Color& col, const Color& cameraCol) {

	// リストに追加
	RNDemo::Get().GetDemoObjMgr().AddList(this);

	m_pos    = pos;
	m_rot    = rot;
	m_col    = col;
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight() * 0.5f));
	m_camera->SetBGCol(cameraCol);
	m_clippingCameraID = NONEDATA;
}

//========================================
// デストラクタ
//========================================
CDemoPlayer::~CDemoPlayer() {

	// リストから削除
	RNDemo::Get().GetDemoObjMgr().SubList(this);

	// カメラを破棄
	CMemory::Release(&m_camera);
}

//========================================
// 更新処理
//========================================
void CDemoPlayer::Update(void) {

	// 操作
	if (ms_control == this) {

		float moveForce = 0.0f;

		// 前進
		if (RNLib::Input().GetKeyPress(DIK_UP))
			moveForce = 0.5f;

		// 後進
		if (RNLib::Input().GetKeyPress(DIK_DOWN))
			moveForce = -0.5f;
		
		// 左回転
		if (RNLib::Input().GetKeyPress(DIK_LEFT))
			m_rot.y -= 0.025f;

		// 右回転
		if (RNLib::Input().GetKeyPress(DIK_RIGHT))
			m_rot.y += 0.025f;

		// 移動
		m_pos.x += sinf(m_rot.y) * moveForce;
		m_pos.z += cosf(m_rot.y) * moveForce;
	}

	{// カメラの設定
		Pos3D posV = m_pos + Pos3D(0.0f, 10.0f, 0.0f);
		Pos3D posR = posV;
		posR.x += sinf(m_rot.y);
		posR.z += cosf(m_rot.y);
		m_camera->SetPosVAndPosR(posV, posR);
		m_camera->SetClipping(ms_isCameraClipping);
	}

	// モデル配置
	RNLib::Model().Put(m_pos, m_rot, RNLib::DefaultData().GetModelIdx(CDefaultData::MODEL::OBSERVER))
		->SetCol(m_col)
		->SetOutLine(true)
		->SetClippingCamera(m_clippingCameraID);
}