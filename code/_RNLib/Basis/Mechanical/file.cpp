//========================================
// 
// ファイルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ファイルクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CFile::CFile() {

	m_files   = NULL;
	m_fileNum = 0;
	strcpy(m_searchString, "");
}

//========================================
// デストラクタ
//========================================
CFile::~CFile() {

}

//========================================
// 初期化処理
//========================================
void CFile::Init(void) {

}

//========================================
// 終了処理
//========================================
void CFile::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CFile::Update(void) {

}

//========================================
// 選択した開くファイル名取得
//========================================
bool CFile::GetSelectOpenFileName(char** resultFileName, const char* initDir, const char* fileType) {

	TCHAR        fileName[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle      = "開く";				// ウィンドウのタイトル指定
	ofn.lStructSize     = sizeof(ofn);			// 構造体のサイズ指定
	ofn.lpstrFile       = fileName;				// 選択されたファイル名が格納される配列指定
	ofn.lpstrFile[0]    = '\0';					// ファイル名初期化
	ofn.nMaxFile        = sizeof(fileName);		// lpstrFile 配列の最大サイズをファイル名格納用文字列のサイズで指定
	ofn.lpstrFilter     = fileType;				// 選択できるファイルの種類を指定
	ofn.nFilterIndex    = 1;					// 初期選択されているフィルタのインデックスを指定
	ofn.lpstrFileTitle  = NULL;					// 選択されたファイルのタイトルを指定(使用されない場合はNULL)
	ofn.nMaxFileTitle   = 0;					// lpstrFileTitle の最大サイズ(使用されない場合は0)
	ofn.lpstrInitialDir = initDir;				// 初期表示するディレクトリ(使用されない場合はNULL)
	ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	// ↑ダイアログのオプションを指定
	//   OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST は、選択されたパスが存在することと、選択されたファイルが存在することを確認するもの
	//   OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの

	// 開いたファイル名を取得出来た時、
	if (GetOpenFileName(&ofn)) {

		// ファイル名が無い時、失敗
		if (!strcmp(fileName, ""))
			return false;

		// ファイル名を適用
		StrCpyDynamicMemory(resultFileName, fileName);

		// 入力をクリア
		RNLib::Input().ClearInputInfo();
	}
	else {
		// 入力をクリア
		RNLib::Input().ClearInputInfo();

		return false;
	}

	return true;
}

//========================================
// 選択した保存するファイル名取得
//========================================
bool CFile::GetSelectSaveFileName(char** resultFileName, const char* initDir, const char* fileType) {

	TCHAR        fileName[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle      = "名前を付けて保存";	// ウィンドウのタイトル指定
	ofn.lStructSize     = sizeof(ofn);			// 構造体のサイズ指定
	ofn.lpstrFile       = fileName;				// 選択されたファイル名が格納される配列指定
	ofn.lpstrFile[0]    = '\0';					// ファイル名初期化
	ofn.nMaxFile        = sizeof(fileName);		// lpstrFile 配列の最大サイズをファイル名格納用文字列のサイズで指定
	ofn.lpstrFilter     = fileType;				// 選択できるファイルの拡張子を指定
	ofn.nFilterIndex    = 1;					// 初期選択されているフィルタのインデックスを指定
	ofn.lpstrFileTitle  = NULL;					// 選択されたファイルのタイトルを指定(使用されない場合はNULL)
	ofn.nMaxFileTitle   = 0;					// lpstrFileTitle の最大サイズ(使用されない場合は0)
	ofn.lpstrInitialDir = initDir;				// 初期表示するディレクトリ(使用されない場合はNULL)
	ofn.Flags           = OFN_NOCHANGEDIR;
	// ↑ダイアログのオプションを指定
	//   OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの

	if (GetSaveFileName(&ofn))
	{// 開いたファイル名を取得出来た時、
		// ファイル名のポインタに読み込んだファイル名を代入するが、
		// 文末が拡張子で終わっていない時、それに加えて拡張子を連結させる
		const int len    = strlen(fileName);
		const int extlen = strlen(fileType);
		if (len < extlen || strcmp(fileName + len - extlen, fileType) != 0) {

			// ファイル名を適用
			StrCpyDynamicMemory(resultFileName, CreateText("%s%s", fileName, fileType));
		}
		else {
			StrCpyDynamicMemory(resultFileName, fileName);
		}

		// 入力をクリア
		RNLib::Input().ClearInputInfo();
	}
	else {
		// 入力をクリア
		RNLib::Input().ClearInputInfo();

		return false;
	}

	return true;
}

//========================================
// "data"を起点としたパス文字列を生成する
//========================================
void CFile::ConvPathToDataStartPath(char** path) {

	// パスがdataディレクトリを含まない場合はエラー
	if (strstr(*path, "\\Data\\") == NULL) {
		return;
	}

	// dataディレクトリの直後の文字列をコピーする
	static char dataStartPath[TXT_MAX] = "";
	const char *start = strstr(*path, "Data\\");
	strncpy(dataStartPath, start, TXT_MAX);
	dataStartPath[TXT_MAX - 1] = '\0'; // NULL終端を付加

	StrCpyDynamicMemory(path, dataStartPath);
}

//========================================
// データファイルを開く(読み込み)
//========================================
bool CFile::OpenLoadFile(const char* path, const char* typeName) {

	// ファイルを再確保(増やす)
	const int fileNumOld = m_fileNum++;
	CMemory::ReAlloc(&m_files, fileNumOld, m_fileNum);

	// ファイルを開く
	m_files[fileNumOld] = fopen(path, "r");
	if (m_files[fileNumOld] == NULL) {

		// エラーメッセージ
		RNLib::Window().Message_ERROR(CreateText("ファイルを開けませんでした。\n%s", path));

		// ファイルを閉じる
		CloseFile();

		return false;
	}

	// 種類名が指定されている時、
	if (typeName != NULL) {

		// 1行読み込み
		char* lineString = NULL;
		RNLib::File().Scan(CFile::SCAN::STRING_DYNAMIC, &lineString);

		// 種類名と一致していない時、
		if (strcmp(lineString, typeName)) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("ファイルの種類が異なります。\n%s", path));

			// ファイルを閉じる
			CloseFile();

			// 読み込んだ文字列を解放
			CMemory::Release(&lineString);

			return false;
		}

		// 読み込んだ文字列を解放
		CMemory::Release(&lineString);
	}


	return true;
}

//========================================
// データファイルを開く(書き出し)
//========================================
bool CFile::OpenSaveFile(const char* path) {

	int fileNumOld = m_fileNum++;
	CMemory::ReAlloc<FILE*>(&m_files, fileNumOld, m_fileNum);
	m_files[fileNumOld] = fopen(path, "w");
	if (m_files[fileNumOld] == NULL) {
		CMemory::ReAlloc<FILE*>(&m_files, m_fileNum, fileNumOld);

		// エラーメッセージ
		RNLib::Window().Message_ERROR(CreateText("ファイルを開けませんでした。\n%s", path));

		// ファイルを閉じる
		CloseFile();

		return false;
	}

	return true;
}

//========================================
// データファイルを閉じる
//========================================
void CFile::CloseFile(void) {

	if (m_fileNum <= 0)
		return;

	if (m_files[m_fileNum-1] != NULL) {
		fclose(m_files[m_fileNum-1]);
		m_fileNum--;
		CMemory::ReAlloc(&m_files, m_fileNum + 1, m_fileNum);
	}
}

//========================================
// 検索ループ
//========================================
bool CFile::SearchLoop(const char* endIdentifier) {

	if (m_files == NULL)
		return false;

	do{
		strcpy(m_searchString, "");
		fscanf(m_files[m_fileNum - 1], "%s", m_searchString);
	} while (m_searchString[0] == '#');

	return (strcmp(m_searchString, endIdentifier) != 0);
}