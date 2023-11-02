//========================================
// 
// 文字列関連のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ print.h ]]]
//========================================
#ifndef _PRINT_H_
#define _PRINT_H_

//****************************************
// 関数宣言
//****************************************
const char *CreateText(const char* format, ...);	// 引数に入力された文字列のポインタを返す
const char* ConvSJistoUtf8(char* pSource);
void StrCpyDynamicMemory(char** ppDestination, const char* pSource);	// 文字列のポインタを動的に確保し、文字列を保存する
int StrLen(char* pString);
void UninitPrint(void);	// 文字列関連

#endif