//========================================
// 
// �G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object-manager.h"

//****************************************
// �N���X��`
//****************************************
// �G�t�F�N�g3D�N���X
class CEffect3D {
public:
	//========== [[[ �\���̐錾 ]]]
	struct Circle {

	};

	//========== [[[ �֐��錾 ]]]
	CEffect3D();
	~CEffect3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void CreateCylinder(const Pos3D& pos, const UShort& divNum, const Circle& circleA, const Circle& circleB);
};