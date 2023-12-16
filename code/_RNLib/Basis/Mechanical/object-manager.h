//========================================
// 
// �I�u�W�F�N�g�}�l�[�W���[�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "object.h"

//****************************************
// �N���X��`
//****************************************
// �I�u�W�F�N�g�}�l�[�W���[�N���X
class CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
	static void          UpdateAllMgrs  (void);
	static void          ReleaseAllMgrs (void);
	static CObjectMgr**& GetObjectMgrs  (void) { return ms_objectMgrs; }
	static UShort&       GetObjectMgrNum(void) { return ms_objectMgrNum; }
	static void          SubMgr         (CObjectMgr* mgr);
	CObjectMgr                          (const char* name);
	virtual ~CObjectMgr                 ();
	virtual void         Update         (void) {}
	void                 UpdateAll      (void);
	virtual void         ReleaseAll     (void);
	void                 Release        (CObject* obj);
	void                 AddList        (CObject* obj);
	void                 SubList        (CObject* obj);
	char*&               GetName        (void) { return m_name; }
	int&                 GetNum         (void) { return m_num; }
	CObject*&            GetTop         (void) { return m_top; }
	bool                 ListLoop       (CObject** obj);

private:
	//========== [[[ �֐��錾 ]]]
	void ReleaseDeleteObj(void);

	//========== [[[ �ϐ��錾 ]]]
	static CObjectMgr** ms_objectMgrs;
	static UShort       ms_objectMgrNum;
	char*    m_name;
	CObject* m_top;	// �擪
	CObject* m_cur;	// �Ō��
	int      m_num;
};