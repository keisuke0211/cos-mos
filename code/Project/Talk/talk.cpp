//================================================================================================
// 
// ��b����[talk.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../main.h"
#include "talk.h"

//��b�C�x���g�̃t�@�C���p�X
const char *CTalk::EVENT_FILE[(int)EVENT::MAX] = {
	"",//�C�x���g����
	"data\\TALK\\talk000.txt", // �o���O�i�^�C�g��->�X�e�[�W�I���̍��ԁH
	"data\\TALK\\talk001.txt", // �s�������i1-1�̊J�n�C�x���g
	"data\\TALK\\talk002.txt", // ���P�b�g����(1-3�N���A��
};

CTalk::Talk  *CTalk::s_pTalk = NULL;        //��b���e
CTalk::EVENT  CTalk::s_Event = EVENT::NONE; //�C�x���g

//=======================================
// �R���X�g���N�^
//=======================================
CTalk::CTalk()
{
	s_pTalk = NULL;        //��b���e
	s_Event = EVENT::NONE; //�C�x���g
	m_bTalk = false;       //��b�����ǂ���
	m_nTalkNumAll = 0;     //�ő��b��
	m_nTalkID = 0;         //��b�ԍ�
	m_bEndSpeak = false;   //�����I���i��b���̂̏I���ł͂Ȃ�

	m_pText = NULL;

	m_pFont.col = COLOR_WHITE;   // �����̐F
	m_pFont.fTextSize  = 40.0f;  // �����̃T�C�Y
	m_pFont.nAppearTime = 5;     // 1�����ڂ��\�������܂ł̎���
	m_pFont.nStandTime = 8;      // �ҋ@����
	m_pFont.nEraseTime = 0;      // ������܂ł̎���

	m_pShadow.col = COLOR_BLACK; // �e�̐F
	m_pShadow.bShadow = true;                   // �e�t���O
	m_pShadow.AddPos = Pos3D(6.0f, 6.0f, 0.0f); // �����̈ʒu���炸�炷�l 
	m_pShadow.AddSize = Pos2D(4.0f, 4.0f);      // �����̃T�C�Y�̉��Z�l 

	DeleteLog(); //��b���O�폜
}

//=======================================
// �f�X�g���N�^
//=======================================
CTalk::~CTalk()
{
	//��b���O�폜
	DeleteLog();
}

//=======================================
// ��b�C�x���g�w��
//=======================================
CTalk *CTalk::Create(EVENT Event)
{
	//��b���Ȃ̂�NULL��Ԃ�
	if (s_pTalk != NULL && s_Event != EVENT::NONE && Event == EVENT::NONE) return NULL;

	//�C���X�^���X����
	CTalk *pTalk = new CTalk;

	if (pTalk != NULL)
	{//��������
		//����������
		pTalk->Init(Event);
	}

	//�C���X�^���X��Ԃ�
	return pTalk;
}

//=======================================
// ��b���O�폜
//=======================================
void CTalk::DeleteLog(void)
{
	if (s_pTalk != NULL)
	{
		for (int nCntTalk = 0; nCntTalk < m_nTalkNumAll; nCntTalk++)
		{
			//��b���O�폜
			if (s_pTalk[nCntTalk].pLog != NULL)
			{
				delete[] s_pTalk[nCntTalk].pLog;
				s_pTalk[nCntTalk].pLog = NULL;
			}
		}

		//��b�f�[�^�폜
		delete s_pTalk;
		s_pTalk = NULL;
	}

	s_Event = EVENT::NONE; //�C�x���g
	m_bTalk = false;       //��b�����ǂ���
	m_nTalkNumAll = 0;     //�ő��b��
	m_nTalkID = 0;         //��b�ԍ�
	m_bEndSpeak = false;   //�����I���i��b���̂̏I���ł͂Ȃ�
	DeleteText();          //�\������e�L�X�g�̃������J��
}

//=======================================
//��b�C�x���g�Ǎ�
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	int nTalkCounter = 0;

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(EVENT_FILE[(int)Event])) {
		while (RNLib::File().SearchLoop("END")) {
			if (nTalkCounter != 0 &&
				nTalkCounter >= m_nTalkNumAll) break;

			//��b���擾�E��b���O���m��
			if (RNLib::File().CheckIdentifier("NUM_TALK"))
			{
				RNLib::File().Scan(_RNC_File::SCAN::INT, &m_nTalkNumAll);
				s_pTalk = new Talk[m_nTalkNumAll];
			}

			//��b���e�ǂݎ��
			else if (RNLib::File().CheckIdentifier("SET_TALK")){

				//��b���O�ۊǗp
				char LogTmp[TXT_MAX] = {};
				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].nTex, "TEXTURE");
				}

				//��������
				CInt len = strlen(&LogTmp[0]) + 1;
				s_pTalk[nTalkCounter].pLog = new char[len];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				//���̔ԍ���
				nTalkCounter++;
			}
		}

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//=======================================
// ����������
//=======================================
void CTalk::Init(EVENT &Event)
{
	//��b���O�Ǎ�
	LoadTalk(Event);

	//�ǂݍ��߂�
	if (s_pTalk != NULL)
	{
		//��b�J�n
		m_bTalk = true;
		m_nTalkID = -1;      //��b�ԍ�
		m_bEndSpeak = false; //�����I���i��b���̂̏I���ł͂Ȃ�

		NextSpeak();         //���ɕ\������e�L�X�g
	}
}

//=======================================
// �I������
//=======================================
void CTalk::Uninit(void)
{
	//�f�[�^�J��
	DeleteLog();
}

//=======================================
// �X�V����
//=======================================
void CTalk::Update(void)
{
	//��b�I��
	if (s_pTalk == NULL || !m_bTalk)
		return;

	//���̉�b��  or  ��b�I��
	else if (!m_bEndSpeak &&
			 (RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y) ||
			 RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y, 1)))
	{
		//���̔�����
		NextSpeak();
	}
}

//=======================================
//�\������e�L�X�g�̃������J��
//=======================================
void CTalk::DeleteText(void)
{
	//�������J��
	if (m_pText != NULL)
	{
		m_pText->Uninit();
		m_pText = NULL;
	}
}

//=======================================
//���ɂ���ׂ�e�L�X�g�̐ݒ�
//=======================================
void CTalk::NextSpeak(void)
{
	//���̒�����
	m_nTalkID++;

	//��b�I��
	if (m_nTalkID == m_nTalkNumAll)
	{
		DeleteLog();
	}
	else
	{
		//���̉�b�̍ő啶����
		m_bEndSpeak = false;
		DeleteText();

		m_pText = CFontText::Create(CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(330.0f, 700.0f, 0.0f), INITPOS2D,
									s_pTalk[m_nTalkID].pLog, CFont::FONT_07NIKUMARU, &m_pFont, false, false, &m_pShadow);
	}
}