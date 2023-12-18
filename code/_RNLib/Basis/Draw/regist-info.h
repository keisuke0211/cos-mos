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
	CRegistInfoBase         ();
	virtual ~CRegistInfoBase();
	void ClearParameter     (void);
	void AssignToDrawInfo   (CDrawInfoBase& drawInfo);

	//===== [[[ �ϐ���` ]]]
	short m_clippingID;
};
