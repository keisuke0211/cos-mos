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
class _RNC_CameraMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_CameraMgr ();
	~_RNC_CameraMgr();

private:
	//========== [[[ �֐��錾 ]]]
	void Update(void);
};