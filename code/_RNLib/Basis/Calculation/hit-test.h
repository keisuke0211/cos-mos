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
class CHitTest {
public:
	//========== [[[ �֐��錾 ]]]
	CHitTest();
	~CHitTest();
	void Init(void);
	void Uninit(void);
	void Update(void);
	class XZ {
	public:
		static bool OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale);
	};
};