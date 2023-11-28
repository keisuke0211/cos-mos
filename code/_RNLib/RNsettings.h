//========================================
// 
// RNライブラリの設定管理処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// 名前空間
//****************************************
namespace RNSettings {

	//========== [[[ 構造体定義 ]]]
	struct Info {
		// KeyOptions
		char* projectName             = NULL;
		float windowWidth             = 0.0f;
		float windowHeight            = 0.0f;
		bool  isFullScreen            = false;	
		float resolution              = 0.0f;
		// OtherOptions
		float modelOutLineAddDistanceInterval = 0.0f;
		UShort modelOutLineAddDistanceDelimiter = 0;
	};

	//========== [[[ 関数宣言 ]]]
	void Release(void);
	bool LoadAndSave(const char* path);
	Info GetInfo(void);
	void SetFulScreen(bool screen);
}