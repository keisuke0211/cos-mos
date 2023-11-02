//========================================
// 
// ドール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "setup3D.h"
#include "motion3D.h"
#include "../../Calculation/matrix.h"
#include "../../Calculation/ease.h"
#include "../../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// ドール3Dクラス
class CDoll3D {
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
			bool              isStep  = false;
		};

		// [[[ 関数宣言 ]]]
		CBoneState();
		~CBoneState();
		void         UpdateMotion      (const short& counter);
		void         PrepareMotion     (const CMotion3D::BoneMotionData& boneMotionData);
		Pos3D&       GetPos            (void)                                        { return m_pos; }
		Rot3D&       GetRot            (void)                                        { return m_rot; }
		Scale3D&     GetScale          (void)                                        { return m_scale; }
		void         SetWorldMtx       (const Matrix& worldMtx)                      { m_worldMtx = worldMtx; }
		Matrix&      GetWorldMtx       (void)                                        { return m_worldMtx; }
		void         SetMotionData     (const CMotion3D::BoneMotionData* motionData) { m_motionData = motionData; }
		CBoneState*& GetParentBoneState(void)                                        { return m_parentBoneState; }

	private:
		// [[[ 変数宣言 ]]]
		Pos3D                            m_pos;
		Rot3D                            m_rot;
		Scale3D                          m_scale;
		Matrix                           m_worldMtx;
		AnimStateSum                     m_animeStateSum;
		const CMotion3D::BoneMotionData* m_motionData;
		CBoneState*                      m_parentBoneState;
	};

	//========== [[[ 関数宣言 ]]]
	CDoll3D();
	~CDoll3D();
	void Update          (void);
	void SetUp           (const short& setUpIdx);
	void SetMotion       (const short& motionIdx);
	void OverwriteMotion (const short& motionIdx);
	void SetMotionStop   (const bool& isStop)   { m_motionInfo.isStop = isStop; }
	void SetMotionCounter(const short& counter) { m_motionInfo.counter = counter; }
	// 取得設定
	Pos3D& GetPos                 (void)                              { return m_pos; }
	void   SetPos                 (const Pos3D& pos)                  { m_pos = pos; }
	Rot3D& GetRot                 (void)                              { return m_rot; }
	void   SetRot                 (const Rot3D& rot)                  { m_rot = rot; }
	void   SetCol                 (const Color& col)                  { m_col = col; }
	void   SetBrightnessOfEmission(const float& brightnessOfEmission) { m_brightnessOfEmission = brightnessOfEmission; }
	float& GetBrightnessOfEmission(void)                              { return m_brightnessOfEmission; }

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
	void   UpdateBone      (const CSetUp3D::CData& setUp);
	Matrix FindBoneWorldMtx(CBoneState& boneState, const CSetUp3D::BoneData& boneData, const Matrix& selfMtx);
	void   PrepareMotion   (void);

	//========== [[[ 変数宣言 ]]]
	Pos3D       m_pos;
	Rot3D       m_rot;
	Scale3D     m_scale;
	Color       m_col;
	float       m_brightnessOfEmission;
	CBoneState* m_boneStates;
	short       m_setUpIdx;
	MotionInfo  m_motionInfo;
};