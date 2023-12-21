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
	"",                 //�C�x���g����
	"data\\TALK\\.txt", // �o���O�i�^�C�g��->�X�e�[�W�I���̍��ԁH
	"data\\TALK\\.txt", // �s�������i1-1�̊J�n�C�x���g
	"data\\TALK\\.txt", // ���P�b�g����(1-3�N���A��
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
	m_nNextInterval = 0;   //���̕����E��b��\������܂ł̃C���^�[�o��
	m_nStringNumAll = 0;   //���݂̉�b�̍ő啶����
	m_nStringNum = 0;      //���݂̉�b�̕\��������
	m_bEndSpeak = false;   //�����I���i��b���̂̏I���ł͂Ȃ�
	m_pPopText = NULL;     //�\������e�L�X�g

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
	if (s_pTalk != NULL && s_Event != EVENT::NONE) return NULL;

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
	m_nNextInterval = 0;   //���̕����E��b��\������܂ł̃C���^�[�o��
	m_nStringNumAll = 0;   //���݂̉�b�̍ő啶����
	m_nStringNum = 0;      //���݂̉�b�̕\��������
	m_bEndSpeak = false;   //�����I���i��b���̂̏I���ł͂Ȃ�
	m_pPopText = NULL;     //�\������e�L�X�g
}

//=======================================
//��b�C�x���g�Ǎ�
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	int nTalkCounter = 0;

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile("data\\TALK\\test.txt")) {
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
				//�ꎞ������
				s_pTalk[nTalkCounter].pLog = NULL;
				s_pTalk[nTalkCounter].TalkerID = NONEDATA;
				s_pTalk[nTalkCounter].nTex = NONEDATA;

				char LogTmp[TXT_MAX] = {};

				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].nTex, "TEXTURE");
				}

				CInt len = strlen(&LogTmp[0]);
				s_pTalk[nTalkCounter].pLog = new char[len + 1];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				ZeroMemory(LogTmp, len);
				strcpy(&LogTmp[0], &s_pTalk[nTalkCounter].pLog[0]);
				CInt Talker = s_pTalk[nTalkCounter].TalkerID;
				CInt nTex = s_pTalk[nTalkCounter].nTex;

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
		m_nTalkID = 0;       //��b�ԍ�
		m_nNextInterval = 0; //���̕����E��b��\������܂ł̃C���^�[�o��
		m_nStringNum = 0;    //���݂̉�b�̕\��������
		m_bEndSpeak = false; //�����I���i��b���̂̏I���ł͂Ȃ�
		m_pPopText = NULL;   //�\������e�L�X�g

		m_nStringNumAll = strlen(s_pTalk[0].pLog) + 1; //���݂̉�b�̍ő啶����
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
	if (s_pTalk == NULL)
		return;

	//�e�L�X�g�\��
	PopUpText();

	//��b�I��
	if (!m_bTalk)
		return;

	//�������܂��I����Ă��Ȃ�
	else if (!m_bEndSpeak)
		NextChar();

	//���̉�b��  or  ��b�I��
	else if (RNLib::Input().GetTrigger(DIK_RETURN, _RNC_Input::BUTTON::A) ||
			 RNLib::Input().GetTrigger(DIK_RETURN, _RNC_Input::BUTTON::A, 1))
	{
		//���̔�����
		m_nTalkID++;
		m_bEndSpeak = false;
		m_pPopText = NULL;

		//��b�I��
		if (m_nTalkID == m_nTalkNumAll)
			m_bTalk = false;
	}
}

//=======================================
// ���̕�����\��
//=======================================
void CTalk::NextChar(void)
{
	//�C���^�[�o���������ďI��
	if (--m_nNextInterval > 0) {
		m_nNextInterval--;
		return;
	}

	char Text[TXT_MAX] = {};

	//�C���^�[�o���N���A
	m_nNextInterval = 0;

	//���̕������ցi���̕��������ő吔�𒴂�����ő啶�����ɐݒ�
	CInt NextTextNum = m_nStringNum + 2 <= m_nStringNumAll ? m_nStringNum + 2 : m_nStringNumAll;

	//���̕����܂Őڑ�
	for (int nCntText = 0; nCntText < NextTextNum; nCntText++)
	{
		sprintf(&Text[0], "%s%c", &Text[0], s_pTalk[m_nTalkID].pLog[nCntText]);
	}

	//m_pPopText�J���E�m��
	//������R�s�[
	//strcpy(&m_pPopText[0], &Text[0]);

	//���������ő吔�ƈ�v�Ȃ甭���I��
	if (NextTextNum == m_nStringNumAll)
		m_bEndSpeak = true;

	//�C���^�[�o���ݒ�
	m_nNextInterval = NEXT_INTERVAL;
}

//=======================================
// ���͕\��
//=======================================
void CTalk::PopUpText(void)
{
	//�e�L�X�g�̔w�i
	RNLib::Polygon2D().Put(PRIORITY_UI, Pos2D(RNLib::Window().GetCenterX(), 600.0f), 0.0f)
		->SetCol(Color{ 10,10,10,200 })
		->SetSize(600.0f, 40.0f);

	//�e�L�X�g
	RNLib::Text2D().Put(PRIORITY_TEXT, m_pPopText, _RNC_Text::ALIGNMENT::LEFT, NONEDATA, Pos2D(60.0f, 600.0f), 0.0f)
		->SetSize(Size2D(10.0f, 10.0f));
}