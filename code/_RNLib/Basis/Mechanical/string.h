//========================================
// 
// 文字列クラス
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// 文字列クラス(変数型)
class String {
public:
	//========== [[[ 関数定義 ]]]
	// コンストラクタ
	String() {

		m_string = NULL;
	}
	String(const char* format, ...) {

		if (format == NULL) {
			m_string = NULL;
			return;
		}

		// 可変長引数のためのリスト
		va_list args;
		int size;

		// 可変長引数の処理を始める
		va_start(args, format);

		// 出力する文字列のサイズを取得する
		size = vsnprintf(NULL, 0, format, args);

		// 可変長引数の処理を終了する
		va_end(args);

		// 文字列のサイズに合わせたメモリを動的に確保する
		m_string = (char*)malloc(size + 1);

		// 可変長引数の処理を再度始める
		va_start(args, format);

		// 文字列を書き出す
		vsprintf(m_string, format, args);

		// 可変長引数の処理を終了する
		va_end(args);
	}
	// デストラクタ
	~String() {

		if (m_string != NULL)
			free(m_string);
	}
	// char型ポインタを取得
	char*& GetCharPointer(void) { return m_string; }
	// 変換
	operator char* () const {
		return m_string;
	}
	// 代入
	char*& operator=(const String& other) {

		if (m_string != NULL)
			free(m_string);

		// メンバ変数などをコピーする処理を行う
		if (this != &other) {

			// 文字列の長さに応じてメモリを確保する
			int nStrLen = strlen(other.m_string);
			m_string = (char*)malloc(sizeof(char) * (nStrLen + 1));

			// 文字列をコピーする
			strcpy(m_string, other.m_string);
		}
		return m_string;
	}

private:
	//========== [[[ 変数宣言 ]]]
	char* m_string;
};