//========================================
// 
// �h�[��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "motion3D.h"
#include "../../Calculation/matrix.h"
#include "../../Calculation/ease.h"
#include "../../../RNmain.h"
#include "../../Mechanical/object.h"

//****************************************
// �N���X��`
//****************************************
// �h�[��3D�N���X
class CDoll3D : public CObject {
public:
	//========== [[[ �N���X��` ]]]
	// �{�[����ԃN���X
	class CBoneState {
	public:
		// [[[ �\���̐錾 ]]]
		// �ړ��A�j�����
		struct MoveAnimState {
			CEase::TYPE posEase   = CEase::TYPE::LINEAR;
			Pos3D       oldPos    = INITPOS3D;
			Pos3D       targetPos = INITPOS3D;
			short       counter   = 0;
			short       time      = 0;
		};

		// ��]�A�j�����
		struct SpinAnimState {
			CEase::TYPE rotEase   = CEase::TYPE::LINEAR;
			Rot3D       oldRot    = INITROT3D;
			Rot3D       targetRot = INITROT3D;
			short       counter   = 0;
			short       time      = 0;
		};

		// �g�k�A�j�����
		struct ScalingAnimState {
			CEase::TYPE scaleEase   = CEase::TYPE::LINEAR;
			Scale3D     oldScale    = INITSCALE3D;
			Scale3D     targetScale = INITSCALE3D;
			short       counter     = 0;
			short       time        = 0;
		};

		// �A�j����ԑ���
		struct AnimStateSum {
			MoveAnimState*    move    = NULL;
			SpinAnimState*    spin    = NULL;
			ScalingAnimState* scaling = NULL;
			bool              isStep  = false;
		};

		// [[[ �֐��錾 ]]]
		CBoneState();
		~CBoneState();
		void         UpdateMotion      (const short& counter);
		void         PrepareMotion     (const CMotion3D::BoneMotionData& boneMotionData);
		Pos3D&       GetPos            (void)                                        { return m_pos; }
		void         AddPos            (const Vector3D& addVec)                      { m_pos += addVec; }
		Rot3D&       GetRot            (void)                                        { return m_rot; }
		void         SetRot            (const Rot3D& rot)                            { m_rot = rot; }
		void         AddRot            (const Vector3D& addVec)                      { m_rot += addVec; }
		Scale3D&     GetScale          (void)                                        { return m_scale; }
		void         SetWorldMtx       (const Matrix& worldMtx)                      { m_worldMtx = worldMtx; }
		Matrix&      GetWorldMtx       (void)                                        { return m_worldMtx; }
		void         SetMotionData     (const CMotion3D::BoneMotionData* motionData) { m_motionData = motionData; }
		void         SetParentBoneState(CBoneState* parentBoneState)                 { m_parentBoneState = parentBoneState; }
		CBoneState*& GetParentBoneState(void)                                        { return m_parentBoneState; }

	private:
		// [[[ �ϐ��錾 ]]]
		Pos3D                            m_pos;
		Rot3D                            m_rot;
		Scale3D                          m_scale;
		Matrix                           m_worldMtx;
		AnimStateSum                     m_animeStateSum;
		const CMotion3D::BoneMotionData* m_motionData;
		CBoneState*                      m_parentBoneState;
	};

	//========== [[[ �֐��錾 ]]]
	CDoll3D(const UShort& priority, const short& setUpIdx);
	~CDoll3D();
	void Update          (void);
	void SetUp           (const short& setUpIdx);
	void SetMotion       (const short& motionIdx);
	void OverwriteMotion (const short& motionIdx);
	void SetMotionStop   (const bool& isStop)   { m_motionInfo.isStop = isStop; }
	void SetMotionCounter(const short& counter) { m_motionInfo.counter = counter; }
	// �擾�ݒ�
	Pos3D&      GetPos                 (void)                              { return m_pos; }
	void        SetPos                 (const Pos3D& pos)                  { m_pos = pos; m_isSetPos = true; }
	Rot3D&      GetRot                 (void)                              { return m_rot; }
	void        SetRot                 (const Rot3D& rot)                  { m_rot = rot; }
	void        SetCol                 (const Color& col)                  { m_col = col; }
	void        SetBrightnessOfEmission(const float& brightnessOfEmission) { m_brightnessOfEmission = brightnessOfEmission; }
	float&      GetBrightnessOfEmission(void)                              { return m_brightnessOfEmission; }
	CBoneState& GetBoneState           (const UShort& boneIdx)             { return m_boneStates[boneIdx]; }

private:
	//========== [[[ �\���̒�` ]]]
	struct MotionInfo {
		short idx     = NONEDATA;
		short oldIdx  = NONEDATA;
		short counter = 0;
		bool  isStop  = false;
	};

	//========== [[[ �֐��錾 ]]]
	void   UpdateMotion    (void);
	void   UpdateBone      (const CSetUp3D::CData& setUp);
	Matrix FindBoneWorldMtx(CBoneState& boneState, const CSetUp3D::BoneData& boneData, const Matrix& selfMtx);
	void   PrepareMotion   (void);

	//========== [[[ �ϐ��錾 ]]]
	UShort      m_priority;
	Pos3D       m_pos;
	bool        m_isSetPos;
	Rot3D       m_rot;
	Scale3D     m_scale;
	Color       m_col;
	CBoneState* m_boneStates;
	short       m_setUpIdx;
	MotionInfo  m_motionInfo;
	float       m_brightnessOfEmission;
};