//========================================
// 
// RNエージェント(仲介者)の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "RNlib.h"

//****************************************
// クラス定義
//****************************************
// RNエージェントクラス
class _RNC_Agent {
public:
	static void Init     (HINSTANCE& instanceHandle, const char*& settingsPath, const char*& optionsDataPath, const UShort& priorityMax, const RNSystem::MODE& mode);
	static void Uninit   (void);
	static void EndUninit(void);
	static void Update   (void);
	static void Draw     (void);
	static void EndDraw  (void);
};