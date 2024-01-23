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

//��ʉ����\���̈ʒu�E�T�C�Y
const Pos3D CTalk::TYPE_UNDER_POS = Pos3D(230.0f, 700.0f, 0.0f);
CFloat CTalk::TYPE_UNDER_FONT_SIZE = 40.0f;

/*  �J�[�e���̕�  */ CFloat CTalk::TYPE_CURTAIN_WIDTH = 1280.0f;
/* �J�[�e���̍��� */ CFloat CTalk::TYPE_CURTAIN_HEIGHT = 40.0f;
/*�㕔�J�[�e���ʒu*/ CFloat CTalk::TYPE_CURTAIN_OVER_BEHIND_POS_Y = -TYPE_CURTAIN_HEIGHT;
/*�����J�[�e���ʒu*/ CFloat CTalk::TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y = 720.0f + TYPE_CURTAIN_HEIGHT;
/* �t�H���g�T�C�Y */ CFloat CTalk::TYPE_CURTAIN_FONT_SIZE = 35.0f;

/*�A�j���[�V�����J�E���^�[*/int CTalk::s_CurtainCounter = 0;

CTalk::Talk  *CTalk::s_pTalk = NULL;        //��b���e
CTalk::EVENT  CTalk::s_Event = EVENT::NONE; //�C�x���g

short CTalk::s_1P_Voice = NONEDATA;
short CTalk::s_2P_Voice = NONEDATA;

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
	m_pTextSub = NULL;
	m_pos = TYPE_UNDER_POS;

	SetFontOption(SHOWTYPE::Under);

	//��b���O�폜
	DeleteLog();
}

//=======================================
// �f�X�g���N�^
//=======================================
CTalk::~CTalk()
{
	s_CurtainCounter = 0;

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

	//�{�C�X�Ǎ�
	if (s_1P_Voice == NONEDATA) s_1P_Voice = RNLib::Sound().Load("data\\SOUND\\SE\\Voice\\cos.wav");
	if (s_2P_Voice == NONEDATA) s_2P_Voice = RNLib::Sound().Load("data\\SOUND\\SE\\Voice\\mos.wav");

	s_Event = EVENT::NONE; //�C�x���g
	m_bTalk = false;       //��b�����ǂ���
	m_nTalkNumAll = 0;     //�ő��b��
	m_nTalkID = 0;         //��b�ԍ�
	m_nAutoCounter = 0;    //�����i�s�J�E���^�[
	m_bAuto = true;        //�����i�s�t���O
	s_CurtainCounter = 0;  //�A�j���[�V�����J�E���^�[
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
	RNLib::Text2D().Put(PRIORITY_TEXT, "�����炵���ӂ����", _RNC_Text::ALIGNMENT::LEFT, NONEDATA, Pos2D(200.0f, 200.0f), 0.0f);

	//��b�I��
	if (s_pTalk == NULL)
		return;

	if (!m_bTalk)
	{
		SetCurtain();
		return;
	}

	//�\�����@��K�p����
	ShowType();

	//�����i�s
	Auto();

	//��b�X�L�b�v
	if (RNLib::Input().GetTrigger(DIK_U, _RNC_Input::BUTTON::START) ||
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

	//�e�L�X�g�{�b�N�X�̈ʒu�ݒ�
	if (m_pText != NULL && s_pTalk[m_nTalkID].type == SHOWTYPE::Curtain)
	{
		SetCurtain(false);
		m_pText->SetTxtBoxPos(m_pos, false, true);
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
	if (m_pTextSub != NULL)
	{
		m_pTextSub->Uninit();
		m_pTextSub = NULL;
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
			m_pos = TYPE_UNDER_POS;
			break;

			//==========================
			// ��ʏ㉺�ɍ�����p�ӂ��Ă��̏�ɃZ���t��\��
		case SHOWTYPE::Curtain:
			SetCurtain();
			break;
	}

	//�t�H���g�ݒ�
	SetFontOption(s_pTalk[m_nTalkID].type);
}

//=======================================
//�����i�s
//=======================================
void CTalk::Auto(void)
{
	// �t���O��TRUE�ŃJ�E���^�[����
	if (m_bAuto && m_pText != NULL && m_pText->GetLetter())
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
		m_pText->SetTextSkip(true);
		m_bTalk = false;
	}
	else
	{
		//���̉�b�̍ő啶����
		DeleteText();

		//�t�H���g�ݒ�
		SetFontOption(s_pTalk[m_nTalkID].type);

		//�T�E���h�ݒ�@����ԍ����v���C���[�ԍ��ȊO�Ȃ�T�E���h�����B�@���v���Ă���Ȃ����ԍ����
		CShort SeIdx = 
			s_pTalk[m_nTalkID].TalkerID < 0 || s_pTalk[m_nTalkID].TalkerID > 2 ? NONEDATA :
			s_pTalk[m_nTalkID].TalkerID % 2 == 0 ? s_1P_Voice : s_2P_Voice;

		m_pText = CFontText::Create(CFontText::BOX_NONE, m_pos, INITPOS2D, s_pTalk[m_nTalkID].pLog,
									CFont::FONT_WAKUWAKU, &m_pFont, false, false, NULL, NULL, SeIdx);

		//��������ꍇ
		if (s_pTalk[m_nTalkID].TalkerID == 2)
		{
			//2P�ݒ�
			m_pFont.col = GetTalkerColor(1);
			m_pos.y = GetTalkerPosY(1);
			m_pTextSub = CFontText::Create(CFontText::BOX_NONE, m_pos, INITPOS2D, s_pTalk[m_nTalkID].pLog,
										CFont::FONT_WAKUWAKU, &m_pFont, false, false, NULL, NULL, s_2P_Voice);
		}

		//�e�L�X�g�{�b�N�X�̈ʒu�ݒ�
		if (m_pText != NULL && s_pTalk[m_nTalkID].type == SHOWTYPE::Curtain)
		{
			SetCurtain(false);
			m_pText->SetTxtBoxPos(m_pos, false, true);
		}

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
	m_pFont.nAppearTime = 5; // 1�����ڂ��\�������܂ł̎���
	m_pFont.nStandTime = NEXT_POPUP_INTERVAL;  // �ҋ@����
	m_pFont.nEraseTime = 0;  // ������܂ł̎���

	switch (type) {
			//==========================
			// ��ʉ����ɕ\��
		case SHOWTYPE::Under: m_pFont.fTextSize = TYPE_UNDER_FONT_SIZE; break;

			//==========================
			// ��ʏ㉺�ɍ�����p�ӂ��Ă��̏�ɃZ���t��\��
		case SHOWTYPE::Curtain: 
			m_pFont.fTextSize = TYPE_CURTAIN_FONT_SIZE;

			if (m_pText != NULL)
			{
				//�Ö����\��������܂őҋ@
				if (m_nTalkID == 0 && m_pText->GetPopCount() == 0)
					m_pFont.nAppearTime = TYPE_CURTAIN_APPEAR_INTERVAL;

				//�o�����Ԃ�ݒ�
				m_pText->SetAppearTime(m_pFont.nAppearTime);
			}
			break;
	}

	//�F�擾
	if (s_pTalk != NULL)
		m_pFont.col = GetTalkerColor(s_pTalk[m_nTalkID].TalkerID);
}

//=======================================
//�Ö��ݒ�
//=======================================
void CTalk::SetCurtain(const bool bSetCurtain)
{
	//�J�E���^�[�̊����Z�o  ->  �o���ʎZ�o
	CFloat rate = RNLib::Ease().Easing(EASE_TYPE::LINEAR, s_CurtainCounter, TYPE_CURTAIN_COUNTER);
	CFloat Move = rate * TYPE_CURTAIN_HEIGHT * 1.5f;

	//�J�E���^�[����
	if (m_bTalk && s_CurtainCounter < TYPE_CURTAIN_COUNTER)
		s_CurtainCounter++;

	else if (!m_bTalk && 0 < s_CurtainCounter)
		s_CurtainCounter--;

	//��ʁE�J�[�e���㉺�̒��S�ʒu
	CFloat CenterX = RNLib::Window().GetCenterX();
	CFloat CurtainOverPos = TYPE_CURTAIN_OVER_BEHIND_POS_Y + Move;
	CFloat CurtainBottomPos = TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y - Move;
	const Color CURTAIN_COLOR = { 0,0,0,100 };

	//�Ö��z�u
	if (bSetCurtain)
	{
		//��̖�
		RNLib::Polygon2D().Put(PRIORITY_EFFECT, Pos2D(CenterX, CurtainOverPos), 0.0f)
			->SetSize(TYPE_CURTAIN_WIDTH, TYPE_CURTAIN_HEIGHT)
			->SetCol(CURTAIN_COLOR);

		//���̖�
		RNLib::Polygon2D().Put(PRIORITY_EFFECT, Pos2D(CenterX, CurtainBottomPos), 0.0f)
			->SetSize(TYPE_CURTAIN_WIDTH, TYPE_CURTAIN_HEIGHT)
			->SetCol(CURTAIN_COLOR);
	}

	//�J�E���^�[���O�ŃZ���t�폜
	if (s_CurtainCounter == 0)
	{
		DeleteLog();
		return;
	}

	//�\�����W�擾
	m_pos.y = GetTalkerPosY(s_pTalk[m_nTalkID].TalkerID);
}

//=======================================
//����ʃZ���t�J���[�擾
//=======================================
Color CTalk::GetTalkerColor(CInt &Talker)
{
	// �v���C���[�J���[�ݒ�
	if (Talker == 0 || Talker == 2)
		return CPlayer::P1_COLOR;
	else if (Talker == 1)
		return CPlayer::P2_COLOR;

	// �Ⴆ�Δ���
	else return COLOR_WHITE;
}

//=======================================
//����ʃZ���t�\�����W�擾
//=======================================
float CTalk::GetTalkerPosY(CInt &Talker)
{
	if (Talker <= 0 || Talker <= 2)
	{
		switch ((int)CPlayer::GetInfo(Talker)->side)
		{
			case (int)CPlayer::WORLD_SIDE::FACE:
			case 2:
				return TYPE_CURTAIN_OVER_BEHIND_POS_Y + TYPE_CURTAIN_HEIGHT * 1.5f; break;

			case (int)CPlayer::WORLD_SIDE::BEHIND:
				return TYPE_CURTAIN_BOTTOM_BEHIND_POS_Y - TYPE_CURTAIN_HEIGHT * 1.5f; break;
		}
	}

	//���ݒn��Ԃ�
	return m_pos.y;
}