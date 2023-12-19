//========================================
// 
// RNライブラリの関数定義
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"

//****************************************
// 関数定義
//****************************************
bool  RN_GetIsEq        (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsEq    (numA, numB, allowableError); }
bool  RN_GetIsNoEq      (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsNoEq  (numA, numB, allowableError); }
bool  RN_GetIsGtOrEq    (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsGtOrEq(numA, numB, allowableError); }
bool  RN_GetIsGt        (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsGt    (numA, numB, allowableError); }
bool  RN_GetIsLsOrEq    (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsLsOrEq(numA, numB, allowableError); }
bool  RN_GetIsLs        (const float& numA, const float& numB, const float& allowableError = 0.000001f) { return RNLib::Number().GetIsLs    (numA, numB, allowableError); }
short RN_LoadTexture (const char* loadPath) { return RNLib::Texture ().Load(loadPath); }
short RN_LoadModel   (const char* loadPath) { return RNLib::Model   ().Load(loadPath); }
short RN_LoadSetUp3D (const char* loadPath) { return RNLib::SetUp3D ().Load(loadPath); }
short RN_LoadMotion3D(const char* loadPath) { return RNLib::Motion3D().Load(loadPath); }