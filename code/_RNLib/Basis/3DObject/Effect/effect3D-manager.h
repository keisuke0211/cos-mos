//========================================
// 
// �G�t�F�N�g3D�}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object-manager.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D�}�l�[�W���[�N���X
class _RNC_Effect3DMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Effect3DMgr ();
	~_RNC_Effect3DMgr();

private:
	//========== [[[ �֐��錾 ]]]
	void Update(void);
};