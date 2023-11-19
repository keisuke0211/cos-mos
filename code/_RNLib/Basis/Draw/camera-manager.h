//========================================
// 
// �J�����}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// �N���X��`
//****************************************
// �J�����}�l�[�W���[�N���X
class CCameraMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	CCameraMgr();
	~CCameraMgr();
	void Update(void);
	bool CheckDeletedCamera(CCamera*& camera);
	void AddDeletedCamera(CCamera* camera);
	void ReleaseDeletedCamera(void);

private:
	//========== [[[ �ϐ��錾 ]]]
	CCamera** m_deletedCamera;
	UShort m_deletedCameraNum;
};