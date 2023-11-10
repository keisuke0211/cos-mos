//========================================
// 
// カウントの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// カウントクラス
class CCount {
public:
	//========== [[[ 関数宣言 ]]]
	CCount           ();
	~CCount          ();
	void   Init      (void);
	void   Uninit    (void);
	void   Update    (void);
	UShort GetCount  (void) { return m_count; };
	bool   GetBlinkF2(void) { return m_isBlinkF2; }
	bool   GetBlinkF4(void) { return m_isBlinkF4; }

private:
	//========== [[[ 変数宣言 ]]]
	UShort m_count;
	bool   m_isBlinkF2;
	bool   m_isBlinkF4;
};