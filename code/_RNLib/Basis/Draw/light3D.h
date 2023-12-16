//========================================
// 
// ���C�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/string.h"
#include "../Mechanical/object.h"

//****************************************
// �N���X��`
//****************************************
// ���C�g3D�N���X
class CLight3D : CObject {
public:
	//========== [[[ �\���̒�` ]]]
	// �������\����
	struct LinearLight {
		Matrix rotMtx = INITMATRIX;
		Color  col    = COLOR_WHITE;
	};

	//========== [[[ �֐��錾 ]]]
	static void AllDisable(Device& device);
	CLight3D                      (const String& loadPath);
	~CLight3D                     ();
	void         Update           (void) {}
	void         Load             (const String& loadPath);
	void         Setting          (Device& device);
	short&       GetID            (void) { return m_ID; }
	LinearLight& GetLinearLight   (const UShort idx) { return m_linearLights[idx]; }
	void         AddLinearLight   (void);
	UShort&      GetLinearLightNum(void) { return m_linearLightNum; }
	Rot3D&       GetRot           (void) { return m_rot; }
	void         SetRot           (const Rot3D& rot) { m_rot = rot; }

private:
	//========== [[[ �ϐ��錾 ]]]
	static short  ms_IDCount;
	static UShort ms_linearLightNumOld;
	short         m_ID;
	LinearLight*  m_linearLights;
	UShort        m_linearLightNum;
	Rot3D         m_rot;
	Color         m_col;
};