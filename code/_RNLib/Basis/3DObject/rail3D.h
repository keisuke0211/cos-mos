//========================================
// 
// レール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/string.h"
#include"../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// レール3Dクラス
class CRail3D {
public:
	//========== [[[ 関数宣言 ]]]
	CRail3D            (const String& loadPath);
	~CRail3D           ();
	void    Clear      (void);
	void    Load       (const String& loadPath);
	void    Save       (const String& loadPath, const float& scale = 1.0f);
	Matrix  GetMtx     (float rate, const bool& isApplyScale);
	bool&   GetIsLoop  (void)               { return m_isLoop; }
	void    SetIsLoop  (const bool& isLoop) { m_isLoop = isLoop; }
	Pos3D&  GetPoint   (const UShort& idx)  { return m_points[idx]; }
	void    AddPoint   (void);
	void    SubPoint   (const UShort& idx);
	UShort& GetPointNum(void)               { return m_pointNum; }
	float&  GetScale   (void)               { return m_scale; }
	void    SetScale   (const float& scale) { m_scale = scale; }

private:
	//========== [[[ 関数宣言 ]]]
	void CalculateCatmullRomSpline(float rate, Pos3D& pos, Vector3D& tangent);

	//========== [[[ 変数宣言 ]]]
	bool   m_isLoop;
	float  m_scale;
	Pos3D* m_points;
	UShort m_pointNum;
};