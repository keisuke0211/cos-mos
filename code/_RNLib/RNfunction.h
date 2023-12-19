//========================================
// 
// RNライブラリの関数宣言
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// 関数宣言
//****************************************
//----- float値等価比較
// 一致  ==
bool  RN_GetIsEq    (const float& numA, const float& numB, const float& allowableError = 0.000001f);
// 不一致  !=
bool  RN_GetIsNoEq  (const float& numA, const float& numB, const float& allowableError = 0.000001f);
// 以上  >=
bool  RN_GetIsGtOrEq(const float& numA, const float& numB, const float& allowableError = 0.000001f);
// 超えた  >
bool  RN_GetIsGt    (const float& numA, const float& numB, const float& allowableError = 0.000001f);
// 以下  <=
bool  RN_GetIsLsOrEq(const float& numA, const float& numB, const float& allowableError = 0.000001f);
// 未満  <
bool  RN_GetIsLs    (const float& numA, const float& numB, const float& allowableError = 0.000001f);

// リソースの読み込み
short RN_LoadTexture (const char* loadPath);
short RN_LoadModel   (const char* loadPath);
short RN_LoadSetUp3D (const char* loadPath);
short RN_LoadMotion3D(const char* loadPath);