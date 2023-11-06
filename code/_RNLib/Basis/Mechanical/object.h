//========================================
// 
// オブジェクトのヘッダファイル
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
	CObject();
	virtual ~CObject();
	virtual void Update(void) = 0;
	void Delete(void) { m_isDelete = true; }
	// 設定取得
	void     SetNext  (CObject* next) { m_next = next; }
	CObject* GetNext  (void)          { return m_next; }
	void     SetPrev  (CObject* prev) { m_prev = prev; }
	CObject* GetPrev  (void)          { return m_prev; }
	bool     GetDelete(void)          { return m_isDelete; }

private:
	//========== [[[ 変数宣言 ]]]
	CObject* m_prev;
	CObject* m_next;
	bool     m_isDelete;
};