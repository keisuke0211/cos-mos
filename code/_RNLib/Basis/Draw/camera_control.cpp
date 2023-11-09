//========================================
// 
// �J�����̏�ԏ���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �J�����N���X�̃����o�֐�
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
		static const D3DXVECTOR3 SPIN_RATE = D3DXVECTOR3(-0.0004f, 0.0004f, 0.0f);

		switch (process) {
			// [[[ �������� ]]]
		case PROCESS::INIT: {
			CMemory::Alloc<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {
			CMemory::Release<GrabAirMouseInfo>((GrabAirMouseInfo**)&m_stateInfo);
		}break;
			// [[[ �X�V���� ]]]
		case PROCESS::UPDATE: {
			GrabAirMouseInfo* info = (GrabAirMouseInfo*)m_stateInfo;

			// [[[ �J�[�\���̈ړ��ʂɉ����ĉ�]������ ]]]
			if (RNLib::Input().GetMousePress(CInput::MOUSEBUTTON::RIGHT)) {
				D3DXVECTOR2 cursorMove = RNLib::Input().GetCursorMove();
				m_spin.x += cursorMove.y * SPIN_RATE.x * info->spinForce;
				m_spin.y += cursorMove.x * SPIN_RATE.y * info->spinForce;
			}

			{
				// ����X�̊���
				float rotXRate = m_rot.x / D3DX_PI_HALF;

				// [[[ �J�[�\���̈ړ��ʂɉ����Ĉړ������� ]]]
				if (RNLib::Input().GetMousePress(CInput::MOUSEBUTTON::LEFT)) {
					const Vector2D cursorMove = RNLib::Input().GetCursorMove();
					const float    moveAngleX = m_rot.y - D3DX_PI_HALF;
					Vector3D       move       = INITVECTOR3D;

					move.x = sinf(moveAngleX) * cursorMove.x;
					move.z = cosf(moveAngleX) * cursorMove.x;
					move.y = cursorMove.y * (1.0f - fabsf(rotXRate));

					// �c�������XZ��Y�̈ړ��ʉ��Z
					move.x += sinf(m_rot.y) * cursorMove.y * -rotXRate;
					move.z += cosf(m_rot.y) * cursorMove.y * -rotXRate;

					// �����_�Ɉړ��ʂ𔽉f
					m_posR += move * info->moveForce;
				}

				// [[[ �z�C�[���̉�]�ɉ����ăY�[���C��/�A�E�g ]]]
				if (RNLib::Input().GetWheelSpin() == CInput::WHEELSPIN::FRONT) {
					const float XZPlaneRate = 1.0f - fabsf(rotXRate);
					
					m_posR.x += sinf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.z += cosf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.y += info->zoomForce * rotXRate;
				}
				else if (RNLib::Input().GetWheelSpin() == CInput::WHEELSPIN::BACK) {
					const float XZPlaneRate = 1.0f - fabsf(rotXRate);

					m_posR.x -= sinf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.z -= cosf(m_rot.y) * info->zoomForce * XZPlaneRate;
					m_posR.y -= info->zoomForce * rotXRate;
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
			CMemory::Alloc<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
		}break;
			// [[[ �I������ ]]]
		case PROCESS::UNINIT: {
			CMemory::Release<ReciprocateSpinInfo>((ReciprocateSpinInfo**)&m_stateInfo);
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
			float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, count, info->time);

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