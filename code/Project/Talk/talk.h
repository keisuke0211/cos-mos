//================================================================================================
// 
// ��b����[talk.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

//��b�N���X
class CTalk {
public:
	//��b�C�x���g
	enum class EVENT {
		NONE,             //�C�x���g����
		BEFORE_DEPARTURE, // �o���O�i�^�C�g��->�X�e�[�W�I���̍��ԁH
		EMERGENCY_LANDING,// �s�������i1-1�̊J�n�C�x���g
		ROCKET_FOUND,     // ���P�b�g����(1-3�N���A��
		MAX,
	};

	CTalk();
	~CTalk();
	void Init(EVENT &Event);
	void Uninit(void);
	void Update(void);

	static CTalk *Create(EVENT Event);    // ��b�C�x���g�w��
	bool IsTalk(void) { return m_bTalk; } // ��b�����ǂ����擾

private:
	//��b�C�x���g�̃t�@�C���p�X
	static const char *EVENT_FILE[(int)EVENT::MAX];

	static const int NEXT_INTERVAL = 20;//���̕�����\������C���^�[�o��

	void DeleteLog(void);        //��b���O�폜
	void LoadTalk(EVENT &Event); //��b�C�x���g�Ǎ�

	void PopUpText(void); //���͕\��
	void NextChar(void);  //���̕�����\��

	//��b���
	struct Talk
	{
		char *pLog;   // ��b���e
		int  nTex;    // �e�N�X�`���ԍ�
		int TalkerID; // ��b���Ă���v���C���[ID
	};

	static Talk *s_pTalk;   //��b���e
	static EVENT s_Event;   //�C�x���g
	bool   m_bTalk;         //��b�����ǂ���
	int    m_nTalkNumAll;   //�ő��b��
	int    m_nTalkID;       //��b�ԍ�
	int    m_nNextInterval; //���̕����E��b��\������܂ł̃C���^�[�o��
	int    m_nStringNumAll; //���݂̉�b�̍ő啶����
	int    m_nStringNum;    //���݂̉�b�̕\��������
	int    m_bEndSpeak;     //�����I���i��b���̂̏I���ł͂Ȃ�
	char   *m_pPopText;     //�\������e�L�X�g
};