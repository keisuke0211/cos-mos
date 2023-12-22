//================================================================================================
// 
// ��b����[talk.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

#include "../System/words/font-text.h"

//��b�N���X
class CTalk {
public:
	//��b�C�x���g
	enum class EVENT {
		NONE,             //�C�x���g����
		BEFORE_DEPARTURE, // �o���O�i�^�C�g��->�X�e�[�W�I���̍���
		OPENING_1_1,      // �s�������i1-1��OP�C�x���g
		ROCKET_FOUND,     // ���P�b�g����(1-3�N���A��
		MAX,
	};

	CTalk();
	~CTalk();
	void Init(EVENT &Event);
	void Uninit(void);
	void Update(void);

	// -- �ݒ� ------------------------------------------
	void SetPos(const Pos3D &pos) { m_pos = pos; }
	void SetSize(CFloat size)     { m_pFont.fTextSize = size; }

	// -- �擾 ------------------------------------------
	static CTalk *Create(EVENT Event);    // ��b�C�x���g�w��
	bool IsTalk(void) { return m_bTalk; } // ��b�����ǂ����擾

private:
	//��b�C�x���g�̃t�@�C���p�X
	static const char *EVENT_FILE[(int)EVENT::MAX];

	static const int NEXT_POPUP_INTERVAL = 4; //���̕�����\������C���^�[�o��
	static const int NEXT_SPEAK_INTERVAL = 40;//���̔����܂ł̃C���^�[�o��

	void DeleteLog(void);        //��b���O�폜
	void LoadTalk(EVENT &Event); //��b�C�x���g�Ǎ�

	void DeleteText(void);//�\������e�L�X�g�̃������m�ہi������NULL�Ȃ�J���̂�
	void NextSpeak(void); //���ɂ���ׂ�e�L�X�g�̐ݒ�

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
	CFontText *m_pText;
	FormFont   m_pFont;
	FormShadow m_pShadow;

	Pos3D  m_pos;
	Pos2D  m_size;
	int    m_nTalkNumAll; //�ő��b��
	int    m_nTalkID;     //��b�ԍ�
	bool   m_bEndSpeak;   //�����I���i��b���̂̏I���ł͂Ȃ�
};