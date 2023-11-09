//========================================
// 
// 設定の処理のヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ setting.h ]]]
//========================================
#ifndef _SETTING_H_
#define _SETTING_H_

//****************************************
// 構造体定義
//****************************************
// 設定の情報構造体
typedef struct
{
	int nBGMVolume;		// BGM音量
	int nSEVolume;		// SE音量
	bool bVibration;	// 振動フラグ
	bool bFullScreen;	// 全画面フラグ
}Setting;

//****************************************
// プロトタイプ宣言
//****************************************
//========== [[[ 基本接続 ]]]
// 設定の更新処理
void UpdateSetting(void);
// 設定メニューの更新処理
void UpdateSettingMenu(void);
//========== [[[ 入出力 ]]]
// 設定の読み込み処理
void LoadSetting(void);
// 設定の書き出し処理
void SaveSetting(void);
//========== [[[ 取得 ]]]
// 設定の情報を取得
Setting *GetSetting(void);
// 設定中フラグを取得
bool *GetSettingNow(void);
//========== [[[ 設定 ]]]
// 設定メニューの設定処理
void SetSettingMenu(void);

#endif