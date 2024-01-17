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

	//��b�\���̎��
	enum class SHOWTYPE {
		Under = -1,// ��ʒ��������ɕ\��
		Wipe,      // ���f���ƃZ���t��\��
		Curtain,   // ��ʏ㉺�ɈÖ���p�ӂ��Ă��̏�ɃZ���t��\��
		MAX
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

	//�e�L�X�g�{�b�N�X�̉�ʉ����ʒu
	static const Pos3D TEXTBOX_UNDER_POS; 

	static CFloat CURTAIN_WIDTH;              // �J�[�e���̕�
	static CFloat CURTAIN_HEIGHT;             // �J�[�e���̍���
	static CFloat CURTAIN_OVER_BEHIND_POS_Y;  // �㕔�J�[�e���ʒu
	static CFloat CURTAIN_BOTTOM_BEHIND_POS_Y;// �����J�[�e���ʒu
	static CInt CURTAIN_COUNTER = 20;         // �J�[�e���̕\��ON/OFF�̃A�j���[�V�����J�E���^�[
	static int s_CurtainCounter;              // �A�j���[�V�����J�E���^�[

	static CInt NEXT_POPUP_INTERVAL = 4; //���̕�����\������C���^�[�o��
	static CInt NEXT_SPEAK_INTERVAL = 40;//���̔����܂ł̃C���^�[�o��
	static CInt AUTO_COUNTER = 100;       //�����I�����玩���i�s����܂ł̃J�E���^�[

	void DeleteLog(void);        //��b���O�폜
	void LoadTalk(EVENT &Event); //��b�C�x���g�Ǎ�

	void SetFontOption(const SHOWTYPE& type);

	void ShowType(void);  //�\�����@��K�p����
	void Auto(void);      //�����i�s
	void DeleteText(void);//�\������e�L�X�g�̃������m�ہi������NULL�Ȃ�J���̂�
	void NextSpeak(void); //���ɂ���ׂ�e�L�X�g�̐ݒ�
	void Skip(void);      //��b�X�L�b�v

	void SetCurtain(void);//�Ö��ݒ�

	//��b���
	struct Talk
	{
		char *pLog;   // ��b���e
		int TalkerID; // ��b���Ă���v���C���[ID
		SHOWTYPE type;// �`����@
	};

	static Talk *s_pTalk; //��b���e
	static EVENT s_Event; //�C�x���g
	bool   m_bTalk;       //��b�����ǂ���
	CFontText *m_pText;
	FormFont   m_pFont;

	Pos3D  m_pos;
	Pos2D  m_size;
	int    m_nTalkNumAll; //�ő��b��
	int    m_nTalkID;     //��b�ԍ�
	bool   m_bAuto;       //�����i�s�t���O
	int    m_nAutoCounter;//�����i�s�̃J�E���^�[
};