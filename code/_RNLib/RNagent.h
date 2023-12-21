//========================================
// 
// RN�G�[�W�F���g(�����)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "RNlib.h"

//****************************************
// �N���X��`
//****************************************
// RN�G�[�W�F���g�N���X
class _RNC_Agent {
public:
	static void Init     (HINSTANCE& instanceHandle, const char*& settingsPath, const char*& optionsDataPath, const UShort& priorityMax, const RNSystem::MODE& mode);
	static void Uninit   (void);
	static void EndUninit(void);
	static void Update   (void);
	static void Draw     (void);
	static void EndDraw  (void);
};