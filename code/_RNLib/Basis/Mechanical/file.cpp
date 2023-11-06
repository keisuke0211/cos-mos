//========================================
// 
// ファイルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//****************************************
// 定数定義
//****************************************
namespace {
	const char* c_anchorNames[] = {
		"NONE" ,
		"CENTER","TOP"     ,"BOTTOM"     ,
		"LEFT"  ,"LEFTTOP" ,"LEFTBOTTOM" ,
		"RIGHT" ,"RIGHTTOP","RIGHTBOTTOM",
	};
}
CHECK_ARRAY_SIZE(c_anchorNames, (int)ANCHOR::MAX);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CFileクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CFile::CFile() {
	files = NULL;
	fileNum = 0;
	strcpy(searchString, "");
}

//========================================
// デストラクタ
//========================================
CFile::~CFile() {

}

//========================================
// 選択した開くファイル名取得
//========================================
bool CFile::GetSelectOpenFileName(char* resultFileName, const char* initDir, const char* fileType) {

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
	// ダイアログのオプションを指定
	// OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST は、選択されたパスが存在することと、選択されたファイルが存在することを確認するもの
	// OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// 開いたファイル名を取得出来た時、
	if (GetOpenFileName(&ofn)) {
		if (!strcmp(fileName, ""))
			return false;
		sprintf(resultFileName, fileName);
		RNLib::Input().ClearInputInfo();
	}
	else {
		RNLib::Input().ClearInputInfo();
		return false;
	}

	return true;
}

//========================================
// 選択した開くファイル名取得
//========================================
bool CFile::GetSelectSaveFileName(char* resultFileName, const char* initDir, const char* fileType) {

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
	// ダイアログのオプションを指定
	// OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{// 開いたファイル名を取得出来た時、
		// ファイル名のポインタに読み込んだファイル名を代入するが、
		// 文末が拡張子で終わっていない時、それに加えて拡張子を連結させる
		int len = strlen(fileName);
		int extlen = strlen(fileType);
		if (len < extlen || strcmp(fileName + len - extlen, fileType) != 0)
			sprintf(resultFileName, "%s%s", fileName, fileType);
		else
			strcpy(resultFileName, fileName);

		RNLib::Input().ClearInputInfo();
		return true;
	}
	else {
		RNLib::Input().ClearInputInfo();
		return false;
	}
}

//========================================
// "data"を起点としたパス文字列を生成する
//========================================
char* CFile::ConvPathToDataStartPath(const char* path) {

	static char dataStartPath[TXT_MAX] = "";

	// パスがdataディレクトリを含まない場合はエラー
	if (strstr(path, "Data\\") == NULL) {
		RNLib::Window().Message(CreateText("\"%s\" does not contain \"Data\\\" directory.\n", path), "Error");
		return NULL;
	}

	// dataディレクトリの直後の文字列をコピーする
	const char *start = strstr(path, "Data\\");
	strncpy(dataStartPath, start, TXT_MAX);
	dataStartPath[TXT_MAX - 1] = '\0'; // NULL終端を付加

	return dataStartPath;
}

//========================================
// データファイルを開く(読み込み)
//========================================
bool CFile::OpenLoadFile(const char* path) {

	int fileNumOld = fileNum++;
	RNLib::Memory().ReAlloc<FILE*>(&files, fileNumOld, fileNum);
	files[fileNumOld] = fopen(path, "r");
	if (files[fileNumOld] == NULL) {

		// エラーメッセージ
		RNLib::Window().Message_ERROR(CreateText("ファイルを開けませんでした。\n%s", path));

		// ファイルを閉じる
		CloseFile();

		RNLib::Memory().ReAlloc<FILE*>(&files, fileNum, fileNumOld);

		return false;
	}

	return true;
}

//========================================
// データファイルを開く(書き出し)
//========================================
bool CFile::OpenSaveFile(const char* path) {

	int fileNumOld = fileNum++;
	RNLib::Memory().ReAlloc<FILE*>(&files, fileNumOld, fileNum);
	files[fileNumOld] = fopen(path, "w");
	if (files[fileNumOld] == NULL) {
		RNLib::Memory().ReAlloc<FILE*>(&files, fileNum, fileNumOld);

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
		RNLib::Memory().ReAlloc<FILE*>(&files, fileNum + 1, fileNum);
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
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%d ", castData);
		else
			fscanf(files[fileNum-1], "%d,", castData);
	}break;
	case SCAN::FLOAT: {
		float* castData = (float*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%f ", castData);
		else
			fscanf(files[fileNum-1], "%f,", castData);
	}break;
	case SCAN::FLOAT2D: {
		float* castData = (float*)data;
		ScanExecution(SCAN::FLOAT, castData, isCSV, isEnd);
		*castData *= PIXEL2D_SIZE;
	}break;
	case SCAN::CAHR: {
		char scanString[TXT_MAX];
		ScanExecution(SCAN::STRING, scanString, isCSV, isEnd);
		char* castData = (char*)data;
		*castData = scanString[0];
	}break;
	case SCAN::STRING: {
		char* castData = (char*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%s ", castData);
		else
			fscanf(files[fileNum-1], "%[^,],", castData);
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
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		fscanf(files[fileNum-1], "%f %f %f", &castData->x, &castData->y, &castData->z);
		*castData *= PIXEL2D_SIZE;

		// アンカーに応じて位置補正
		char aAnchor[TXT_MAX];
		ScanExecution(SCAN::STRING, aAnchor, isCSV, isEnd);
		int cntAnchor;
		for (cntAnchor = 0; cntAnchor < (int)ANCHOR::MAX; cntAnchor++) {
			if (strcmp(aAnchor, c_anchorNames[cntAnchor]))
				continue;

			switch ((ANCHOR)cntAnchor) {
			case ANCHOR::CENTER: {
				*castData += RNLib::Window().GetCenterPos();
			}break;
			case ANCHOR::TOP: {
				castData->x += RNLib::Window().GetCenterX();
			}break;
			case ANCHOR::BOTTOM: {
				castData->x += RNLib::Window().GetCenterX();
				castData->y += RNLib::Window().GetHeight();
			}break;
			case ANCHOR::LEFT: {
				castData->y += RNLib::Window().GetCenterY();
			}break;
			case ANCHOR::LEFTTOP: {
			}break;
			case ANCHOR::LEFTBOTTOM: {
				castData->y += RNLib::Window().GetHeight();
			}break;
			case ANCHOR::RIGHT: {
				castData->x += RNLib::Window().GetWidth();
				castData->y += RNLib::Window().GetCenterY();
			}break;
			case ANCHOR::RIGHTTOP: {
				castData->x += RNLib::Window().GetWidth();
			}break;
			case ANCHOR::RIGHTBOTTOM: {
				castData->x += RNLib::Window().GetWidth();
				castData->y += RNLib::Window().GetHeight();
			}break;
			}

			break;
		}

		assert(cntAnchor < (int)ANCHOR::MAX);
	}break;
	case SCAN::POS3D: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%f %f %f ", &castData->x, &castData->y, &castData->z);
		else
			fscanf(files[fileNum-1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
	}break;
	case SCAN::ROT: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%f %f %f ", &castData->x, &castData->y, &castData->z);
		else
			fscanf(files[fileNum-1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
	}break;
	case SCAN::ROT_CORRECT: {
		D3DXVECTOR3* castData = (D3DXVECTOR3*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%f %f %f ", &castData->x, &castData->y, &castData->z);
		else
			fscanf(files[fileNum-1], "%f %f %f,", &castData->x, &castData->y, &castData->z);
		*castData *= D3DX_PI;
	}break;
	case SCAN::COLOR: {
		Color* castData = (Color*)data;
		if (!isCSV || isEnd)
			fscanf(files[fileNum-1], "%d %d %d %d ", &castData->r, &castData->g, &castData->b, &castData->a);
		else
			fscanf(files[fileNum-1], "%d %d %d %d,", &castData->r, &castData->g, &castData->b, &castData->a);
	}break;
	case SCAN::TEXIDX: {
		char texPath[TXT_MAX];
		ScanExecution(SCAN::STRING, texPath, isCSV, isEnd);
		int* castData = (int*)data;
		*castData = RNLib::Texture().Load(texPath);
	}break;
	case SCAN::MODELIDX: {
		char modelPath[TXT_MAX];
		ScanExecution(SCAN::STRING, modelPath, isCSV, isEnd);
		int* castData = (int*)data;
		*castData = RNLib::Model().Load(modelPath);
	}break;
	case SCAN::MODELSUIDX: {
		char modelSUPath[TXT_MAX];
		ScanExecution(SCAN::STRING, modelSUPath, isCSV, isEnd);
		int* castData = (int*)data;
		*castData = RNLib::SetUp3D().Load(modelSUPath);
	}break;
	case SCAN::MOTION3DIDX: {
		char motion3DPath[TXT_MAX];
		ScanExecution(SCAN::STRING, motion3DPath, isCSV, isEnd);
		int* castData = (int*)data;
		*castData = RNLib::Motion3D().Load(motion3DPath);
	}break;
	}
}