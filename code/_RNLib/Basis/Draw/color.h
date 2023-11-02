//========================================
// 
// 色の処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ color.h ]]]
//========================================
#ifndef _COLOR_H_
#define _COLOR_H_

//****************************************
// マクロ定義
//****************************************
#define COLOR_RED       Color{255,0  ,0  ,255}
#define COLOR_GREEN     Color{0  ,255,0  ,255}
#define COLOR_BLUE      Color{0  ,0  ,255,255}
#define COLOR_YELLOW    Color{255,255,0  ,255}
#define COLOR_MAGENTA   Color{255,0  ,255,255}
#define COLOR_CYAN      Color{0  ,255,255,255}
#define COLOR_WHITE     Color{255,255,255,255}
#define COLOR_WHITE_TRA Color{255,255,255,127}
#define COLOR_BLACK     Color{0  ,0  ,0  ,255}

//****************************************
// 構造体定義
//****************************************
// 色管理情報構造体
struct ColorInfo {
	// 背景色
	Color bgCol = INITBGCOLOR;
};

//****************************************
// プロトタイプ宣言
//****************************************
//========== [[[ 取得 ]]]
// 色管理情報の取得
ColorInfo *GetColorInfo(void);
//========== [[[ 設定 ]]]
// 背景色を設定
void SetBGColor(Color bgCol);
//========== [[[ 入出力 ]]]
// 色の読み込み処理
void LoadColor(FILE *pFile, Color *pCol);
//========== [[[ 変換 ]]]
// Color形式の色をD3DXCOLOR形式に変換
D3DXCOLOR ColorToD3DXCOLOR(Color col);
// Color形式の色をD3DCOLOR形式に変換
D3DCOLOR ColorToD3DCOLOR(Color col);
// D3DCOLOR形式の色をColor形式に変換
Color D3DCOLORToColor(D3DCOLORVALUE col);
//========== [[[ その他 ]]]
// 色に明るさを反映
Color BrightnessToColor(Color col, float fBrightness);
// Color形式の色の値を割合で返す
Color ColorRate(Color col, float fRate);
// Color形式の色を加算
Color ColorAdd(Color col1, Color col2);
// Color形式の色を混ぜる
Color ColorMix(Color col1, Color col2);

#endif