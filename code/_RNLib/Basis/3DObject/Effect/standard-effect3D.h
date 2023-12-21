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
class _RNC_StandardEffect3D {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_StandardEffect3D        ();
	~_RNC_StandardEffect3D       ();
	void SetPriority             (const UShort& priority) { m_priority = priority; }
	void CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force);

private:
	//========== [[[ �F�B�錾 ]]]
	friend class C3DObject;

	//========== [[[ �֐��錾 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);

	//========== [[[ �ϐ��錾 ]]]
	UShort m_priority;
};