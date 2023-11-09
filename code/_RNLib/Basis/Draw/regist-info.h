//========================================
// 
// ���p�o�^���N���X�̒�`
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "camera.h"
#include "draw-info.h"

//----------------------------------------
// �N���X��`
//----------------------------------------
// ���p�`����N���X
class CRegistInfoBase {
public:
	//===== [[[ �֐��錾 ]]]
	CRegistInfoBase();
	virtual ~CRegistInfoBase();
	void AssignToDrawInfo(CDrawInfoBase& drawInfo, const CDrawInfoBase::TYPE& type);

	//===== [[[ �ϐ���` ]]]
	short m_priority;
	short m_clippingID;
};
