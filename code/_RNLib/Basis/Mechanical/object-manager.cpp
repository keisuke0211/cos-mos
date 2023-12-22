//========================================
// 
// �I�u�W�F�N�g�}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�I�u�W�F�N�g�}�l�[�W���[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�����o�ϐ���`
//****************************************
CObjectMgr** CObjectMgr::ms_objectMgrs = NULL;
UShort CObjectMgr::ms_objectMgrNum = NULL;

//========================================
// [�ÓI] �S�}�l�[�W���[�X�V����
//========================================
void CObjectMgr::UpdateAllMgrs(void) {

	// �S�}�l�[�W���[�̑S�X�V�����ƍX�V�������s��
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		ms_objectMgrs[cntObjectMgr]->UpdateAll();
		ms_objectMgrs[cntObjectMgr]->Update();
	}
}

//========================================
// [�ÓI] �S�}�l�[�W���[�������
//========================================
void CObjectMgr::ReleaseAllMgrs(void) {

	// �S�}�l�[�W���[�̑S�X�V�����ƍX�V�������s��
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		ms_objectMgrs[cntObjectMgr]->DeleteAll();
	}
}

//========================================
// [�ÓI] �}�l�[�W���[��r��
//========================================
void CObjectMgr::SubMgr(CObjectMgr* mgr) {

	bool isSub = false;
	for (int cntObjectMgr = 0; cntObjectMgr < ms_objectMgrNum; cntObjectMgr++) {
		if (isSub) 
		{// �r�����I����
			// 1�O�ɂ��炷
			ms_objectMgrs[cntObjectMgr - 1] = ms_objectMgrs[cntObjectMgr];
			ms_objectMgrs[cntObjectMgr] = NULL;
		}
		else 
		{// �r�����ĂȂ�
			if (mgr == ms_objectMgrs[cntObjectMgr]) 
			{// �J�E���g�ƑΏۃA�h���X����v�������A
				// �r���t���O��^�ɂ���
				isSub = true;
			}
		}
	}

	// �r���o�������A���������炵���������Ċm��
	if (isSub) {
		ms_objectMgrNum--;
		RNLib::Memory().ReAlloc(&ms_objectMgrs, ms_objectMgrNum + 1, ms_objectMgrNum);
	}
}

//========================================
// �R���X�g���N�^
//========================================
CObjectMgr::CObjectMgr(const char* name) {

	{// �I�u�W�F�N�g�}�l�[�W���[��ɒǉ�����
		int numOld = ms_objectMgrNum++;
		RNLib::Memory().ReAlloc<CObjectMgr*>(&ms_objectMgrs, numOld, ms_objectMgrNum, false);
		ms_objectMgrs[numOld] = this;
	}

	m_name = NULL;
	RNLib::Memory().AllocString(&m_name, name);
	m_top  = NULL;
	m_cur  = NULL;
	m_num  = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CObjectMgr::~CObjectMgr(void) {

	// �S�J������
	CObjectMgr::DeleteAll();

	// ���g���}�l�[�W���[�񂩂�r��
	SubMgr(this);

	// ���O�̉��
	RNLib::Memory().Release(&m_name);
}

//========================================
// �S�폜����
//========================================
void CObjectMgr::DeleteAll(void) {

	// �擪���珇�ԂɍŌ���܂ō폜�t���O�𗧂�����
	// (���p����̗v�f�͉������邪�A
	//    �{�͉̂�������A���S�t���O�𗧂�����)
	CObject* obj = NULL;
	while (ListLoop(&obj)) {

		obj->Delete();
	}

	// �폜�t���O�������Ă���I�u�W�F�N�g���������
	ReleaseDeleteObj();
}

//========================================
// �ꊇ�X�V����
//========================================
void CObjectMgr::UpdateAll(void) {

	// �擪���珇�ԂɍŌ���܂ōX�V���Ă���
	CObject* pList = NULL;
	while (ListLoop(&pList)) {

		pList->Update();
	}

	// �폜�t���O�������Ă���I�u�W�F�N�g���������
	ReleaseDeleteObj();
}

//========================================
// ���X�g�ɒǉ�
//========================================
void CObjectMgr::AddList(CObject* obj) {

	// ���Ƀ��X�g�ɒǉ����Ă���ꍇ�I��
	if (obj->GetIsAdd()) {
		return;
	}
	else {
		obj->SetIsAdd(true);
	}

	if (m_cur == NULL)
	{// �Ō�������݂��Ȃ�(���܂�1���Ȃ�)���A
		// �擪�ƍŌ����V�K�ɐݒ肷��
		m_top = obj;
		m_cur = obj;

		obj->SetPrev(NULL);	// �O ����
		obj->SetNext(NULL);	// �� ����
	}
	else
	{// �Ō�������݂��鎞�A
		// ���Ō���̎���V�K�ɐݒ肷��
		m_cur->SetNext(obj);

		obj->SetPrev(m_cur);	// �O ���Ō��
		obj->SetNext(NULL);		// �� ����

		// �Ō����V�K�ɐݒ肷��
		m_cur = obj;
	}

	m_num++;
}

//========================================
// ���X�g����r��
//========================================
void CObjectMgr::SubList(CObject* obj) {

	// ���Ƀ��X�g����폜���Ă���ꍇ�I��
	if (obj->GetIsSub()) {
		return;
	}
	else {
		obj->SetIsSub(true);
	}

	if (m_top == obj)
	{// �������擪�̎��A
		// �擪�����ɐݒ肷��
		m_top = obj->GetNext();

		// �擪�̑O�𖳂��ɂ���
		if (m_top != NULL) {
			m_top->SetPrev(NULL);
		}
	}

	if (m_cur == obj)
	{// �������Ō���̎��A
		// �Ō����O�ɐݒ肷��
		m_cur = obj->GetPrev();

		// �Ō���̎��𖳂��ɂ���
		if (m_cur != NULL) {
			m_cur->SetNext(NULL);
		}
	}

	if (obj->GetPrev() != NULL && obj->GetNext() != NULL)
	{// ���܂�Ă��鎞�A
		// �O�Ǝ����q��
		CObject* prev = obj->GetPrev();
		CObject* next = obj->GetNext();
		prev->SetNext(next);
		next->SetPrev(prev);
	}

	m_num--;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�I�u�W�F�N�g�}�l�[�W���[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �I�u�W�F�N�g�������
//========================================
void CObjectMgr::Release(CObject* obj) {

	// ����NULL�ł���ΏI��
	if (obj == NULL)
		return;

	// ���X�g����폜
	SubList(obj);

	// ���
	RNLib::Memory().Release(&obj);
}

//========================================
// �폜�t���O�������Ă���I�u�W�F�N�g���������
//========================================
void CObjectMgr::ReleaseDeleteObj(void) {

	// �擪���珇�ԂɍŌ���܂ŉ�����Ă���
	// (���폜�t���O���^�̎��A�������)
	CObject* obj = m_top;
	while (obj != NULL) {
		CObject* nextObj = obj->GetNext();

		// �폜�t���O���^�̎��A�������
		if (obj->GetIsDelete())
			Release(obj);

		obj = nextObj;
	}
}