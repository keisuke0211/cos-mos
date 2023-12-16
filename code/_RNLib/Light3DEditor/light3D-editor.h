//========================================
// 
// ���C�g3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// �N���X��`
//****************************************
// ���C�g3D�G�f�B�^�[�N���X
class CLight3DEditor {
public:
	//========== [[[ �֐��錾 ]]]
	CLight3DEditor ();
	~CLight3DEditor();
	void Init      (void);
	void Uninit    (void);
	void Update    (void);

private:
	//========== [[[ �ϐ��錾 ]]]
	CCamera*  m_camera;
	CLight3D* m_light;
	bool      m_isLockSubLinearLight;
	UShort    m_selectLinearLightIdx;
};