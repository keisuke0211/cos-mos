//========================================
// 
// �t�@�C���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �t�@�C���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CFile::CFile() {

	files   = NULL;
	fileNum = 0;
	strcpy(searchString, "");
}

//========================================
// �f�X�g���N�^
//========================================
CFile::~CFile() {

}

//========================================
// ����������
//========================================
void CFile::Init(void) {

}

//========================================
// �I������
//========================================
void CFile::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CFile::Update(void) {

}

//========================================
// �I�������J���t�@�C�����擾
//========================================
bool CFile::GetSelectOpenFileName(char** resultFileName, const char* initDir, const char* fileType) {

	TCHAR        fileName[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle      = "�J��";				// �E�B���h�E�̃^�C�g���w��
	ofn.lStructSize     = sizeof(ofn);			// �\���̂̃T�C�Y�w��
	ofn.lpstrFile       = fileName;				// �I�����ꂽ�t�@�C�������i�[�����z��w��
	ofn.lpstrFile[0]    = '\0';					// �t�@�C����������
	ofn.nMaxFile        = sizeof(fileName);		// lpstrFile �z��̍ő�T�C�Y���t�@�C�����i�[�p������̃T�C�Y�Ŏw��
	ofn.lpstrFilter     = fileType;				// �I���ł���t�@�C���̎�ނ��w��
	ofn.nFilterIndex    = 1;					// �����I������Ă���t�B���^�̃C���f�b�N�X���w��
	ofn.lpstrFileTitle  = NULL;					// �I�����ꂽ�t�@�C���̃^�C�g�����w��(�g�p����Ȃ��ꍇ��NULL)
	ofn.nMaxFileTitle   = 0;					// lpstrFileTitle �̍ő�T�C�Y(�g�p����Ȃ��ꍇ��0)
	ofn.lpstrInitialDir = initDir;				// �����\������f�B���N�g��(�g�p����Ȃ��ꍇ��NULL)
	ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	// ���_�C�A���O�̃I�v�V�������w��
	//   OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST �́A�I�����ꂽ�p�X�����݂��邱�ƂƁA�I�����ꂽ�t�@�C�������݂��邱�Ƃ��m�F�������
	//   OFN_NOCHANGEDIR �́A�f�B���N�g����ύX���Ȃ��悤�ɂ������

	// �J�����t�@�C�������擾�o�������A
	if (GetOpenFileName(&ofn)) {

		// �t�@�C�������������A���s
		if (!strcmp(fileName, ""))
			return false;

		// �t�@�C������K�p
		StrCpyDynamicMemory(resultFileName, fileName);

		// ���͂��N���A
		RNLib::Input().ClearInputInfo();
	}
	else {
		// ���͂��N���A
		RNLib::Input().ClearInputInfo();

		return false;
	}

	return true;
}

//========================================
// �I�������ۑ�����t�@�C�����擾
//========================================
bool CFile::GetSelectSaveFileName(char** resultFileName, const char* initDir, const char* fileType) {

	TCHAR        fileName[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lpstrTitle      = "���O��t���ĕۑ�";	// �E�B���h�E�̃^�C�g���w��
	ofn.lStructSize     = sizeof(ofn);			// �\���̂̃T�C�Y�w��
	ofn.lpstrFile       = fileName;				// �I�����ꂽ�t�@�C�������i�[�����z��w��
	ofn.lpstrFile[0]    = '\0';					// �t�@�C����������
	ofn.nMaxFile        = sizeof(fileName);		// lpstrFile �z��̍ő�T�C�Y���t�@�C�����i�[�p������̃T�C�Y�Ŏw��
	ofn.lpstrFilter     = fileType;				// �I���ł���t�@�C���̊g���q���w��
	ofn.nFilterIndex    = 1;					// �����I������Ă���t�B���^�̃C���f�b�N�X���w��
	ofn.lpstrFileTitle  = NULL;					// �I�����ꂽ�t�@�C���̃^�C�g�����w��(�g�p����Ȃ��ꍇ��NULL)
	ofn.nMaxFileTitle   = 0;					// lpstrFileTitle �̍ő�T�C�Y(�g�p����Ȃ��ꍇ��0)
	ofn.lpstrInitialDir = initDir;				// �����\������f�B���N�g��(�g�p����Ȃ��ꍇ��NULL)
	ofn.Flags           = OFN_NOCHANGEDIR;
	// ���_�C�A���O�̃I�v�V�������w��
	//   OFN_NOCHANGEDIR �́A�f�B���N�g����ύX���Ȃ��悤�ɂ������

	if (GetSaveFileName(&ofn))
	{// �J�����t�@�C�������擾�o�������A
		// �t�@�C�����̃|�C���^�ɓǂݍ��񂾃t�@�C�����������邪�A
		// �������g���q�ŏI����Ă��Ȃ����A����ɉ����Ċg���q��A��������
		const int len    = strlen(fileName);
		const int extlen = strlen(fileType);
		if (len < extlen || strcmp(fileName + len - extlen, fileType) != 0) {

			// �t�@�C������K�p
			StrCpyDynamicMemory(resultFileName, CreateText("%s%s", fileName, fileType));
		}
		else {
			StrCpyDynamicMemory(resultFileName, fileName);
		}

		// ���͂��N���A
		RNLib::Input().ClearInputInfo();
	}
	else {
		// ���͂��N���A
		RNLib::Input().ClearInputInfo();

		return false;
	}

	return true;
}

//========================================
// "data"���N�_�Ƃ����p�X������𐶐�����
//========================================
void CFile::ConvPathToDataStartPath(char** path) {

	// �p�X��data�f�B���N�g�����܂܂Ȃ��ꍇ�̓G���[
	if (strstr(*path, "\\Data\\") == NULL) {
		return;
	}

	// data�f�B���N�g���̒���̕�������R�s�[����
	static char dataStartPath[TXT_MAX] = "";
	const char *start = strstr(*path, "Data\\");
	strncpy(dataStartPath, start, TXT_MAX);
	dataStartPath[TXT_MAX - 1] = '\0'; // NULL�I�[��t��

	StrCpyDynamicMemory(path, dataStartPath);
}

//========================================
// �f�[�^�t�@�C�����J��(�ǂݍ���)
//========================================
bool CFile::OpenLoadFile(const char* path, const char* typeName) {

	// �t�@�C�����Ċm��(���₷)
	const int fileNumOld = fileNum++;
	CMemory::ReAlloc(&files, fileNumOld, fileNum);

	// �t�@�C�����J��
	files[fileNumOld] = fopen(path, "r");
	if (files[fileNumOld] == NULL) {

		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR(CreateText("�t�@�C�����J���܂���ł����B\n%s", path));

		// �t�@�C�������
		CloseFile();

		return false;
	}

	// ��ޖ����w�肳��Ă��鎞�A
	if (typeName != NULL) {

		// 1�s�ǂݍ���
		char* lineString = NULL;
		RNLib::File().Scan(CFile::SCAN::STRING_DYNAMIC, &lineString);

		// ��ޖ��ƈ�v���Ă��Ȃ����A
		if (strcmp(lineString, typeName)) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�t�@�C���̎�ނ��قȂ�܂��B\n%s", path));

			// �t�@�C�������
			CloseFile();

			// �ǂݍ��񂾕���������
			CMemory::Release(&lineString);

			return false;
		}

		// �ǂݍ��񂾕���������
		CMemory::Release(&lineString);
	}


	return true;
}

//========================================
// �f�[�^�t�@�C�����J��(�����o��)
//========================================
bool CFile::OpenSaveFile(const char* path) {

	int fileNumOld = fileNum++;
	CMemory::ReAlloc<FILE*>(&files, fileNumOld, fileNum);
	files[fileNumOld] = fopen(path, "w");
	if (files[fileNumOld] == NULL) {
		CMemory::ReAlloc<FILE*>(&files, fileNum, fileNumOld);

		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR(CreateText("�t�@�C�����J���܂���ł����B\n%s", path));

		// �t�@�C�������
		CloseFile();

		return false;
	}

	return true;
}

//========================================
// �f�[�^�t�@�C�������
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
// �������[�v
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
// �ǂݎ�菈��
//========================================
void CFile::Scan(const SCAN scan, void* data, const char* identifier) {

	// ���ʎq���݂鎞�A���ʎq�ƕs��v�Ȃ�I��
	if (identifier != NULL) {
		if (strcmp(searchString, identifier))
			return;
	}

	// �ǂݎ����s
	ScanExecution(scan, data, false, false);
}

//========================================
// �ǂݎ�菈��(CSV)
//========================================
void CFile::ScanCSV(const SCAN scan, void* data, bool isEnd) {

	// �ǂݎ����s
	ScanExecution(scan, data, true, isEnd);
}

//========================================
// �ǂݎ�菈��(���s����)
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