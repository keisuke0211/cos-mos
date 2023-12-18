//========================================
// 
// �J�����̏�ԏ���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �J�����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// ��ԏ���
//========================================
void CCamera::ProcessState(const PROCESS process) {

	switch (m_state) {
		//----------------------------------------
		// �}�E�X�Œ���͂�
		//----------------------------------------
	case STATE::GRAB_AIR_MOUSE: {
		switch (process) {
			// [[[ �������� ]]]
		case PROCESS::INIT: {
			RNLib::Memory().Alloc<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
			m_isPivotToPosV = true;
		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {
			RNLib::Memory().Release<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ �X�V���� ]]]
		case PROCESS::UPDATE: {
			GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;

			// [[[ �J�[�\���̈ړ��ʂɉ����ĉ�]������ ]]]
			if (RNLib::Input().GetMousePress(_RNC_Input::MOUSEBUTTON::RIGHT)) {
				D3DXVECTOR2 cursorMove = RNLib::Input().GetCursorMove();
				m_spin.x += cursorMove.y * 0.0002f * info->spinForce;
				m_spin.y += cursorMove.x * 0.0002f * info->spinForce;
			}

			{
				// ����X�̊���
				float rotXRate = m_rot.x / D3DX_PI_HALF;

				// [[[ �J�[�\���̈ړ��ʂɉ����Ĉړ������� ]]]
				if (RNLib::Input().GetMousePress(_RNC_Input::MOUSEBUTTON::LEFT)) {
					const Vector2D cursorMove = RNLib::Input().GetCursorMove();
					const float    moveAngleX = m_rot.y - D3DX_PI_HALF;
					Vector3D       move       = INITVECTOR3D;

					move.x = sinf(moveAngleX) * cursorMove.x;
					move.z = cosf(moveAngleX) * cursorMove.x;
					move.y = cursorMove.y * (1.0f - fabsf(rotXRate));

					// �c�������XZ��Y�̈ړ��ʉ��Z
					move.x += sinf(m_rot.y) * cursorMove.y * rotXRate;
					move.z += cosf(m_rot.y) * cursorMove.y * rotXRate;

					// �����_�Ɉړ��ʂ𔽉f
					m_posV += move * info->moveForce * 0.1f;
				}

				// [[[ �z�C�[���̉�]�ɉ����ăY�[���C��/�A�E�g ]]]
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
		// ������]
		//----------------------------------------
	case STATE::RECIPROCATE_SPIN: {
		switch (process) {
			// [[[ �������� ]]]
		case PROCESS::INIT: {
			RNLib::Memory().Alloc<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {
			RNLib::Memory().Release<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
		}break;
			// [[[ �X�V���� ]]]
		case PROCESS::UPDATE: {
			ReciprocateSpinInfo* info = (ReciprocateSpinInfo*)m_stateInfo;

			// �J�E���g
			info->count = (info->count + 1) % (info->time * 2);
			int count = info->count;
			if (count > info->time)
				count = info->time - (count - info->time);

			// ����
			float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, count, info->time);

			// ���ۂ̌������Z�o
			m_rot = (info->rotA * (1.0f - rate)) + (info->rotB * rate);
		}break;
		}
	}break;
	}
}

//========================================
// ��Ԑݒ�
//========================================
void CCamera::SetState(const STATE state) {

	// ���̏�Ԃ̏I������
	ProcessState(PROCESS::UNINIT);

	// ��Ԃ���
	m_state = state;

	// �V������Ԃ̏�������
	ProcessState(PROCESS::INIT);
}

//========================================
// �}�E�X�Œ���͂�
//========================================
void CCamera::SetGrabAirMouse(const float moveForce, const float spinForce, const float zoomForce) {
	
	SetState(STATE::GRAB_AIR_MOUSE);
	GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;
	info->moveForce = moveForce;
	info->spinForce = spinForce;
	info->zoomForce = zoomForce;
}

//========================================
// ������]
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