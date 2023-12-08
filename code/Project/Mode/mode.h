//========================================
// 
// モードの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
// モードの情報構造体
class CMode {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class TYPE{
		NONE = -1,
		LOGO,
		TITLE,
		GAME,
		RESULT,
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CMode                       ();
	virtual ~CMode              ();
	virtual void   Init         (void);
	virtual void   Uninit       (void);
	virtual void   Update       (void);
	virtual void   ProcessState (const PROCESS process) = 0;
	        void   SetType      (TYPE type) { m_type = type; }
	        TYPE   GetType      (void)      { return m_type; }
	static  CMode* Create       (TYPE type);

protected:
	//========== [[[ 関数宣言 ]]]
	void SetState (const int nState);

	//========== [[[ 変数宣言 ]]]
	int m_state;
	int m_stateCtr;

private:
	//========== [[[ 変数宣言 ]]]
	TYPE m_type;
};