//========================================
// 
// 遷移のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ transition.h ]]]
//========================================
#ifndef _TRANSITION_H_
#define _TRANSITION_H_

//****************************************
// クラス定義
//****************************************
// 遷移クラス
class CTransition {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE, OPEN, OPEN_WAIT, CLOSE, WAIT_SET_MODE,
	};
	enum class TYPE {
		NONE,
		FADE,
	};

	//========== [[[ 関数宣言 ]]]
	CTransition();
	~CTransition();
	void Update(void);
	void Set(STATE state, TYPE type);
	STATE GetState(void) { return m_state; }

private:
	//========== [[[ 変数宣言 ]]]
	STATE m_state;
	int   m_nStateCtr;
	TYPE  m_type;
};

#endif