//========================================
// 
// ドール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "motion3D.h"
#include "../../Draw/PutObject/model.h"
#include "../../Calculation/matrix.h"
#include "../../Calculation/ease.h"
#include "../../../RNmain.h"
#include "../../Mechanical/object.h"
#include "../../Mechanical/memory.h"

//****************************************
// クラス定義
//****************************************
// ドール3Dクラス
class CDoll3D : public CObject {
public:
	//========== [[[ クラス定義 ]]]
	// ボーン状態クラス
	class CBoneState {
	public:
		// [[[ 構造体宣言 ]]]
		// 移動アニメ状態
		struct MoveAnimState {
			CEase::TYPE posEase   = CEase::TYPE::LINEAR;
			Pos3D       oldPos    = INITPOS3D;
			Pos3D       targetPos = INITPOS3D;
			short       counter   = 0;
			short       time      = 0;
		};

		// 回転アニメ状態
		struct SpinAnimState {
			CEase::TYPE rotEase   = CEase::TYPE::LINEAR;
			Rot3D       oldRot    = INITROT3D;
			Rot3D       targetRot = INITROT3D;
			short       counter   = 0;
			short       time      = 0;
		};

		// 拡縮アニメ状態
		struct ScalingAnimState {
			CEase::TYPE scaleEase   = CEase::TYPE::LINEAR;
			Scale3D     oldScale    = INITSCALE3D;
			Scale3D     targetScale = INITSCALE3D;
			short       counter     = 0;
			short       time        = 0;
		};

		// アニメ状態総括
		struct AnimStateSum {
			MoveAnimState*    move    = NULL;
			SpinAnimState*    spin    = NULL;
			ScalingAnimState* scaling = NULL;
		};

		// 揺れ状態
		struct SwayingState {
			Pos3D targetAddPos = INITPOS3D;
			Pos3D oldAddPos    = INITPOS3D;
			Pos3D addPos       = INITPOS3D;
			short counter      = 0;
			short counterMax   = 0;
		};

		// [[[ 関数宣言 ]]]
		CBoneState();
		~CBoneState();
		void           Update            (const short& motionCounter, const CSetUp3D::BoneData& boneData);
		void           PrepareMotion     (const CMotion3D::BoneMotionData& boneMotionData);
		Pos3D&         GetAnimPos        (void)                                        { return m_animPos; }
		void           SetAddPos         (const Pos3D& pos)                            { m_addPos = pos; }
		Pos3D&         GetAddPos         (void)                                        { return m_addPos; }
		void           AddAddPos         (const Vector3D& addVec)                      { m_addPos += addVec; }
		Rot3D&         GetAnimRot        (void)                                        { return m_animRot; }
		void           SetAddRot         (const Rot3D& rot)                            { m_addRot = rot; }
		Rot3D&         GetAddRot         (void)                                        { return m_addRot; }
		void           AddAddRot         (const Vector3D& addVec)                      { m_addRot += addVec; }
		Scale3D&       GetAnimScale      (void)                                        { return m_animScale; }
		void           SetAddScale       (const Scale3D& scale)                        { m_addScale = scale; }
		Scale3D&       GetAddScale       (void)                                        { return m_addScale; }
		void           SetWorldMtx       (const Matrix& worldMtx)                      { m_worldMtx = worldMtx; }
		Matrix&        GetWorldMtx       (void)                                        { return m_worldMtx; }
		SwayingState*& GetSwayingState   (void)                                        { return m_swayingState; }
		void           CreateSwayingState(void)                                        { CMemory::Alloc(&m_swayingState); *m_swayingState = {}; }
		void           SetMotionData     (const CMotion3D::BoneMotionData* motionData) { m_motionData = motionData; }
		void           SetParentBoneState(CBoneState* parentBoneState)                 { m_parentBoneState = parentBoneState; }
		CBoneState*&   GetParentBoneState(void)                                        { return m_parentBoneState; }

	private:
		// [[[ 変数宣言 ]]]
		Pos3D                            m_animPos;
		Pos3D                            m_addPos;
		Rot3D                            m_animRot;
		Rot3D                            m_addRot;
		Scale3D                          m_animScale;
		Scale3D                          m_addScale;
		Matrix                           m_worldMtx;
		AnimStateSum                     m_animeStateSum;
		SwayingState*                    m_swayingState;
		const CMotion3D::BoneMotionData* m_motionData;
		CBoneState*                      m_parentBoneState;
	};

	//========== [[[ 関数宣言 ]]]
	CDoll3D                            (const UShort& priority, const short& setUpIdx);
	~CDoll3D                           ();
	void        Update                 (void);
	void        SetUp                  (const short& setUpIdx);
	void        SetMotion              (const short& motionIdx);
	void        OverwriteMotion        (const short& motionIdx);
	void        SetMotionStop          (const bool& isStop)                { m_motionInfo.isStop = isStop; }
	void        SetMotionCounter       (const short& counter)              { m_motionInfo.counter = counter; }
	void        SetPos                 (const Pos3D& pos)                  { m_pos = pos; m_isSetPos = true; }
	Pos3D&      GetPos                 (void)                              { return m_pos; }
	void        SetRot                 (const Rot3D& rot)                  { m_rot = rot; }
	Rot3D&      GetRot                 (void)                              { return m_rot; }
	void        SetScale               (const Scale3D& scale)              { m_scale = scale; }
	Scale3D&    GetScale               (void)                              { return m_scale; }
	void        SetCol                 (const Color& col)                  { m_col = col; }
	void        SetIsShow              (const bool& isShow)                { m_isShow = isShow; }
	bool&       GetIsShow              (void)                              { return m_isShow; }
	void        SetBrightnessOfEmission(const float& brightnessOfEmission) { m_brightnessOfEmission = brightnessOfEmission; }
	float&      GetBrightnessOfEmission(void)                              { return m_brightnessOfEmission; }
	CBoneState& GetBoneState           (const UShort& boneIdx)             { return m_boneStates[boneIdx]; }

private:
	//========== [[[ 構造体定義 ]]]
	struct MotionInfo {
		short idx     = NONEDATA;
		short oldIdx  = NONEDATA;
		short counter = 0;
		bool  isStop  = false;
	};

	//========== [[[ 関数宣言 ]]]
	void   UpdateMotion    (void);
	void   UpdateBone      (CSetUp3D::CData& setUp);
	void   DrawModelVtxIdx (CModel::Vertex3DInfo*& vtxInfo, ULong& vtxNum);
	void   DrawFace        (CSetUp3D::CData& setUp, CModel::Vertex3DInfo**& vtxInfo, ULong*& vtxNum);
	Matrix FindBoneWorldMtx(const short& idx, CBoneState*& boneState, CSetUp3D::BoneData*& boneData, Matrix& selfMtx);
	void   PrepareMotion   (void);

	//========== [[[ 変数宣言 ]]]
	UShort      m_priority;
	Pos3D       m_pos;
	bool        m_isSetPos;
	Rot3D       m_rot;
	Scale3D     m_scale;
	Color       m_col;
	CBoneState* m_boneStates;
	bool        m_isShow;
	short       m_setUpIdx;
	MotionInfo  m_motionInfo;
	float       m_brightnessOfEmission;
};