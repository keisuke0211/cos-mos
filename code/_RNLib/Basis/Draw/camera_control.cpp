//========================================
// 
// カメラの状態処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| カメラクラス
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
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {
			RNLib::Memory().Alloc<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
			m_isPivotToPosV = true;
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {
			RNLib::Memory().Release<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {
			GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;

			// [[[ カーソルの移動量に応じて回転させる ]]]
			if (RNLib::Input().GetMousePress(_RNC_Input::MOUSEBUTTON::RIGHT)) {
				D3DXVECTOR2 cursorMove = RNLib::Input().GetCursorMove();
				m_spin.x += cursorMove.y * 0.0002f * info->spinForce;
				m_spin.y += cursorMove.x * 0.0002f * info->spinForce;
			}

			{
				// 向きXの割合
				float rotXRate = m_rot.x / D3DX_PI_HALF;

				// [[[ カーソルの移動量に応じて移動させる ]]]
				if (RNLib::Input().GetMousePress(_RNC_Input::MOUSEBUTTON::LEFT)) {
					const Vector2D cursorMove = RNLib::Input().GetCursorMove();
					const float    moveAngleX = m_rot.y - D3DX_PI_HALF;
					Vector3D       move       = INITVECTOR3D;

					move.x = sinf(moveAngleX) * cursorMove.x;
					move.z = cosf(moveAngleX) * cursorMove.x;
					move.y = cursorMove.y * (1.0f - fabsf(rotXRate));

					// 縦方向基準でXZにYの移動量加算
					move.x += sinf(m_rot.y) * cursorMove.y * rotXRate;
					move.z += cosf(m_rot.y) * cursorMove.y * rotXRate;

					// 注視点に移動量を反映
					m_posV += move * info->moveForce * 0.1f;
				}

				// [[[ ホイールの回転に応じてズームイン/アウト ]]]
				if (RNLib::Input().GetWheelSpin() == _RNC_Input::WHEELSPIN::FRONT) {
					m_posV += RNLib::Geometry().FindRotVec(m_rot) * info->zoomForce * 4.0f;
				}
				else if (RNLib::Input().GetWheelSpin() == _RNC_Input::WHEELSPIN::BACK) {
					m_posV -= RNLib::Geometry().FindRotVec(m_rot) * info->zoomForce * 4.0f;
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
			float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, count, info->time);

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
void CCamera::SetGrabAirMouse(const float moveForce, const float spinForce, const float zoomForce) {
	
	SetState(STATE::GRAB_AIR_MOUSE);
	GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;
	info->moveForce = moveForce;
	info->spinForce = spinForce;
	info->zoomForce = zoomForce;
}

//========================================
// 往復回転
//========================================
void CCamera::SetReciprocateSpin(const Pos3D posR, const float dist, const Rot3D rotA, const Rot3D rotB, const int time) {

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