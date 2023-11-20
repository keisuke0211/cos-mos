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
	enum class PLAYER   { RED, BLUE, MAX, };

	//========== [[[ 関数宣言 ]]]
	CDemo();
	~CDemo();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CDemoPlayer& GetPlayer(const PLAYER& player) { return *m_players[(int)player]; }
	CObjectMgr&  GetDemoObjMgr(void) { return m_demoObjMgr; }

private:
	//========== [[[ 列挙型定義 ]]]
	enum class SCREEN_TYPE { NORMAL, DIVISION, MAX };

	//========== [[[ 定数宣言 ]]]
	static const char* PLAYER_NAMES[(int)PLAYER::MAX];
	static const char* SCREEN_TYPE_NAMES[(int)SCREEN_TYPE::MAX];

	//========== [[[ 変数宣言 ]]]
	CDemoPlayer* m_players[(int)PLAYER::MAX];
	UShort       m_controlPlayerCount;
	UShort       m_screenTypeCount;
	CObjectMgr   m_demoObjMgr;
};

// RNデモ
namespace RNDemo {
	CDemo& Get(void);
}