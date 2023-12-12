//========================================
// 
// カメラの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object.h"
#include "../Mechanical/memory.h"
#include "color.h"

//****************************************
// クラス定義
//****************************************
// カメラクラス
class CCamera : public CObject {
public:
	//========== [[[ 列挙型定義 ]]]
	// 状態
	enum class STATE {
		NONE,
		GRAB_AIR_MOUSE,		// マウスで宙を掴む
		RECIPROCATE_SPIN,	// 往復回転
		MAX,
	};

	// 振動の強さ
	enum class VIB_POWER { LOW, MID, HIGH, MAX, };

	//========== [[[ 関数宣言 ]]]
	static void StartRenderingScreen(Device& device);
	            CCamera             (const Scale2D& scale2D);
	            ~CCamera            ();
	void        Update              (void);
	void        StartRendering      (Device& device);
	void        EndRendering        (Device& device);
	UShort&     GetID               (void)                   { return m_ID; }
	void        SetPosVAndPosR      (const Pos3D& posV, const Pos3D& posR);
	Pos3D&      GetPosV             (void)                   { return m_posV; }
	Pos3D&      GetPosR             (void)                   { return m_posR; }
	Rot3D&      GetRot              (void)                   { return m_rot; }
	Scale2D&    GetScale2D          (void)                   { return m_scale; }
	Vector3D    GetVec              (void)                   { return m_posR - m_posV; }
	Vector3D    GetNor              (void)                   { Vector3D vec = GetVec(); return *D3DXVec3Normalize(&vec, &vec); }
	Viewport    GetViewport         (void)                   { return m_MTInfo.viewport; }
	float&      GetRadian           (void)                   { return m_radian; }
	void        SetRadianGoal       (const float& goal)      { m_radianGoal = goal; }
	void        SetVib              (const float& vibPower);
	void        PivotToPosV         (void)                   { if (m_state == STATE::NONE) m_isPivotToPosV = true; }
	void        PivotToPosR         (void)                   { if (m_state == STATE::NONE) m_isPivotToPosV = false; }
	void        SetFixed            (const bool& isFixid)    { m_isFixed = isFixid; }
	void        SetDraw             (const bool& isDraw)     { m_isDraw = isDraw; }
	bool&       GetDraw             (void)                   { return m_isDraw; }
	void        SetBGCol            (const Color& col)       { m_BGCol = col; }
	void        SetIsClipping       (const bool& isClipping) { m_isClipping = isClipping; }
	bool&       GetIsClipping       (void)                   { return m_isClipping; }
	Texture&    GetTexture          (void)                   { return m_MTInfo.textures[0]; }
	float&      GetMotionBlurPower  (void)                   { return m_motionBlur.power; }
	void        SetMotionBlurPower  (const float& power)     { m_motionBlur.power = power; }
	float&      GetMotionBlurScale  (void)                   { return m_motionBlur.scale; }
	void        SetMotionBlurScale  (const float& scale)     { m_motionBlur.scale = scale; }
	float&      GetMotionBlurAngle  (void)                   { return m_motionBlur.angle; }
	void        SetMotionBlurAngle  (const Angle& angle)     { m_motionBlur.angle = angle; }
	Color&      GetMotionBlurColor  (void)                   { return m_motionBlur.col; }
	void        SetMotionBlurColor  (const Color& col)       { m_motionBlur.col = col; }
	// <<< 状態系 >>>
	void        RemoveState         (void) { SetState(STATE::NONE); }
	void        SetGrabAirMouse     (const float moveForce, const float spinForce, const float zoomForce);
	void        SetReciprocateSpin  (const Pos3D posR, const float dist, const Rot3D rotA, const Rot3D rotB, const int time);
	// <<< 操作系 >>>
	void        ParallelMove        (const Vector3D move) { m_posV += move; m_posR += move; }
	void        Spin                (const Vector3D add)  { m_spin += add; }

private:
	//========== [[[ 構造体宣言 ]]]
	// モーションブラー情報
	struct MotionBlur {
		float power = 0.0f;
		float scale = 1.0f;
		Angle angle = 0.0f;
		Color col   = INITCOLOR;
	};
	// マルチターゲットレンダリング情報
	struct MTRenderingInfo {
		Texture      textures[2];	// レンダリングターゲット用テクスチャ
		Surface      renders[2];	// テクスチャレンダリング用インターフェース
		Surface      ZBuff;			// テクスチャレンダリング用Zバッファ
		Viewport     viewport;		// テクスチャレンダリング用ビューポート
		VertexBuffer vtxBuff;		// フィードバック用ポリゴンの頂点バッファ
	};
	// 状態系
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

	//========== [[[ 関数宣言 ]]]
	static void SetUpProjectionMatrix(Device& device, const Scale2D& scale, const float& radian);
	void ProcessState(const PROCESS process);
	void SetState(const STATE state);

	//========== [[[ 変数の宣言 ]]]
	static UShort   ms_IDCount;
	UShort          m_ID;
	Pos3D           m_posV;		// 視点
	Pos3D           m_posR;		// 注視点
	Pos3D           m_posVib;	// 振動位置
	Rot3D           m_rot;
	Vector3D        m_spin;
	Scale2D         m_scale;
	float           m_dist;
	float           m_radian;
	float           m_radianGoal;
	float           m_vibForce;
	// 状態系
	bool            m_isPivotToPosV;
	bool            m_isFixed;
	STATE           m_state;
	void*           m_stateInfo;
	// 描画関連
	bool            m_isDraw;
	Color           m_BGCol;
	bool            m_isClipping;
	MTRenderingInfo m_MTInfo;
	MotionBlur      m_motionBlur;
};