//========================================
// 
// �G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D_cylinder.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D�N���X
class _RNC_Effect3D {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Effect3D                     ();
	~_RNC_Effect3D                    ();
	void                Init          (void);
	void                Uninit        (void);
	void                Update        (void);
	CEffect3D_Cylinder* CreateCylinder(const UShort& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const _RNC_Ease::TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType, const UShort& divNum, const CEffect3D_Cylinder::Circle& circleFront, const CEffect3D_Cylinder::Circle& circleBack);
};