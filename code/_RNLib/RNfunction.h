//========================================
// 
// RN���C�u�����̊֐��錾
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �֐��錾
//****************************************
// float�l������r
bool  RN_GetIsEq    /* == */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
bool  RN_GetIsNoEq  /* != */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
bool  RN_GetIsGtOrEq/* >= */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
bool  RN_GetIsGt    /* >  */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
bool  RN_GetIsLsOrEq/* <= */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
bool  RN_GetIsLs    /* <  */(const float& numA, const float& numB, const float& allowableError = 0.000001f);
// ���\�[�X�̓ǂݍ���
short RN_LoadTexture (const char* loadPath);
short RN_LoadModel   (const char* loadPath);
short RN_LoadSetUp3D (const char* loadPath);
short RN_LoadMotion3D(const char* loadPath);