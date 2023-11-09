//========================================
// 
// デモの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// 前方宣言
//****************************************
class CDemoPlayer;

//****************************************
// クラス定義
//****************************************
// デモクラス
class CDemo {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class PLAYER { RED, BLUE, MAX, };

	//========== [[[ 関数宣言 ]]]
	CDemo();
	~CDemo();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CObjectMgr& GetDemoObjMgr(void) { return m_demoObjMgr; }

private:
	//========== [[[ 変数宣言 ]]]
	CDemoPlayer* m_players[(int)PLAYER::MAX];
	UShort       m_controlPlayerCount;
	CObjectMgr   m_demoObjMgr;
};

// RNデモ
namespace RNDemo {
	CDemo& Get(void);
}