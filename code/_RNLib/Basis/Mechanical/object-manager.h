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
	static void  UpdateAllMgrs(void);
	static void  SubMgr(CObjectMgr* mgr);
	             CObjectMgr();
	virtual      ~CObjectMgr();
	virtual void Update(void) {}
	virtual void UpdateAll(void);
	virtual void ReleaseAll(void);
	void         ReleaseDeleteObj(void);
	void         Release(CObject* obj);
	void         AddList(CObject* obj);
	void         SubList(CObject* obj);
	int          GetNum(void) { return m_num; }
	CObject*     GetTop(void) { return m_top; }
	bool         ListLoop(CObject** obj);

private:
	//========== [[[ 変数宣言 ]]]
	CObject* m_top;	// 先頭
	CObject* m_cur;	// 最後尾
	int m_num;
	static CObjectMgr** ms_objectMgrs;
	static UShort ms_objectMgrNum;
};