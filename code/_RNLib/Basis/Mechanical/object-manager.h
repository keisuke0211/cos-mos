//========================================
// 
// オブジェクトマネージャーのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "object.h"

//****************************************
// クラス定義
//****************************************
// オブジェクトマネージャークラス
class CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CObjectMgr         (const char* name);
	virtual ~CObjectMgr();
	void     UpdateAll (void);
	void     DeleteAll (void);
	void     AddList   (CObject* obj);
	void     SubList   (CObject* obj);
	char*    GetName   (void) { return m_name; }
	UInt     GetNum    (void) { return m_num; }
	CObject* GetTop    (void) { return m_top; }
	template<class T> bool ListLoop(T** obj) {

		CObject** casted = (CObject**)obj;

		if (*casted == NULL)
			*casted = m_top;
		else
			*casted = (*casted)->GetNext();

		return (*obj != NULL);
	}

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_Agent;
	friend class _RNC_Text2D;

	//========== [[[ 関数宣言 ]]]
	static void          UpdateAllMgrs   (void);
	static void          ReleaseAllMgrs  (void);
	static CObjectMgr**& GetObjectMgrs   (void) { return ms_objectMgrs; }
	static UShort&       GetObjectMgrNum (void) { return ms_objectMgrNum; }
	static void          SubMgr          (CObjectMgr* mgr);
	virtual void         Update          (void) {}
	void                 Release         (CObject* obj);
	void                 ReleaseDeleteObj(void);

	//========== [[[ 変数宣言 ]]]
	static CObjectMgr** ms_objectMgrs;
	static UShort       ms_objectMgrNum;
	char*    m_name;
	CObject* m_top;	// 先頭
	CObject* m_cur;	// 最後尾
	UInt     m_num;
};