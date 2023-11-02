//========================================
// 
// テキストのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ text.h ]]]
//========================================
#ifndef _TEXT_H_
#define _TEXT_H_

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// テキストクラス
class CText {
public:
	//========== [[[ 列挙型定義 ]]]
	// 表示形式
	enum class ALIGNMENT {
		CENTER,	// 中央揃え
		LEFT,	// 左揃え
		RIGHT,	// 右揃え
		MAX,
	};

	//========== [[[ 構造体定義 ]]]
	// フォント毎の情報構造体
	typedef struct Font_ {
		int   nStartCode = 0;			// 開始文字
		int   nPtnWidth  = 0;			// パターン幅
		int   nPtnHeight = 0;			// パターン高さ
		float fSpaceRate = 0.0f;		// 間隔比率
		int   nTexIdx    = NONEDATA;	// テクスチャ番号
	}FontData;

	//========== [[[ 関数宣言 ]]]
	/* コンストラクタ         */CText();
	/* デストラクタ           */~CText();
	/* 終了処理               */void Uninit(void);
	/* フォントの読み込み処理 */void LoadFont(void);
	/* フォント存在チェック   */bool CheckFontExists(int nFont) {
		if (nFont < 0 || nFont >= m_nFontNum) {
			return false;
		}
		return true;
	}
	/* フォントの情報を取得   */FontData GetFont(const int nFont) {
		if (nFont < 0 || nFont >= m_nFontNum) {
			FontData defFont = {};
			return defFont;
		}
		return m_pFont[nFont];
	};
	/* フォントの数を取得    */int GetFontNum(void) { return m_nFontNum; }
	/* フォントの幅取得      */float GetFontWidth   (int nFont);
	/* フォントの幅取得      */float GetFontWidth2D (int nFont);
	/* フォントの高さ取得    */float GetFontHeight  (int nFont);
	/* フォントの高さ取得    */float GetFontHeight2D(int nFont);

	//========== [[[ 変数宣言 ]]]
	FontData* m_pFont;	// フォント情報のポインタ
	int m_nFontNum;	// フォント数
};

#endif