//========================================
// 
// ���C�g3D�}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// �N���X��`
//****************************************
// ���C�g3D�}�l�[�W���[�N���X
class _RNC_Light3DMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Light3DMgr();
	~_RNC_Light3DMgr();
	void Update(void);
};