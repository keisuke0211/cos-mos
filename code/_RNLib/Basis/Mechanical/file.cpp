//========================================
// 
// �t�@�C���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �t�@�C���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CFile::CFile() {

	m_files   = NULL;
	m_fileNum = 0;
	strcpy(m_searchString, "");
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
	const int fileNumOld = m_fileNum++;
	CMemory::ReAlloc(&m_files, fileNumOld, m_fileNum);

	// �t�@�C�����J��
	m_files[fileNumOld] = fopen(path, "r");
	if (m_files[fileNumOld] == NULL) {

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

	int fileNumOld = m_fileNum++;
	CMemory::ReAlloc<FILE*>(&m_files, fileNumOld, m_fileNum);
	m_files[fileNumOld] = fopen(path, "w");
	if (m_files[fileNumOld] == NULL) {
		CMemory::ReAlloc<FILE*>(&m_files, m_fileNum, fileNumOld);

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

	if (m_fileNum <= 0)
		return;

	if (m_files[m_fileNum-1] != NULL) {
		fclose(m_files[m_fileNum-1]);
		m_fileNum--;
		CMemory::ReAlloc(&m_files, m_fileNum + 1, m_fileNum);
	}
}

//========================================
// �������[�v
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