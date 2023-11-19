//========================================
// 
// ファイルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ファイルクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CFile::CFile() {

	files   = NULL;
	fileNum = 0;
	strcpy(searchString, "");
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
	const int fileNumOld = fileNum++;
	CMemory::ReAlloc(&files, fileNumOld, fileNum);

	// ファイルを開く
	files[fileNumOld] = fopen(path, "r");
	if (files[fileNumOld] == NULL) {

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

	int fileNumOld = fileNum++;
	CMemory::ReAlloc<FILE*>(&files, fileNumOld, fileNum);
	files[fileNumOld] = fopen(path, "w");
	if (files[fileNumOld] == NULL) {
		CMemory::ReAlloc<FILE*>(&files, fileNum, fileNumOld);

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

	if (fileNum <= 0)
		return;

	if (files[fileNum-1] != NULL) {
		fclose(files[fileNum-1]);
		fileNum--;
		CMemory::ReAlloc(&files, fileNum + 1, fileNum);
	}
}

//========================================
// 検索ループ
//========================================
bool CFile::SearchLoop(const char* endIdentifier) {

	if (files == NULL)
		return false;

	do{
		strcpy(searchString, "");
		fscanf(files[fileNum - 1], "%s", searchString);
	} while (searchString[0] == '#');

	return (strcmp(searchString, endIdentifier) != 0);
}

//========================================
// 読み取り処理
//========================================
void CFile::Scan(const SCAN scan, void* data, const char* identifier) {

	// 識別子が在る時、識別子と不一致なら終了
	if (identifier != NULL) {
		if (strcmp(searchString, identifier))
			return;
	}

	// 読み取り実行
	ScanExecution(scan, data, false, false);
}

//========================================
// 読み取り処理(CSV)
//========================================
void CFile::ScanCSV(const SCAN scan, void* data, bool isEnd) {

	// 読み取り実行
	ScanExecution(scan, data, true, isEnd);
}

//========================================
// 読み取り処理(実行部分)
//========================================
void CFile::ScanExecution(const SCAN scan, void* data, bool isCSV, bool isEnd) {

	searchString;

	switch (scan) {
	case SCAN::INT: {
		int* castData = (int*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%d ", castData) :
			fscanf(files[fileNum - 1], "%d,", castData);
	}break;
	case SCAN::SHORT: {
		short* castData = (short*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%hd ", castData) :
			fscanf(files[fileNum - 1], "%hd,", castData);
	}break;
	case SCAN::USHORT: {
		UShort* castData = (UShort*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%hu ", castData) :
			fscanf(files[fileNum - 1], "%hu,", castData);
	}break;
	case SCAN::FLOAT: {
		float* castData = (float*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%f ", castData) :
			fscanf(files[fileNum - 1], "%f,", castData);
	}break;
	case SCAN::CAHR: {
		char scanString[TXT_MAX];
		ScanExecution(SCAN::STRING, scanString, isCSV, isEnd);
		char* castData = (char*)data;
		*castData = scanString[0];
	}break;
	case SCAN::STRING: {
		char* castData = (char*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%s ", castData) :
			fscanf(files[fileNum - 1], "%[^,],", castData);
	}break;
	case SCAN::STRING_DYNAMIC: {
		char scanString[TXT_MAX];
		ScanExecution(SCAN::STRING, scanString, isCSV, isEnd);
		StrCpyDynamicMemory((char**)data, scanString);
	}break;
	case SCAN::BOOL: {
		int temp;
		ScanExecution(SCAN::INT, &temp, isCSV, isEnd);
		bool* castData = (bool*)data;
		*castData = (temp != 0);
	}break;
	case SCAN::POS2D: {
		Pos2D* castData = (Pos2D*)data;
		fscanf(files[fileNum-1], "%f %f", &castData->x, &castData->y);
	}break;
	case SCAN::POS3D: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%f %f %f ", &castData->x, &castData->y, &castData->z) :
			fscanf(files[fileNum - 1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
	}break;
	case SCAN::ROT: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%f %f %f ", &castData->x, &castData->y, &castData->z) :
			fscanf(files[fileNum - 1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
	}break;
	case SCAN::ROT_CORRECT: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum-1], "%f %f %f ", &castData->x, &castData->y, &castData->z):
			fscanf(files[fileNum-1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
		*castData *= D3DX_PI;
	}break;
	case SCAN::COLOR: {
		Color* castData = (Color*)data;
		!isCSV || isEnd ?
			fscanf(files[fileNum - 1], "%hu %hu %hu %hu ", &castData->r, &castData->g, &castData->b, &castData->a) :
			fscanf(files[fileNum - 1], "%hu %hu %hu %hu,", &castData->r, &castData->g, &castData->b, &castData->a);
	}break;
	case SCAN::TEXIDX: {
		char texPath[TXT_MAX];
		ScanExecution(SCAN::STRING, texPath, isCSV, isEnd);
		short* castData = (short*)data;
		*castData = RNLib::Texture().Load(texPath);
	}break;
	case SCAN::MODELIDX: {
		char modelPath[TXT_MAX];
		ScanExecution(SCAN::STRING, modelPath, isCSV, isEnd);
		short* castData = (short*)data;
		*castData = RNLib::Model().Load(modelPath);
	}break;
	case SCAN::MODELSUIDX: {
		char modelSUPath[TXT_MAX];
		ScanExecution(SCAN::STRING, modelSUPath, isCSV, isEnd);
		short* castData = (short*)data;
		*castData = RNLib::SetUp3D().Load(modelSUPath);
	}break;
	case SCAN::MOTION3DIDX: {
		char motion3DPath[TXT_MAX];
		ScanExecution(SCAN::STRING, motion3DPath, isCSV, isEnd);
		short* castData = (short*)data;
		*castData = RNLib::Motion3D().Load(motion3DPath);
	}break;
	}
}