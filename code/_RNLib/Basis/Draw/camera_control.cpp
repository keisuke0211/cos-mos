//========================================
// 
// カメラの状態処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| カメラクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 状態処理
//========================================
void CCamera::ProcessState(const PROCESS process) {

	switch (m_state) {
		//----------------------------------------
		// マウスで宙を掴む
		//----------------------------------------
	case STATE::GRAB_AIR_MOUSE: {
		static const D3DXVECTOR3 SPIN_RATE = D3DXVECTOR3(-0.0004f, 0.0004f, 0.0f);

		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {
			RNLib::Memory().Alloc<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {
			RNLib::Memory().Release<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {
			GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;

			// [[[ カーソルの移動量に応じて回転させる ]]]
			if (RNLib::Input().GetMousePress(CInput::MOUSEBUTTON::RIGHT)) {
				D3DXVECTOR2 cursorMove = RNLib::Input().GetCursorMove();
				m_spin.x += cursorMove.y * SPIN_RATE.x * info->spinForce;
				m_spin.y += cursorMove.x * SPIN_RATE.y * info->spinForce;
			}

			{
				// 向きXの割合
				float rotXRate = m_rot.x / D3DX_PI_HALF;

				// [[[ カーソルの移動量に応じて移動させる ]]]
				if (RNLib::Input().GetMousePress(CInput::MOUSEBUTTON::LEFT)) {
					D3DXVECTOR3 move = INITD3DXVECTOR3;					// 移動量
					D3DXVECTOR2 cursorMove = RNLib::Input().GetCursorMove();	// カーソルの移動量
					float       moveAngleX = m_rot.y - D3DX_PI_HALF;			// 移動向きX

					move.x = sinf(moveAngleX) * cursorMove.x;
					move.z = cosf(moveAngleX) * cursorMove.x;
					move.y = cursorMove.y * (1.0f - fabsf(rotXRate));

					// 縦方向基準でXZにYの移動量加算
					move.x += sinf(m_rot.y) * cursorMove.y * -rotXRate;
					move.z += cosf(m_rot.y) * cursorMove.y * -rotXRate;

					// 注視点に移動量を反映
					m_posR += move * info->moveForce;
				}

				// [[[ ホイールの回転に応じてズームイン/アウト ]]]
				if (RNLib::Input().GetWheelSpin() == CInput::WHEELSPIN::FRONT) {
					float XZPlaneRate = 1.0f - fabsf(rotXRate);
					
					m_posR.x += sinf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.z += cosf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.y += info->zoomForce * rotXRate;
				}
				else if (RNLib::Input().GetWheelSpin() == CInput::WHEELSPIN::BACK) {
					float XZPlaneRate = 1.0f - fabsf(rotXRate);

					m_posR.x -= sinf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.z -= cosf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.y -= info->zoomForce * rotXRate;
				}
			}
		}break;
		}
	}break;
		//----------------------------------------
		// 往復回転
		//----------------------------------------
	case STATE::RECIPROCATE_SPIN: {
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {
			RNLib::Memory().Alloc<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {
			RNLib::Memory().Release<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {
			ReciprocateSpinInfo* info = (ReciprocateSpinInfo*)m_stateInfo;

			// カウント
			info->count = (info->count + 1) % (info->time * 2);
			int count = info->count;
			if (count > info->time)
				count = info->time - (count - info->time);

			// 割合
			float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, count, info->time);

			// 実際の向きを算出
			m_rot = (info->rotA * (1.0f - rate)) + (info->rotB * rate);
		}break;
		}
	}break;
	}
}

//========================================
// 状態設定
//========================================
void CCamera::SetState(const STATE state) {

	// 今の状態の終了処理
	ProcessState(PROCESS::UNINIT);

	// 状態を代入
	m_state = state;

	// 新しい状態の初期処理
	ProcessState(PROCESS::INIT);
}

//========================================
// マウスで宙を掴む
//========================================
void CCamera::GrabAirMouse(const float moveForce, const float spinForce, const float zoomForce) {
	
	SetState(STATE::GRAB_AIR_MOUSE);
	GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;
	info->moveForce = moveForce;
	info->spinForce = spinForce;
	info->zoomForce = zoomForce;
}

//========================================
// 往復回転
//========================================
void CCamera::ReciprocateSpin(const D3DXVECTOR3 posR, const float dist, const D3DXVECTOR3 rotA, const D3DXVECTOR3 rotB, const int time) {

	SetState(STATE::RECIPROCATE_SPIN);
	m_posR = posR;
	m_dist = dist;
	m_rot  = rotA;
	ReciprocateSpinInfo* info = (ReciprocateSpinInfo*)m_stateInfo;
	info->rotA   = rotA;
	info->rotB   = rotB;
	info->time  = time;
	info->count = 0;
}