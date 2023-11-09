//========================================
// 
// �J�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object.h"
#include "../Mechanical/memory.h"

//****************************************
// �N���X��`
//****************************************
// �J�����N���X
class CCamera : public CObject {
public:
	//========== [[[ �񋓌^��` ]]]
	// ���
	enum class STATE {
		NONE,
		GRAB_AIR_MOUSE,		// �}�E�X�Œ���͂�
		RECIPROCATE_SPIN,	// ������]
		MAX,
	};

	// �U���̋���
	enum class VIB_POWER { LOW, MID, HIGH, MAX, };

	//========== [[[ �֐��錾 ]]]
	            CCamera             (const Scale2D& scale2D);
	            ~CCamera            ();
	void        Update              (void);
	static void StartRenderingScreen(Device& device);
	void        StartRendering      (Device& device);
	void        EndRendering        (Device& device);
	void        SetTexture          (Device& device) { device->SetTexture(0, m_MTInfo.textures[0]); }
	void        SetPosVAndPosR      (const Pos3D& posV, const Pos3D& posR);
	void        SetVib              (const float& vibPower);
	void        SetRadianGoal       (const float& goal) { m_radianGoal = goal; }
	void        SetFixed            (const bool& isFixid) { m_isFixed = isFixid; }
	void        SetClipping         (const bool& isClipping) { m_isClipping = isClipping; }
	bool&       GetClipping         (void) { return m_isClipping; }
	void        PivotToPosV         (void) { if (m_state == STATE::NONE) m_isPivotToPosV = true; }
	void        PivotToPosR         (void) { if (m_state == STATE::NONE) m_isPivotToPosV = false; }
	short&      GetID               (void) { return m_ID; }
	Pos3D&      GetPosV             (void) { return m_posV; }
	Pos3D&      GetPosR             (void) { return m_posR; }
	Rot3D&      GetRot              (void) { return m_rot; }
	Vector3D    GetVec              (void) { return m_posR - m_posV; }
	Vector3D    GetNor              (void) { Vector3D vec = GetVec(); return *D3DXVec3Normalize(&vec, &vec); }
	void        SetBGCol            (const Color& col) { m_BGCol = col; }
	// <<< ��Ԍn >>>
	void        RemoveState         (void) { SetState(STATE::NONE); }
	void        SetGrabAirMouse     (const float moveForce, const float spinForce, const float zoomForce);
	void        SetReciprocateSpin  (const Pos3D posR, const float dist, const Rot3D rotA, const Rot3D rotB, const int time);
	// <<< ����n >>>
	void        ParallelMove        (const Vector3D move) { m_posV += move; m_posR += move; }
	void        Spin                (const Vector3D add)  { m_spin += add; }

private:
	//========== [[[ �\���̐錾 ]]]
	// �}���`�^�[�Q�b�g�����_�����O���
	struct MTRenderingInfo {
		Texture      textures[2];	// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
		Surface      renders[2];	// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X
		Surface      ZBuff;			// �e�N�X�`�������_�����O�pZ�o�b�t�@
		Viewport     viewport;		// �e�N�X�`�������_�����O�p�r���[�|�[�g
		VertexBuffer vtxBuff;		// �t�B�[�h�o�b�N�p�|���S���̒��_�o�b�t�@
	};
	// ��Ԍn
	struct GrabAirMouseInfo {
		float moveForce;
		float spinForce;
		float zoomForce;
	};
	struct ReciprocateSpinInfo {
		Rot3D rotA = INITROT3D;
		Rot3D rotB = INITROT3D;
		int time  = 0;
		int count = 0;
	};

	//========== [[[ �֐��錾 ]]]
	static void SetUpProjectionMatrix(Device& device, const Scale2D& scale, const float& radian);
	void ProcessState(const PROCESS process);
	void SetState(const STATE state);

	//========== [[[ �ϐ��̐錾 ]]]
	static short    ms_IDCount;
	short           m_ID;
	Pos3D           m_posV;		// ���_
	Pos3D           m_posR;		// �����_
	Pos3D           m_posVib;	// �U���ʒu
	Rot3D           m_rot;
	Vector3D        m_spin;
	float           m_dist;
	float           m_radian;
	float           m_radianGoal;
	float           m_vibForce;
	// ��Ԍn
	bool            m_isPivotToPosV;
	bool            m_isFixed;
	bool            m_isClipping;
	STATE           m_state;
	void*           m_stateInfo;
	// �`��֘A
	Color           m_BGCol;
	MTRenderingInfo m_MTInfo;
};