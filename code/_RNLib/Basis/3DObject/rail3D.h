//========================================
// 
// ���[��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/string.h"
#include"../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// ���[��3D�N���X
class CRail3D {
public:
	//========== [[[ �֐��錾 ]]]
	CRail3D            (const String& loadPath);
	~CRail3D           ();
	void    Clear      (void);
	void    Load       (const String& loadPath);
	void    Save       (const String& loadPath);
	Matrix  GetMtx     (float rate);
	Pos3D&  GetPoint   (const UShort& idx) { return m_points[idx]; }
	void    AddPoint   (void);
	void    SubPoint   (const UShort& idx);
	UShort& GetPointNum(void)              { return m_pointNum; }

private:
	//========== [[[ �֐��錾 ]]]
	void CalculateCatmullRomSpline(float rate, Pos3D& pos, Vector3D& tangent);

	//========== [[[ �ϐ��錾 ]]]
	Pos3D* m_points;
	UShort m_pointNum;
};