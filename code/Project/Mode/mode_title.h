//========================================
// 
// ���[�h:�^�C�g���̏���
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// �O���錾
//****************************************
class CFontText;
class CWords;

//****************************************
// �N���X��`
//****************************************
// ���[�h(�^�C�g��)�N���X
class CMode_Title :public CMode {
public:
	//========== [[[ �萔��` ]]]
	static const char* TEXT_FILE;				// �e�L�X�g���̃t�@�C���p�X
	static const int WORDS_MAX         = 7;		// �����̍ő吔
	static const int FONT_TEXT_MAX     = 10;	// �e�L�X�g�̍ő吔
	static const int PAUSE_LEFT_ANIME  = 15;	// ��ʍ��̃A�j���[�V��������
	static const int PAUSE_RIGHT_ANIME = 15;	// ��ʉE�̃A�j���[�V��������
	static const int VOLUME_MSX        = 20;	// �T�E���h�̍ő�l
	static const int COOLDOWN          = 20;	// �N�[���_�E��


	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE,
	};
	enum TITLE {
		TITLE_TITLE = 0,	// �^�C�g��
		TITLE_OUTSET,		// ���o�I��
		TITLE_MENU_ANIME,	// ���j���[���o
		TITLE_MENU,			// ���j���[
		TITLE_SELECT,		// �X�e�[�W�I��
		TITLE_NEXT,			// ���̉�ʂɈړ� 
		TITLE_MAX
	};	

	//========== [[[ �֐��錾 ]]]
	static void SetSelect(bool bSelect) { s_bStageSelect = bSelect; }
	CMode_Title();
	~CMode_Title();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

private:
	//========== [[[ �񋓌^��` ]]]
	enum MENU{
		MENU_GAME = 0,	// �Q�[��
		MENU_CONTROLLER,// ������@
		MENU_SETTING,	// �ݒ�
		MENU_END,		// �I��
		MENU_MAX
	};
	enum TEX{
		TEX_BG = 0,		// �w�i
		TEX_PLANET,		// �f��
		TEX_MAX
	};
	enum CONTROLLER {
		INPUT_TITLE = 0,// �^�C�g��
		INPUT_MOVE,		// �ړ�
		INPUT_JUMP,		// �W�����v
		INPUT_SWAP,		// �X���b�v
		INPUT_DECISION,	// ����
		INPUT_BACK,		// �߂�
		INPUT_MAX
	};
	enum SETTING {
		SETTING_SCREEN = 1,	// �t���X�N���[��
		SETTING_BGM ,		// BGM
		SETTING_SE,			// SE
		SETTING_BACK,
		SETTING_BGM_TEXT,
		SETTING_SE_TEXT,
		SETTING_MAX
	};
	enum TEXT {
		TEXT_TITLE = 0,	// �^�C�g��
		TEXT_MENU,		// ���j���[
		TEXT_RIGHT,		// �����
		TEXT_ALL,		// �S��
		TEXT_MAX
	};

	//========== [[[ �\���̒�` ]]]
	// �X�e�[�W��ޏ��
	struct PlanetType{
		int nModel;				// ���f��
		char Text[TXT_MAX];		// �X�e�[�W��
	};

	// ������@�̃e�L�X�g���
	struct Operation{
		char Text[TXT_MAX];		// �e�L�X�g
	};

	// �ݒ���
	struct Setting{
		char Text[TXT_MAX];		// �e�L�X�g
	};

	// ���j���[���
	struct Menu {
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
		int nCntLeftAnime;
		int nCntRightAnime;
		int nRightCoolDown;	//�@����ʏo���̃N�[���_�E��
		int nMaineSelect;
		int nMaineOldSelect;
		int nSubSelect;
		int nRightTextType;
		bool bMenu;
		bool bRightMove;
		bool bRightDisp;
		bool bRightCoolDown;
		bool bClose;
		bool bSubMenu;

		int BoxTex;
		int OperationMax;
		int SettingMax;

		// �X�N���[��
		int nCntScrChg;		// �X�N���[���ύX�̃J�E���g
		bool bFullScreen;	// �X�N���[�����[�h

		// �T�E���h
		int nBGMVolume;
		int nSEVolume;
		int nBGMOldVolume;
		int nSEOldVolume;

		Operation *pOperation;
		Setting *pSetting;
	};

	// *** �֐� ***
	/* �e�L�X�g�̓Ǎ�			*/void TextLoad(void);
	/* �ݒ菈��					*/void SettingMenu(void);
	/* �e�L�X�g�A�j���[�V����	*/void TextAnime(void);
	/* ���j���[�A�j���[�V����	*/void MenuAnime(void);
	/* ���j���[����				*/void MenuCreate(void);
	/* ���j���[�I��				*/void MenuSelect(void);
	/* �T�u�e�L�X�g�̐���		*/void SubTextCreate(void);
	/* �X�e�[�W�Ǎ�				*/void StageLoad(void);
	/* �X�e�[�W�I��				*/void StageSelect(void);
	/* �e�L�X�g�폜				*/void TextRelease(TEXT type);
	/* ���[�h�؂�ւ�			*/void SwapMode(TITLE aTitle);

	// *** �ÓI�ϐ� ***
	static bool s_bStageSelect;

	// *** �ϐ� ***
	TITLE Title;
	Menu m_Menu;
	D3DXVECTOR3 m_BgPos[TEX_MAX];
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_nSelect;
	int m_nOldSelect;
	int m_nPlanetIdx;
	int m_nOldnPlanet;
	int m_RocketIdx;
	bool m_bMove[WORDS_MAX];
	bool m_bBackMode;
	CWords *m_TITLE[WORDS_MAX];
	CWords *m_TitleShadow[WORDS_MAX];
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
	PlanetType *m_PlanetType;
};