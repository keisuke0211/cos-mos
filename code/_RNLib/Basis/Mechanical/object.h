//========================================
// 
// オブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// オブジェクトクラス
class CObject {
public:
	//========== [[[ 関数宣言 ]]]
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
	//========== [[[ 友達宣言 ]]]
	friend class CObjectMgr;

	//========== [[[ 関数宣言 ]]]
	virtual void Update(void) = 0;

	//========== [[[ 変数宣言 ]]]
	CObject* m_prev;
	CObject* m_next;
	bool     m_isDelete;
	bool     m_isAdd;
	bool     m_isSub;
};