//========================================
// 
// ���p�`����N���X�̒�`
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "camera.h"

//----------------------------------------
// �N���X��`
//----------------------------------------
// ���p�`����N���X
class CDrawInfoBase {
public:
	//===== [[[ �֐��錾 ]]]
	CDrawInfoBase         ();
	virtual ~CDrawInfoBase();
	virtual void Draw     (Device& device, const Matrix& viewMtx) {}

	//===== [[[ �ϐ���` ]]]
	short m_clippingID;
};
