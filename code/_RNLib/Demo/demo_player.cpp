//========================================
// 
// デモプレイヤーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "demo.h"
#include "demo_player.h"
#include "demo_zone.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]デモプレイヤークラス
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
CDemoPlayer::CDemoPlayer(const Pos3D& pos, const Rot3D& rot, const Color& col, const Color& cameraCol) : CDemoObject(TYPE::PLAYER) {

	m_pos    = pos;
	m_move   = INITVECTOR3D;
	m_rot    = rot;
	m_spin   = INITVECTOR3D;
	m_col    = col;
	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetOverwriteBGCol(cameraCol);
	m_clippingCameraID = NONEDATA;
}

//========================================
// デストラクタ
//========================================
CDemoPlayer::~CDemoPlayer() {

	// カメラを破棄
	RNLib::Memory().Release(&m_camera);
}

//========================================
// 更新処理
//========================================
void CDemoPlayer::Update(void) {

	// 操作
	if (ms_control == this) {

		float moveForce = 0.2f;
		float moveangle = 0.0f;

		// 移動
		if (RNLib::Input().GetKeyPress(DIK_W)) {
			if      (RNLib::Input().GetKeyPress(DIK_A)) { moveangle = D3DX_PI * -0.25f; }
			else if (RNLib::Input().GetKeyPress(DIK_D)) { moveangle = D3DX_PI * 0.25f;  }
			else                                        { moveangle = 0.0f;             }
		}
		else if (RNLib::Input().GetKeyPress(DIK_S)) {
			if      (RNLib::Input().GetKeyPress(DIK_A)) { moveangle = D3DX_PI * -0.75f; }
			else if (RNLib::Input().GetKeyPress(DIK_D)) { moveangle = D3DX_PI * 0.75f;  }
			else                                        { moveangle = D3DX_PI;          }
		}
		else if (RNLib::Input().GetKeyPress(DIK_A)) { moveangle = D3DX_PI * -0.5f; }
		else if (RNLib::Input().GetKeyPress(DIK_D)) { moveangle = D3DX_PI * 0.5f;  }
		else { moveForce = 0.0f; }

		// 回転量
		if (RNLib::Input().GetKeyPress(DIK_UP   ))m_spin.x -= 0.02f;
		if (RNLib::Input().GetKeyPress(DIK_DOWN ))m_spin.x += 0.02f;
		if (RNLib::Input().GetKeyPress(DIK_LEFT ))m_spin.y -= 0.02f;
		if (RNLib::Input().GetKeyPress(DIK_RIGHT))m_spin.y += 0.02f;

		// 移動量
		m_move.x += (sinf(m_rot.y + moveangle) * moveForce);
		m_move.z += (cosf(m_rot.y + moveangle) * moveForce);
	}

	// 移動量適用
	m_pos += m_move;
	m_move *= 0.75f;

	// 回転量適用
	m_rot += m_spin;
	m_spin *= 0.75f;
	if (m_rot.x < -D3DX_PI_HALF * 0.9f)
		m_rot.x = -D3DX_PI_HALF * 0.9f;
	else if (m_rot.x > D3DX_PI_HALF * 0.9f)
		m_rot.x = D3DX_PI_HALF * 0.9f;

	// 当たり判定処理
	HitTest();

	if (m_camera != NULL)
	{// カメラの設定
		Pos3D posV = m_pos + Pos3D(0.0f, 10.0f, 0.0f);
		Pos3D posR = posV + RNLib::Geometry().FindRotVec(m_rot);
		m_camera->SetPosVAndPosR(posV, posR);
		m_camera->SetIsClipping(ms_isCameraClipping);
	}

	// モデル配置
	RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::OBSERVER), m_pos, m_rot)
		->SetCol(m_col)
		->SetOutLineIdx(true)
		->SetClippingCamera(m_clippingCameraID);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]デモプレイヤークラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 当たり判定処理
//========================================
void CDemoPlayer::HitTest(void) {

	CDemoObject* object = NULL;
	while (RNDemo::Get().GetDemoObjMgr().ListLoop((CObject**)&object)) {

		switch (object->GetType()) {
		case TYPE::ZONE: {
			
			// 自身が操作対象でなければ抜ける
			if (ms_control != this)
				break;

			CDemoZone* demoZone = (CDemoZone*)object;

			if (RNLib::HitTest().XZ_OverlapPointToSquare(m_pos, demoZone->GetPos(), demoZone->GetScale())) {
				CDemoZone::SetActive(demoZone);
			}

		}break;
		}
	}
}