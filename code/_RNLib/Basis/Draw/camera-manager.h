//========================================
// 
// �J�����}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// �N���X��`
//****************************************
// �J�����}�l�[�W���[�N���X
class CCameraMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	CCameraMgr();
	~CCameraMgr();
	void Update(void);
};