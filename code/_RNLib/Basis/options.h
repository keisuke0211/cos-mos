//========================================
// 
// オプション処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// オプションクラス
class _RNC_Options {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Options             ();
	~_RNC_Options            ();
	void   Init              (void);
	void   Uninit            (void);
	void   Update            (void);
	void   Load              (void);
	void   Save              (void);
	void   SetOptionsDataPath(const char* optionsDataPath) { m_optionsDataPath = optionsDataPath; }
	void   SetIsFullScreen   (const bool& isFullScreen);
	bool&  GetIsFullScreen   (void) { return m_isFullScreen; }
	void   SetCategoryVolume (const _RNC_Sound::CATEGORY& category, const float& volume) { m_categoryVolumes[(int)category] = volume; }
	float& GetCategoryVolume (const _RNC_Sound::CATEGORY& category)                      { return m_categoryVolumes[(int)category]; }

private:
	//========== [[[ 定数定義 ]]]
	static const int SCREEN_MODE_CHANGE_TIME = 30;

	//========== [[[ 変数宣言 ]]]
	const char* m_optionsDataPath;
	UShort      m_screenModeChangeCounter;
	bool        m_isFullScreen;
	float       m_categoryVolumes[(int)_RNC_Sound::CATEGORY::MAX];
};
