//========================================
// 
// 遷移の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// 遷移クラス
class CTransition {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class STATE { NONE, OPEN, OPEN_WAIT, CLOSE, };
	enum class TYPE { NONE, FADE, };

	//========== [[[ 関数宣言 ]]]
	CTransition();
	~CTransition();
	void   Init    (void);
	void   Uninit  (void);
	void   Update  (void);
	bool   Open    (const TYPE& type, const UShort& time);
	bool   Close   (const TYPE& type, const Color& col, const UShort& time);
	STATE& GetState(void) { return m_state; }

private:
	//========== [[[ 関数宣言 ]]]
	void FillScreen(const float& rate);

	//========== [[[ 変数宣言 ]]]
	TYPE   m_type;
	STATE  m_state;
	UShort m_stateCounter;
	UShort m_time;
	Color  m_col;
};