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
		pTalk->Init();
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
}

//=======================================
//��b�C�x���g�Ǎ�
//=======================================
void CTalk::LoadTalk(EVENT &Event)
{
	//�����Y��
	const char *CODE_TALK    = "TALK";
	const char *CODE_PLAYER  = "PLAYER";
	const char *CODE_TEXTURE = "TEXTURE";

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(EVENT_FILE[(int)Event], "Talk")) {
		while (RNLib::File().SearchLoop("EOF")) {
			//��b���擾�E��b���O���m��
			if (RNLib::File().CheckIdentifier("NUM_TALK"))
			{
				RNLib::File().Scan(_RNC_File::SCAN::INT, &m_nTalkNumAll);
				s_pTalk = new Talk[m_nTalkNumAll];
			}

			//��b���e�ǂݎ��
			else if (RNLib::File().CheckIdentifier("SET_TALK")){
				while (RNLib::File().SearchLoop("END_TALK")) {
					//RNLib::File().Scan(_RNC_File::SCAN::BOOL, &m_isLoop, "isLoop");
					//RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &m_scale, "scale");
				}
			}
		}

		// �t�@�C�������
		RNLib::File().CloseFile();
	}

	//�ǂݍ��߂�
	if (s_pTalk != NULL)
	{
		//��b�J�n
		m_bTalk = true;
	}
}

//=======================================
// ����������
//=======================================
void CTalk::Init(void)
{

}

//=======================================
// �I������
//=======================================
void CTalk::Uninit(void)
{

}

//=======================================
// �X�V����
//=======================================
void CTalk::Update(void)
{

}

//=======================================
// ���͕\��
//=======================================
void CTalk::PopUpText(void)
{

}

//=======================================
// ���̕�����\��
//=======================================
void CTalk::NextChar(void)
{

}