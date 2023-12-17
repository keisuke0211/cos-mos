//========================================
// 
// ���[��3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// �N���X��`
//****************************************
// ���[��3D�G�f�B�^�[�N���X
class CRail3DEditor {
public:
	//========== [[[ �֐��錾 ]]]
	CRail3DEditor();
	~CRail3DEditor();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ �ϐ��錾 ]]]
	CCamera m_camera;
	String  m_editPath;
	CRail3D m_rail3D;
	short   m_counter;
	short   m_time;
	short   m_selectPointIdx;
	bool    m_isSaved;
};