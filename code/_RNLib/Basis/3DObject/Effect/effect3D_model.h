//========================================
// 
// �G�t�F�N�g3D(���f��)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D-base.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D(���f��)�N���X
class CEffect3D_Model : CEffect3D_Base {
public:
	//========== [[[ �֐��錾 ]]]
	CEffect3D_Model (const short& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const EASE_TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType);
	~CEffect3D_Model();

private:
	//========== [[[ �֐��錾 ]]]
	void Update(void);
};