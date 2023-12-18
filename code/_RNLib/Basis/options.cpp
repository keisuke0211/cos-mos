//========================================
// 
// �I�v�V��������
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�I�v�V�����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Options::_RNC_Options() {

	m_optionsDataPath         = NULL;
	m_screenModeChangeCounter = 0;
	m_isFullScreen            = false;
	for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; m_categoryVolumes[cnt] = 1.0f, cnt++);
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Options::~_RNC_Options() {

}

//========================================
// ����������
//========================================
void _RNC_Options::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_Options::Uninit(void) {

	// �������ݏ���
	Save();
}

//========================================
// �X�V����
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
// �ǂݍ��ݏ���
//========================================
void _RNC_Options::Load(void) {

	// �ǂݍ��ރt�@�C���Ƃ��ĊJ�������A
	if (RNLib::File().OpenLoadFile(m_optionsDataPath, "OptionsData")) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(_RNC_File::SCAN::BOOL, &m_isFullScreen, "isFullScreen");
			if (RNLib::File().CheckIdentifier("categoryVolumes")) {
				for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; cnt++)
					RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &m_categoryVolumes[cnt]);
			}
		}

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// �������ݏ���
//========================================
void _RNC_Options::Save(void) {

	// �������ރt�@�C���Ƃ��ĊJ��
	if (RNLib::File().OpenSaveFile(m_optionsDataPath)) {
		fprintf(RNLib::File().GetFile(), "OptionsData\n");
		fprintf(RNLib::File().GetFile(), "isFullScreen %d\n", m_isFullScreen);
		fprintf(RNLib::File().GetFile(), "categoryVolumes");
		for (int cnt = 0; cnt < (int)_RNC_Sound::CATEGORY::MAX; cnt++)
			fprintf(RNLib::File().GetFile(), " %f", m_categoryVolumes[cnt]);
		fprintf(RNLib::File().GetFile(), "\n");
		fprintf(RNLib::File().GetFile(), "END");

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// �t���X�N���[���ݒ菈��
//========================================
void _RNC_Options::SetIsFullScreen(const bool& isFullScreen) {

	if (m_isFullScreen != isFullScreen) {
		m_isFullScreen = isFullScreen;
		m_screenModeChangeCounter = SCREEN_MODE_CHANGE_TIME;
	}
}