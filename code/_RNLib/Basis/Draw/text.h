//========================================
// 
// テキストの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// テキストクラス
class CText {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class ALIGNMENT { CENTER, LEFT, RIGHT, MAX, };

	//========== [[[ 構造体定義 ]]]
	// フォントデータ
	struct FontData {
		UShort startCode = 0;
		UShort ptnX      = 0;
		UShort ptnY      = 0;
		short  texIdx    = NONEDATA;	
	};

	//========== [[[ 関数宣言 ]]]
	CText                  ();
	~CText                 ();
	void      Init         (void);
	void      Uninit       (void);
	void      Update       (void);
	void      LoadFont     (const char* loadPath);
	FontData& GetFontData  (const short& fontIdx) { return fontIdx == NONEDATA ? m_defaultFontData : m_fontDatas[fontIdx]; }
	int       GetFontNum   (void) { return m_fontNum; }
	float     GetFontWidth (const short& fontIdx);
	float     GetFontHeight(const short& fontIdx);

private:
	//========== [[[ 変数宣言 ]]]
	FontData  m_defaultFontData;
	FontData* m_fontDatas;
	UShort    m_fontNum;
};