//========================================
// 
// 幾何学関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"
#include "../Draw/camera.h"

//****************************************
// クラス定義
//****************************************
// 幾何学クラス
class _RNC_Geometry {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Geometry               ();
	~_RNC_Geometry              ();
	float    FindDistance       (const Pos3D& posA, const Pos3D& posB);
	float    FindDistanceXY     (const Pos3D& posA, const Pos3D& posB);
	float    FindDistanceXZ     (const Pos3D& posA, const Pos3D& posB);
	float    FindDistanceToPlane(const Pos3D& basePos, const Pos3D& targetPos, const Vector3D& targetNor);
	Angle    FindAngleXY        (const Pos3D& pos, const Pos3D& targetPos);
	Angle    FindAngleXZ        (const Pos3D& pos, const Pos3D& targetPos);
	Rot3D    FindRot            (const Pos3D& pos, const Pos3D& targetPos);
	Pos3D    FindIntersectionXZ (const Pos3D& posA1, const Pos3D& posA2, const Pos3D& posB1, const Pos3D& posB2);
	Angle    FindAngleDifference(const Angle& angle, const Angle& targetAngle);
	Vector3D FindRotVec         (const Rot3D& rot);
	Rot3D    FindVecRot         (const Vector3D& vec);
	Normal3D FindVecNor         (const Vector3D& vecA, const Vector3D& vecB);
	Normal3D GetRandomVec       (void);

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_Calculation;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};