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
class CEffect3DMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	CEffect3DMgr();
	~CEffect3DMgr();
	void Update(void);
};