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

const Pos3D CTalk::TEXTBOX_UNDER_POS = Pos3D(330.0f, 700.0f, 0.0f); //�e�L�X�g�{�b�N�X�̉�ʉ����ʒu
CFloat      CTalk::POPOVER_FLOAT = 10.0f;           //����ɕ\�����鎞�̕��������

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
	m_nAutoCounter = 0;    //�����i�s�J�E���^�[
	m_bAuto = true;        //�����i�s�t���O

	m_pText = NULL;
	m_pos = TEXTBOX_UNDER_POS;

	SetFontOption(SHOWTYPE::Under);

	m_pShadow.col = COLOR_BLACK; // �e�̐F
	m_pShadow.bShadow = true;                   // �e�t���O
	m_pShadow.AddPos = Pos3D(6.0f, 6.0f, 0.0f); // �����̈ʒu���炸�炷�l
	m_pShadow.AddSize = Pos2D(4.0f, 4.0f);      // �����̃T�C�Y�̉��Z�l

	//��b���O�폜
	DeleteLog();
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
	m_nAutoCounter = 0;    //�����i�s�J�E���^�[
	m_bAuto = false;       //�����i�s�t���O
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

				//��b���O�E�\�����@�ۊǗp
				char LogTmp[TXT_MAX] = {};
				int nTypeTmp = 0;
				while (RNLib::File().SearchLoop("END_TALK")) {
					RNLib::File().Scan(_RNC_File::SCAN::STRING, &LogTmp[0], "TALK");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &s_pTalk[nTalkCounter].TalkerID, "PLAYER");
					RNLib::File().Scan(_RNC_File::SCAN::INT,    &nTypeTmp, "TYPE");
				}

				//��������
				CInt len = strlen(&LogTmp[0]) + 1;
				s_pTalk[nTalkCounter].pLog = new char[len];
				strcpy(&s_pTalk[nTalkCounter].pLog[0], &LogTmp[0]);

				//�\�����@���
				s_pTalk[nTalkCounter].type = (SHOWTYPE)nTypeTmp;

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
		if (Event == EVENT::BEFORE_DEPARTURE) 
			Stage::SetIsCutIn(true);

		//��b�J�n
		m_bTalk = true;
		m_nTalkID = -1; //��b�ԍ�

		NextSpeak();    //���ɕ\������e�L�X�g
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

	//�\�����@��K�p����
	ShowType();

	//�����i�s
	Auto();

	//��b�X�L�b�v
	if (RNLib::Input().GetTrigger(DIK_P, _RNC_Input::BUTTON::START) ||
		RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::START, 1))
	{
		Skip();
	}

	//���̉�b��  or  ��b�I��
	else if (RNLib::Input().GetTrigger(DIK_T, _RNC_Input::BUTTON::Y) ||
			 RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::Y, 1) ||
			 m_nAutoCounter >= AUTO_COUNTER)
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
//�\�����@��K�p����
//=======================================
void CTalk::ShowType(void)
{
	switch (s_pTalk[m_nTalkID].type)
	{
			//==========================
			// ��ʉ����ɕ\��
		case SHOWTYPE::Under:
			m_pos = TEXTBOX_UNDER_POS;
			break;

			//==========================
			// ���f���ƃZ���t��\��
		case SHOWTYPE::Wipe:
			break;

			//==========================
			// ���f���̓���ɃZ���t��\��
		case SHOWTYPE::PopOver:
			//���̐ݒ�
			CInt& TalkerID = s_pTalk[m_nTalkID].TalkerID;

			//���肪�v���C���[�Ŗ�����Ή�ʉ����ʒu�ɐݒ�
			if (TalkerID < 0) m_pos = TEXTBOX_UNDER_POS;
			else
			{
				CPlayer::Info Info = *CPlayer::GetInfo(TalkerID);
				m_pos = Info.pos;

				switch (Info.side)
				{
					case CPlayer::WORLD_SIDE::FACE:   m_pos.y += POPOVER_FLOAT; break;
					case CPlayer::WORLD_SIDE::BEHIND: m_pos.y -= POPOVER_FLOAT; break;
				}
			}
			break;
	}

	//�e�L�X�g�{�b�N�X�̈ʒu�ݒ�
	if (m_pText != NULL)
		m_pText->SetTxtBoxPos(m_pos);
}

//=======================================
//�����i�s
//=======================================
void CTalk::Auto(void)
{
	// �t���O��TRUE�ŃJ�E���^�[����
	if (m_bAuto && m_pText != NULL && m_pText->GetLetter() && m_nTalkID + 1 < m_nTalkNumAll)
		m_nAutoCounter++;

	// �t���O�ؑ�
	if (RNLib::Input().GetTrigger(DIK_Y, _RNC_Input::BUTTON::RIGHT_THUMB) ||
		RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::RIGHT_THUMB, 1))
		m_bAuto = !m_bAuto;

	// �t���O��OFF�ŃJ�E���^�[�N���A
	if (!m_bAuto)
		m_nAutoCounter = 0;
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
		DeleteText();

		m_pText = CFontText::Create(CFontText::BOX_NONE, m_pos, INITPOS2D, s_pTalk[m_nTalkID].pLog,
									CFont::FONT_07NIKUMARU, &m_pFont, false, false, &m_pShadow);

		//�����i�s�J�E���^�[
		m_nAutoCounter = 0;
	}
}

//=======================================
//��b�X�L�b�v
//=======================================
void CTalk::Skip(void)
{
	DeleteLog();
}

//=======================================
//�t�H���g�ݒ�
//=======================================
void CTalk::SetFontOption(const SHOWTYPE& type)
{
	switch (type)
	{
			//==========================
			// ��ʉ����ɕ\��
		case SHOWTYPE::Under:
			m_pFont.col = COLOR_WHITE; // �����̐F
			m_pFont.fTextSize = 40.0f; // �����̃T�C�Y
			break;

			//==========================
			// ���f���ƃZ���t��\��
		case SHOWTYPE::Wipe:
			break;

			//==========================
			// ���f���̓���ɃZ���t��\��
		case SHOWTYPE::PopOver:
			if (s_pTalk != NULL)
			{
				CInt& Talker = s_pTalk[m_nTalkID].TalkerID;

				// �v���C���[�J���[�𕶎��̐F�ɐݒ�
				if (Talker == 0 || Talker == 1)
					m_pFont.col = CPlayer::GetInfo(Talker)->color;

				// �Ⴆ�Δ���
				else m_pFont.col = COLOR_WHITE;

				//�t�H���g�T�C�Y
				m_pFont.fTextSize = 30.0f;
			}
			break;
	}

	m_pFont.nAppearTime = 5;   // 1�����ڂ��\�������܂ł̎���
	m_pFont.nStandTime = 8;    // �ҋ@����
	m_pFont.nEraseTime = 0;    // ������܂ł̎���
}