//========================================
// 
// RN���C�u�����̐ݒ�Ǘ�����
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNsettings.h"

//****************************************
// �������
//****************************************
namespace {
	
	//========== [[[ �ϐ��錾 ]]]
	RNSettings::Info info = {};
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RNSettings�̊֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �������
//========================================
void RNSettings::Release(void) {

	CMemory::Release(&info.projectName);
}

//========================================
// �ǂݍ���&�������ݏ���
//========================================
bool RNSettings::LoadAndSave(const char* path) {

	// �ǂݍ��ރt�@�C���Ƃ��ĊJ�������A
	bool isLoad = RNLib::File().OpenLoadFile(path);
	if (isLoad) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(CFile::SCAN::STRING_DYNAMIC, &info.projectName                     , "projectName"                     );
			RNLib::File().Scan(CFile::SCAN::FLOAT         , &info.windowWidth                     , "windowWidth"                     );
			RNLib::File().Scan(CFile::SCAN::FLOAT         , &info.windowHeight                    , "windowHeight"                    );
			RNLib::File().Scan(CFile::SCAN::BOOL          , &info.isFullScreen                    , "isFullScreen"                    );
			RNLib::File().Scan(CFile::SCAN::FLOAT         , &info.resolution                      , "resolution"                      );
			RNLib::File().Scan(CFile::SCAN::FLOAT         , &info.modelOutLineAddDistanceInterval , "modelOutLineAddDistanceInterval" );
			RNLib::File().Scan(CFile::SCAN::USHORT        , &info.modelOutLineAddDistanceDelimiter, "modelOutLineAddDistanceDelimiter");
		}

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
	else {
		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR(CreateText("RN���C�u�����̐ݒ�t�@�C�����J���܂���ł����B\n�ȉ��̃p�X�ɐݒ�t�@�C�����쐬����̂ŁA������͂��Ă��������B\n%s", path));
	}

	// �������ރt�@�C���Ƃ��ĊJ��
	if (RNLib::File().OpenSaveFile(path)) {
		fprintf(RNLib::File().GetFile(), /*  */"#KeyOption\n");	
		fprintf(RNLib::File().GetFile(), /*  */"projectName							%s\n"  , info.projectName);
		fprintf(RNLib::File().GetFile(), /*  */"windowWidth							%.2f\n", info.windowWidth);
		fprintf(RNLib::File().GetFile(), /*  */"windowHeight						%.2f\n", info.windowHeight);
		fprintf(RNLib::File().GetFile(), /*  */"isFullScreen						%d\n"  , info.isFullScreen);
		fprintf(RNLib::File().GetFile(), /*  */"resolution							%.2f\n", info.resolution);
	 	fprintf(RNLib::File().GetFile(), /*  */"#OtherOption\n");
		fprintf(RNLib::File().GetFile(), /*  */"modelOutLineAddDistanceInterval		%.2f\n", info.modelOutLineAddDistanceInterval);
		fprintf(RNLib::File().GetFile(), /*  */"modelOutLineAddDistanceDelimiter	%d\n"  , info.modelOutLineAddDistanceDelimiter);
		fprintf(RNLib::File().GetFile(), /*  */"END");

		// �t�@�C�������
		RNLib::File().CloseFile();
	}

	return isLoad;
}

//========================================
// ���擾����
//========================================
RNSettings::Info RNSettings::GetInfo(void) {

	return info;
}

//========================================
// �X�N���[�����[�h
//========================================
void RNSettings::SetFulScreen(bool screen) {

	info.isFullScreen = screen;
	RNLib::Window().SetFullScreen(screen);
}