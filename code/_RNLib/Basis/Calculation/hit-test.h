//========================================
// 
// �����蔻��̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// �N���X��`
//****************************************
// �����蔻��N���X
class _RNC_HitTest {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_HitTest                ();
	~_RNC_HitTest               ();
	void Init                   (void);
	void Uninit                 (void);
	void Update                 (void);
	bool XZ_OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale);
};