//========================================
// 
// オプション処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]オプションクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Options::_RNC_Options() {

	m_optionsDataPath         = NULL;
	m_screenModeChangeCounter = 0;
	m_isFullScreen            = false;
	for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; m_categoryVolumes[cnt] = 1.0f, cnt++);
}

//========================================
// デストラクタ
//========================================
_RNC_Options::~_RNC_Options() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Options::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Options::Uninit(void) {

	// 書き込み処理
	Save();
}

//========================================
// 更新処理
//========================================
void _RNC_Options::Update(void)
{
	if (m_screenModeChangeCounter > 0) {
		if (--m_screenModeChangeCounter == 0) {
			RNLib::Window().SetIsFullScreen(m_isFullScreen);
		}
	}

	if (RNLib::Input().GetKeyTrigger(DIK_F11))
		SetIsFullScreen(!m_isFullScreen);
}

//========================================
// 読み込み処理
//========================================
void _RNC_Options::Load(void) {

	// 読み込むファイルとして開けた時、
	if (RNLib::File().OpenLoadFile(m_optionsDataPath, "OptionsData")) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(_RNC_File::SCAN::BOOL, &m_isFullScreen, "isFullScreen");
			if (RNLib::File().CheckIdentifier("categoryVolumes")) {
				for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; cnt++)
					RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &m_categoryVolumes[cnt]);
			}
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// 書き込み処理
//========================================
void _RNC_Options::Save(void) {

	// 書き込むファイルとして開く
	if (RNLib::File().OpenSaveFile(m_optionsDataPath)) {
		fprintf(RNLib::File().GetFile(), "OptionsData\n");
		fprintf(RNLib::File().GetFile(), "isFullScreen %d\n", m_isFullScreen);
		fprintf(RNLib::File().GetFile(), "categoryVolumes");
		for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; cnt++)
			fprintf(RNLib::File().GetFile(), " %f", m_categoryVolumes[cnt]);
		fprintf(RNLib::File().GetFile(), "\n");
		fprintf(RNLib::File().GetFile(), "END");

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// フルスクリーン設定処理
//========================================
void _RNC_Options::SetIsFullScreen(const bool& isFullScreen) {

	if (m_isFullScreen != isFullScreen) {
		m_isFullScreen = isFullScreen;
		m_screenModeChangeCounter = SCREEN_MODE_CHANGE_TIME;
	}
}