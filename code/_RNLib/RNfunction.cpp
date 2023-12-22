//========================================
// 
// RNライブラリの関数定義
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "RNlib.h"

//****************************************
// 関数定義
//****************************************
short RN_LoadTexture (const char* loadPath) { return RNLib::Texture ().Load(loadPath); }
short RN_LoadModel   (const char* loadPath) { return RNLib::Model   ().Load(loadPath); }
short RN_LoadSetUp3D (const char* loadPath) { return RNLib::SetUp3D ().Load(loadPath); }
short RN_LoadMotion3D(const char* loadPath) { return RNLib::Motion3D().Load(loadPath); }