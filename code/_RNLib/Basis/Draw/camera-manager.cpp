//========================================
// 
// �J�����}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�J�����}�l�[�W���[�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CCameraMgr::CCameraMgr() {

	m_deletedCamera = NULL;
	m_deletedCameraNum = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CCameraMgr::~CCameraMgr() {

	CMemory::Release(&m_deletedCamera);
}

//========================================
// �X�V����
//========================================
void CCameraMgr::Update(void) {

}

//========================================
// �폜�ς݃J�������`�F�b�N
//========================================
bool CCameraMgr::CheckDeletedCamera(CCamera*& camera) {

	if (m_deletedCameraNum == 0)
		return false;

	for (int cntCamera = 0; cntCamera < m_deletedCameraNum; cntCamera++) {
		if (m_deletedCamera[cntCamera] == camera) {
			return true;
		}
	}

	return false;
}

//========================================
// �폜�ς݃J�����Ƃ��Ēǉ�
//========================================
void CCameraMgr::AddDeletedCamera(CCamera* camera) {

	const UShort oldNum = m_deletedCameraNum++;
	CMemory::ReAlloc(&m_deletedCamera, oldNum, m_deletedCameraNum);
	m_deletedCamera[oldNum] = camera;
}

//========================================
// �폜�ς݃J���������
//========================================
void CCameraMgr::ReleaseDeletedCamera(void) {

	CMemory::Release(&m_deletedCamera);
	m_deletedCameraNum = 0;
}