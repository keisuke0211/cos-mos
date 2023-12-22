//========================================
// 
// �I�u�W�F�N�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �I�u�W�F�N�g�N���X
class CObject {
public:
	//========== [[[ �֐��錾 ]]]
	             CObject    ();
	virtual      ~CObject   ();
	void         Delete     (void)              { if    (this != NULL) m_isDelete = true; }
	void         SetNext    (CObject* next)     { if    (this != NULL) m_next = next; }
	CObject*     GetNext    (void)              { return this == NULL ? NULL : m_next; }
	void         SetPrev    (CObject* prev)     { if    (this != NULL) m_prev = prev; }
	CObject*     GetPrev    (void)              { return this == NULL ? NULL : m_prev; }
	bool         GetIsDelete(void)              { return this == NULL ? true : m_isDelete; }
	void         SetIsAdd   (const bool& isAdd) { if    (this != NULL) m_isAdd = isAdd; }
	bool         GetIsAdd   (void)              { return this == NULL ? false : m_isAdd; }
	void         SetIsSub   (const bool& isSub) { if    (this != NULL) m_isSub = isSub; }
	bool         GetIsSub   (void)              { return this == NULL ? false : m_isSub; }

private:
	//========== [[[ �F�B�錾 ]]]
	friend class CObjectMgr;

	//========== [[[ �֐��錾 ]]]
	virtual void Update(void) = 0;

	//========== [[[ �ϐ��錾 ]]]
	CObject* m_prev;
	CObject* m_next;
	bool     m_isDelete;
	bool     m_isAdd;
	bool     m_isSub;
};