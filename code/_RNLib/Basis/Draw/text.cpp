//========================================
// 
// �e�L�X�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"
#include "../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�e�L�X�g�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Text::_RNC_Text() {

	m_defaultFontData = {};
	m_fontDatas       = NULL;
	m_fontNum         = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Text::~_RNC_Text() {
	
}

//========================================
// ����������
//========================================
void _RNC_Text::Init(void) {

	// �f�t�H���g�t�H���g�f�[�^���쐬
	m_defaultFontData.texIdx    = RNLib::Texture().Load("RNData\\Texture\\Font.png");
	m_defaultFontData.startCode = 0;
	m_defaultFontData.ptnX      = 128;
	m_defaultFontData.ptnY      = 512;

	// �t�H���g���X�g��ǂݍ���
	RNLib::Text().LoadFont(RNSettings::GetInfo().fontListPath);
}

//========================================
// �I������
//========================================
void _RNC_Text::Uninit(void) {

	// �t�H���g���̃|�C���^�̃��������
	RNLib::Memory().Release(&m_fontDatas);
}

//========================================
// �X�V����
//========================================
void _RNC_Text::Update(void) {

}

//========================================
// �t�H���g�̓ǂݍ��ݏ���
//========================================
void _RNC_Text::LoadFont(const char* loadPath) {

	if (!RNLib::File().OpenLoadFile(loadPath))
		return;

	// �ǂݍ��݃��[�v
	while (RNLib::File().SearchLoop("END")) {
		if (RNLib::File().CheckIdentifier("fontDatas{")) {

			// �t�H���g����ǂݍ���
			RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_fontNum);
			RNLib::Memory().Alloc(&m_fontDatas, m_fontNum);

			UShort fontCount = 0;
			while (RNLib::File().SearchLoop("}")) {
				if (RNLib::File().CheckIdentifier("fontData{")) {

					// �G���[���b�Z�[�W
					if (fontCount >= m_fontNum) {
						RNLib::Window().Message_ERROR(String("�t�H���g�f�[�^�̐����w�萔���I�[�o�[���Ă��܂��B\n%s", loadPath));
						break;
					}

					while (RNLib::File().SearchLoop("}")) {
						RNLib::File().Scan(_RNC_File::SCAN::TEXIDX,&m_fontDatas[fontCount].texIdx   , "texIdx");
						RNLib::File().Scan(_RNC_File::SCAN::USHORT,&m_fontDatas[fontCount].startCode, "startCode");
						RNLib::File().Scan(_RNC_File::SCAN::USHORT,&m_fontDatas[fontCount].ptnX     , "ptnX");
						RNLib::File().Scan(_RNC_File::SCAN::USHORT,&m_fontDatas[fontCount].ptnY     , "ptnY");
					}

					if (++fontCount > m_fontNum)
						RNLib::Window().Message_ERROR("");
				}
			}

			// �G���[���b�Z�[�W
			if (fontCount < m_fontNum) {
				RNLib::Window().Message_ERROR(String("�t�H���g�f�[�^�̐����w�萔�ɑ΂��ĕs�����Ă��܂��B\n%s", loadPath));
				break;
			}
		}
	}

	// �t�@�C�������
	RNLib::File().CloseFile();
}

//========================================
// �t�H���g�̕�/�����擾
//========================================
float _RNC_Text::GetFontWidth(const short& fontIdx) {
	
	return RNLib::Texture().GetWidth(m_fontDatas[fontIdx].texIdx) / m_fontDatas[fontIdx].ptnX;
}
float _RNC_Text::GetFontHeight(const short& fontIdx) {

	return RNLib::Texture().GetHeight(m_fontDatas[fontIdx].texIdx) / m_fontDatas[fontIdx].ptnY;
}