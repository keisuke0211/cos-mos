//========================================
// 
// 幾何学関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]幾何学クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Geometry::_RNC_Geometry() {

}

//========================================
// デストラクタ
//========================================
_RNC_Geometry::~_RNC_Geometry() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Geometry::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Geometry::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Geometry::Update(void) {

}

//========================================
// 2点の距離を調べる
//========================================
float _RNC_Geometry::FindDistance(const Pos3D& posA, const Pos3D& posB) {

	const float x = posA.x - posB.x;	// 平方根のX辺
	const float y = posA.y - posB.y;	// 平方根のY辺
	const float z = posA.z - posB.z;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (y * y) + (z * z));
}

//========================================
// 2点の距離を調べる(XY平面)
//========================================
float _RNC_Geometry::FindDistanceXY(const Pos3D& posA, const Pos3D& posB) {

	const float x = posA.x - posB.x;	// 平方根のX辺
	const float y = posA.y - posB.y;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (y * y));
}

//========================================
// 2点の距離を調べる(XZ平面)
//========================================
float _RNC_Geometry::FindDistanceXZ(const Pos3D& posA, const Pos3D& posB) {

	const float x = posA.x - posB.x;	// 平方根のX辺
	const float z = posA.z - posB.z;	// 平方根のZ辺

	// 2点の距離を返す
	return sqrtf((x * x) + (z * z));
}

//========================================
// 面までの垂直距離を調べる
//========================================
float _RNC_Geometry::FindDistanceToPlane(const Pos3D& basePos, const Pos3D& targetPos, const Vector3D& targetNor) {

	Vector3D planeToPosVec = basePos - targetPos;
	Vector3D planeNor = targetNor;

	D3DXVec3Normalize(&planeToPosVec, &planeToPosVec);
	return fabsf(D3DXVec3Dot(&planeToPosVec, &planeNor));
}

//========================================
// 2点の角度を調べる(XY平面)
//========================================
Angle _RNC_Geometry::FindAngleXY(const Pos3D& pos, const Pos3D& targetPos) {
	
	return -(atan2f(targetPos.y - pos.y, targetPos.x - pos.x) - D3DX_PI_HALF);
}

//========================================
// 2点の角度を調べる(XZ平面)
//========================================
Angle _RNC_Geometry::FindAngleXZ(const Pos3D& pos, const Pos3D& targetPos) {
	
	return -(atan2f(targetPos.z - pos.z, targetPos.x - pos.x) - D3DX_PI_HALF);
}

//========================================
// 2点の向きを調べる
//========================================
Rot3D _RNC_Geometry::FindRot(const Pos3D& pos, const Pos3D& targetPos) {

	return RNLib::Geometry().FindVecRot(targetPos - pos);
}

//========================================
// 交点座標を調べる(XZ平面)
//========================================
Pos3D _RNC_Geometry::FindIntersectionXZ(const Pos3D& posA1, const Pos3D& posA2, const Pos3D& posB1, const Pos3D& posB2) {

	const Vector3D B1B2Vec = posB2 - posB1;	// ベクトルB1⇒B2
	const Vector3D A1A2Vec = posA2 - posA1;	// ベクトルA1⇒A2
	const Vector3D A2B1Vec = posB1 - posA2;	// ベクトルA2⇒B1
	const Vector3D B1A1Vec = posA1 - posB1;	// ベクトルB1⇒A1
	const float areaS1 = ((B1B2Vec.x * B1A1Vec.z) - (B1B2Vec.z * B1A1Vec.x)) * 0.5f;	// 面積S1
	const float areaS2 = ((B1B2Vec.x * A2B1Vec.z) - (B1B2Vec.z * A2B1Vec.x)) * 0.5f;	// 面積S2
	const float S1Rate = (areaS1 / (areaS1 + areaS2));	// 面積S1の割合

	return Pos3D(posA1.x + (A1A2Vec.x * S1Rate), 0.0f, posA1.z + (A1A2Vec.z * S1Rate));
}

//========================================
// 角度の差を調べる
//========================================
Angle _RNC_Geometry::FindAngleDifference(const Angle& angle, const Angle& targetAngle) {
	
	// [[[ angle = + & targetAngle = + ]]]
	if ((angle >= 0.0f) && (targetAngle >= 0.0f))
		return targetAngle - angle;
	// [[[ angle = - & targetAngle = - ]]]
	else if ((angle <= 0.0f) && (targetAngle <= 0.0f))
		return targetAngle - angle;

	// [[[ angle = + & targetAngle = - ]]]
	if ((angle >= 0.0f) && (targetAngle <= 0.0f)) {

		// 目標角度が下側に近い
		if (angle - D3DX_PI <= targetAngle)
			return targetAngle - angle;

		// 目標角度が上側に近い
		else if (angle - D3DX_PI >= targetAngle)
			return (D3DX_PI - angle) + (D3DX_PI + targetAngle);
	}

	// [[[ angle = - & targetAngle = + ]]]
	if ((angle <= 0.0f) && (targetAngle >= 0.0f)) {

		// 目標角度が下側に近い
		if (angle + D3DX_PI >= targetAngle)
			return targetAngle - angle;

		// 目標角度が上側に近い
		else if (angle + D3DX_PI <= targetAngle)
			return -(D3DX_PI + angle) - (D3DX_PI - targetAngle);
	}

	assert(false);

	return 0.0f;
}

//========================================
// 向きのベクトルを調べる
//========================================
Vector3D _RNC_Geometry::FindRotVec(const Rot3D& rot) {

	const float sinX = sinf(rot.x);
	const float cosX = cosf(rot.x);
	const float sinY = sinf(rot.y);
	const float cosY = cosf(rot.y);

	Vector3D vec = Vector3D(sinY * cosX, -sinX, cosY * cosX);
	D3DXVec3Normalize(&vec, &vec);

	return vec;
}

//========================================
// ベクトルの向きを調べる
//========================================
Rot3D _RNC_Geometry::FindVecRot(const Vector3D& vec) {

	Normal3D nor = vec;
	D3DXVec3Normalize(&nor, &nor);

	return Rot3D(asinf(-nor.y), atan2f(nor.x, nor.z), 0.0f);
}

//========================================
// 外積から法線を調べる
//========================================
Normal3D _RNC_Geometry::FindVecNor(const Vector3D& vecA, const Vector3D& vecB) {

	Normal3D nor = INITNORMAL3D;
	D3DXVec3Cross(&nor, &vecA, &vecB);
	D3DXVec3Normalize(&nor, &nor);

	return nor;
}

//========================================
// ランダムなベクトルを取得
//========================================
Normal3D _RNC_Geometry::GetRandomVec(void) {

	float x   = RNLib::Number().GetRandomFloat(1.0f);
	float y   = RNLib::Number().GetRandomFloat(1.0f);
	float z   = RNLib::Number().GetRandomFloat(1.0f);
	float sum = x + y + z;
	if (sum > 0.0f) {
		x /= sum;
		y /= sum;
		z /= sum;

		if (rand() % 2 == 0)x *= -1;
		if (rand() % 2 == 0)y *= -1;
		if (rand() % 2 == 0)z *= -1;
	}
	else {
		x = y = z = 0.333333f;
	}

	return Normal3D(x, y, z);
}