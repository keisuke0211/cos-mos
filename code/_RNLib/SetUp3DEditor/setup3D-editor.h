//========================================
// 
// �Z�b�g�A�b�v3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// �N���X��`
//****************************************
// �Z�b�g�A�b�v3D�G�f�B�^�[�N���X
class CSetUp3DEditor {
public:
	//========== [[[ �֐��錾 ]]]
	CSetUp3DEditor();
	~CSetUp3DEditor();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ �萔��` ]]]
	static const int SLITHER_TIME = 60;

	//========== [[[ �񋓌^��` ]]]
	enum class MESSAGE_TYPE { NONE, SETUP_RELOAD_SUCCEEDED, MOTION_RELOAD_SUCCEEDED, SETUP_RELOAD_FAILED, MOTION_RELOAD_FAILED, };

	//========== [[[ �ϐ��錾 ]]]
	CCamera*     m_camera;
	CDoll3D*     m_doll;
	Vector3D     m_slither;
	int          m_slitherCounter;
	char*        m_editPath;
	char*        m_motionPath;
	MESSAGE_TYPE m_messageType;
	UShort       m_messageCounter;
};