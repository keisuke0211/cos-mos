//========================================
// 
// ファイルの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"
#include "../../RNfunction.h"

//****************************************
// クラス定義
//****************************************
class _RNC_File {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class SCAN { INT, UINT, SHORT, USHORT, FLOAT, CAHR, STRING, STRING_DYNAMIC, BOOL, POS2D, VECTOR3D, POS3D, ROT3D, ROT3D_CORRECT, SCALE3D, COLOR, TEXIDX, MODELIDX, SETUP3DIDX, MOTION3DIDX, };

	//========== [[[ 関数宣言 ]]]
	_RNC_File                    ();
	~_RNC_File                   ();
	void  Init                   (void);
	void  Uninit                 (void);
	void  Update                 (void);
	FILE* GetFile                (void) { return m_files[m_fileNum-1]; }
	char* GetSearchString        (void) { return m_searchString; }
	bool  GetSelectOpenFileName  (char** fileName, const char* initDir, const char* fileType);
	bool  GetSelectSaveFileName  (char** fileName, const char* initDir, const char* fileType);
	void  ConvPathToDataStartPath(char** path);
	bool  OpenLoadFile           (const char* path, const char* typeName = NULL);
	bool  OpenSaveFile           (const char* path);
	void  CloseFile              (void);
	bool  CheckIdentifier        (const char* identifier) { return (strcmp(m_searchString, identifier) == 0); }
	bool  CheckIdentifierPartial (const char* identifier) { return (strncmp(m_searchString, identifier, strlen(identifier)) == 0); }
	void  NewLine                (void)                   { fscanf(m_files[m_fileNum-1], "%s\n", m_searchString); }
	bool  SearchLoop             (const char* endIdentifier);

	//========================================
	// 読み取り処理
	//========================================
	template <class T>void Scan(const SCAN scan, T* data, const char* identifier = NULL) {

		// 識別子と不一致なら終了
		if (identifier != NULL)
			if (strcmp(m_searchString, identifier))
				return;

		// 読み取り実行
		ScanExecution(scan, data, false, false);
	}

	//========================================
	// 読み取り処理(CSV)
	//========================================
	template <class T>void ScanCSV(const SCAN scan, T* data, bool isEnd = false) {

		// 読み取り実行
		ScanExecution(scan, data, true, isEnd);
	}

private:
	//========================================
	// 実行処理
	//========================================
	template <class T>void ScanExecution(const SCAN scan, T* data, bool isCSV, bool isEnd) {

		switch (scan) {
		case SCAN::INT:
			if (typeid(T) == typeid(int)) {
				int& castData = *(int*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%d ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%d,", &castData);
				return;
			}
		break;
		case SCAN::UINT:
			if (typeid(T) == typeid(UInt)) {
				UInt& castData = *(UInt*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%u ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%u,", &castData);
				return;
			}
			break;
		case SCAN::SHORT:
			if (typeid(T) == typeid(short)) {
				short& castData = *(short*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%hd ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%hd,", &castData);
				return;
			}
		break;
		case SCAN::USHORT:
			if (typeid(T) == typeid(UShort)) {
				UShort& castData = *(UShort*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%hu ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%hu,", &castData);
				return;
			}
		break;
		case SCAN::FLOAT:
			if (typeid(T) == typeid(float)) {
				float& castData = *(float*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%f ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%f,", &castData);
				return;
			}
		break;
		case SCAN::CAHR:
			if (typeid(T) == typeid(char)) {
				char& castData = *(char*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%c ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%c,", &castData);
				return;
			}
		break;
		case SCAN::STRING:
			if (typeid(T) == typeid(char)) {
				char& castData = *(char*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%s ", &castData) :
					fscanf(m_files[m_fileNum - 1], "%[^,],", &castData);
				return;
			}
		break;
		case SCAN::STRING_DYNAMIC:
			if (typeid(T) == typeid(char*)) {
				char scanString[TXT_MAX];
				ScanExecution(SCAN::STRING, scanString, isCSV, isEnd);

				char*& castData = *(char**)data;
				StrCpyDynamicMemory(&castData, scanString);
				return;
			}
		break;
		case SCAN::BOOL:
			if (typeid(T) == typeid(bool)) {
				int scanInt;
				ScanExecution(SCAN::INT, &scanInt, isCSV, isEnd);

				bool& castData = *(bool*)data;
				castData = (scanInt != 0);
				return;
			}
		break;
		case SCAN::POS2D:
			if (typeid(T) == typeid(Pos2D)) {
				Pos2D& castData = *(Pos2D*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%f %f", &castData.x, &castData.y) :
					fscanf(m_files[m_fileNum - 1], "%f %f,", &castData.x, &castData.y);
				return;
			}
		break;
		case SCAN::VECTOR3D:
		case SCAN::POS3D:
		case SCAN::ROT3D:
		case SCAN::ROT3D_CORRECT:
		case SCAN::SCALE3D:
			if (typeid(T) == typeid(Vector3D)) {
				Vector3D& castData = *(Vector3D*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%f %f %f", &castData.x, &castData.y, &castData.z) :
					fscanf(m_files[m_fileNum - 1], "%f %f %f,", &castData.x, &castData.y, &castData.z);

				if(scan == SCAN::ROT3D_CORRECT)
					castData *= D3DX_PI;
				return;
			}
		break;
		case SCAN::COLOR:
			if (typeid(T) == typeid(Color)) {
				Color& castData = *(Color*)data;
				!isCSV || isEnd ?
					fscanf(m_files[m_fileNum - 1], "%hu %hu %hu %hu", &castData.r, &castData.g, &castData.b, &castData.a) :
					fscanf(m_files[m_fileNum - 1], "%hu %hu %hu %hu,", &castData.r, &castData.g, &castData.b, &castData.a);
				return;
			}
		break;
		case SCAN::TEXIDX: 
		case SCAN::MODELIDX:
		case SCAN::SETUP3DIDX:
		case SCAN::MOTION3DIDX:
			if (typeid(T) == typeid(short)) {
				char loadPath[TXT_MAX];
				ScanExecution(SCAN::STRING, loadPath, isCSV, isEnd);

				short& castData = *(short*)data;
				switch (scan) {
				case SCAN::TEXIDX     :castData = RN_LoadTexture(loadPath); break;
				case SCAN::MODELIDX   :castData = RN_LoadModel  (loadPath); break;
				case SCAN::SETUP3DIDX :castData = RN_LoadSetUp3D(loadPath); break;
				case SCAN::MOTION3DIDX:castData = RN_LoadModel  (loadPath); break;
				}
				return;
			}
		break;
		}

		assert(false);	// ※型が正しくない
	}

	//========== [[[ 変数宣言 ]]]
	FILE** m_files;
	UShort m_fileNum;
	char   m_searchString[TXT_MAX];
};