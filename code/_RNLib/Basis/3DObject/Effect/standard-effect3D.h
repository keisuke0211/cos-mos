//========================================
// 
// �W���G�t�F�N�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D_cylinder.h"

//****************************************
// �N���X��`
//****************************************
// �W���G�t�F�N�g3D�N���X
class CStandardEffect3D {
public:
	//========== [[[ �֐��錾 ]]]
	CStandardEffect3D();
	~CStandardEffect3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetPriority(const UShort& priority) { m_priority = priority; }
	void CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force);

private:
	//========== [[[ �ϐ��錾 ]]]
	UShort m_priority;
};