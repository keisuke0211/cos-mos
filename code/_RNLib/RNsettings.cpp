//========================================
// 
// RNライブラリの設定管理処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNsettings.h"

//****************************************
// 無名空間
//****************************************
namespace {
	
	//========== [[[ 変数宣言 ]]]
	RNSettings::Info info = {};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNSettings
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 解放処理
//========================================
void RNSettings::Release(void) {

	RNLib::Memory().Release(&info.projectName);
	RNLib::Memory().Release(&info.fontListPath);
}

//========================================
// 読み込み&書き込み処理
//========================================
bool RNSettings::LoadAndSave(const char* path) {

	// 読み込むファイルとして開けた時、
	bool isLoad = false;
	String loadPath("");
	switch (RNSystem::GetMode()) {
	case RNSystem::MODE::SETUP3D_EDITOR:loadPath = "RNData\\SetUp3DEditorSettings.txt"; break;
	case RNSystem::MODE::LIGHT3D_EDITOR:loadPath = "RNData\\Light3DEditorSettings.txt"; break;
	default:loadPath = path; break;
	}
	isLoad = RNLib::File().OpenLoadFile(loadPath);
	if (isLoad) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(_RNC_File::SCAN::STRING_DYNAMIC, &info.projectName                     , "projectName"                     );
			RNLib::File().Scan(_RNC_File::SCAN::FLOAT         , &info.windowWidth                     , "windowWidth"                     );
			RNLib::File().Scan(_RNC_File::SCAN::FLOAT         , &info.windowHeight                    , "windowHeight"                    );
			RNLib::File().Scan(_RNC_File::SCAN::BOOL          , &info.isFullScreen                    , "isFullScreen"                    );
			RNLib::File().Scan(_RNC_File::SCAN::FLOAT         , &info.resolution                      , "resolution"                      );
			RNLib::File().Scan(_RNC_File::SCAN::STRING_DYNAMIC, &info.fontListPath                    , "fontListPath"                    );
			RNLib::File().Scan(_RNC_File::SCAN::FLOAT         , &info.modelOutLineAddDistanceInterval , "modelOutLineAddDistanceInterval" );
			RNLib::File().Scan(_RNC_File::SCAN::USHORT        , &info.modelOutLineAddDistanceDelimiter, "modelOutLineAddDistanceDelimiter");
		}

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
	else {
		// エラーメッセージ
		RNLib::Window().Message_ERROR(String("RNライブラリの設定ファイルを開けませんでした。\n"));
	}

	// 書き込むファイルとして開く
	//if (RNLib::File().OpenSaveFile(path)) {
	//	fprintf(RNLib::File().GetFile(), /*  */"#KeyOption\n");	
	//	fprintf(RNLib::File().GetFile(), /*  */"projectName							%s\n"  , info.projectName);
	//	fprintf(RNLib::File().GetFile(), /*  */"windowWidth							%.2f\n", info.windowWidth);
	//	fprintf(RNLib::File().GetFile(), /*  */"windowHeight						%.2f\n", info.windowHeight);
	//	fprintf(RNLib::File().GetFile(), /*  */"isFullScreen						%d\n"  , info.isFullScreen);
	//	fprintf(RNLib::File().GetFile(), /*  */"resolution							%.2f\n", info.resolution);
	// 	fprintf(RNLib::File().GetFile(), /*  */"#OtherOption\n");
	//	fprintf(RNLib::File().GetFile(), /*  */"modelOutLineAddDistanceInterval		%.2f\n", info.modelOutLineAddDistanceInterval);
	//	fprintf(RNLib::File().GetFile(), /*  */"modelOutLineAddDistanceDelimiter	%d\n"  , info.modelOutLineAddDistanceDelimiter);
	//	fprintf(RNLib::File().GetFile(), /*  */"END");

	//	// ファイルを閉じる
	//	RNLib::File().CloseFile();
	//}

	return isLoad;
}

//========================================
// 情報取得処理
//========================================
RNSettings::Info RNSettings::GetInfo(void) {

	return info;
}