//========================================
// 
// 文字列の処理
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ print.cpp ]]]
//========================================
#include "../../RNlib.h"

//****************************************
// 静的変数宣言
//****************************************
static char* pBuffer = NULL;
static char* pBuffer2 = NULL;

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 引数に入力された文字列のポインタを返す
// Author:RIKU NISHIMURA
//========================================
const char *CreateText(const char* pFormat, ...) {
	va_list args;  // 可変長引数のためのリスト
	int size;
	
	// メモリが使用されていたら解放する
	if (pBuffer != NULL) {
		free(pBuffer);
	}

	va_start(args, pFormat);  // 可変長引数の処理を始める

	 // 出力する文字列のサイズを取得する
	size = vsnprintf(NULL, 0, pFormat, args);
	va_end(args);  // 可変長引数の処理を終了する

	// 文字列のサイズに合わせたメモリを動的に確保する
	pBuffer = (char*)malloc(size + 1);

	va_start(args, pFormat);  // 可変長引数の処理を再度始める
	vsprintf(pBuffer, pFormat, args);  // バッファに文字列を書き出す
	va_end(args);  // 可変長引数の処理を終了する

	return pBuffer;  // 文字列のポインタを返す
}

// http://www.lab.its55.com/?p=32
const char* ConvSJistoUtf8(char* pSource)
{
	//ShiftJISからUTF-16へ変換
	const int nSize = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)
		pSource, -1, NULL, 0);

	char* buffUtf16 = new char[nSize * 2 + 2];
	::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pSource, -1, (LPWSTR)
		buffUtf16, nSize);

	//UTF-16からShift-JISへ変換
	const int nSizeUtf8 = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)
		buffUtf16, -1, NULL, 0, NULL, NULL);

	char* buffUtf8 = new char[nSizeUtf8 * 2];
	ZeroMemory(buffUtf8, nSizeUtf8 * 2);
	::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)buffUtf16, -1, (LPSTR)
		buffUtf8, nSizeUtf8, NULL, NULL);

	// メモリが使用されていたら解放する
	if (pBuffer2 != NULL) {
		free(pBuffer2);
	}

	pBuffer2 = (char*)malloc(nSizeUtf8);
	memcpy(pBuffer2, buffUtf8, nSizeUtf8);

	delete buffUtf16;
	delete buffUtf8;

	return pBuffer2;
}

//========================================
// 文字列のポインタを動的に確保し、文字列を保存する
// Author:RIKU NISHIMURA
//========================================
void StrCpyDynamicMemory(char** ppDestination, const char* pSource) {
	if (pSource == NULL)
		return;

	// 文字列の長さに応じてメモリを確保する
	int nStrLen = strlen(pSource);
	if (*ppDestination != NULL) {
		delete* ppDestination;
	}
	*ppDestination = new char[nStrLen + 1];

	// 文字列をコピーする
	strcpy(*ppDestination, pSource);
}

//========================================
// 文字列の長さを取得
// Author:RIKU NISHIMURA
//========================================
int StrLen(char* pString) {
	// char型の文字列をwchar_t型の文字列に変換
	size_t length = strlen(pString);
	wchar_t* wstr = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, pString, length + 1);
	int nStrLen = (int)wcslen(wstr);
	free(wstr);
	return nStrLen;
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void UninitPrint(void) {
	// メモリが使用されていたら解放する
	if (pBuffer != NULL) {
		free(pBuffer);
	}
	if (pBuffer2 != NULL) {
		free(pBuffer2);
	}
}