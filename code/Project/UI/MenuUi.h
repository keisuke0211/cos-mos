//========================================
// 
// ���j���[
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "../manager.h"

//****************************************
// �O���錾
//****************************************
class CFontText;

//****************************************
// �N���X��`
//****************************************
// ���[�h(�^�C�g��)�N���X
class CMenuUI {
public:
	//========== [[[ �萔��` ]]]
	static const char* TITLE_MENU_FILE;			// ���C�����j���[���̃t�@�C���p�X
	static const char* PAUSE_MENU_FILE;			// ���C�����j���[���̃t�@�C���p�X
	static const char* SUB_MENU_FILE;			// �T�u���j���[���̃t�@�C���p�X
	static const int WORDS_MAX = 7;				// �����̍ő吔
	static const int FONT_TEXT_MAX = 10;		// �e�L�X�g�̍ő吔
	static const int PAUSE_LEFT_ANIME = 20;		// ��ʍ��̃A�j���[�V��������
	static const int PAUSE_RIGHT_ANIME = 20;	// ��ʉE�̃A�j���[�V��������
	static const int VOLUME_MSX = 20;			// �T�E���h�̍ő�l
	static const int COOLDOWN = 20;				// �N�[���_�E��

	//========== [[[ �񋓌^��` ]]]
	enum TITLE_MENU {
		TITLE_MENU_GAME = 0,	// �Q�[��
		TITLE_MENU_SETTING,		// �ݒ�
		TITLE_MENU_END,			// �I��
		TITLE_MENU_MAX,
	};

	// ���j���[
	enum PAUSE_MENU {
		PAUSE_MENU_RESUME = 0,	// ������
		PAUSE_MENU_RESET,		// ��蒼��
		PAUSE_MENU_SELECT,		// �X�e�[�W�I��
		PAUSE_MENU_CONTROLLER,	// ������@
		PAUSE_MENU_SETTING,		// �ݒ�
		PAUSE_MENU_MAX
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
		SETTING_BGM,		// BGM
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

	// ������@�̃e�L�X�g���
	struct Operation {
		char Text[TXT_MAX];		// �e�L�X�g
	};

	// �ݒ���
	struct Setting {
		char Text[TXT_MAX];		// �e�L�X�g
	};

	// ���j���[���
	struct Menu {
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;

		float LeftScaleX;
		float RightScaleX;

		float LeftScaleMaxX;
		float RightScaleMaxX;

		int nCntLeftAnime;
		int nCntRightAnime;
		int nRightCoolDown;	//�@����ʏo���̃N�[���_�E��
		int nMaineSelect;
		int nMaineOldSelect;
		int nSubSelect;
		int nRightTextType;

		// ���j���[
		bool bMenu;				// �����������̃t���O
		bool bBackMode;			// �O�̉�ʂɖ߂邩�̃t���O
		bool bClose;			// ���j���[���邩�̃t���O
		bool bGameEnd;			// �Q�[���I��


		// �T�u���j���[
		bool bSubMenu;			// �����������t���O
		bool bSubMenuMove;		// �ړ������̐ؑփt���O
		bool bSubMenuDisp;		// �폜�t���O
		bool SubMenuCD;			// �����Ԋu�̃N�[���_�E��

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

		// �t�@�C��
		int MainMenuMax;
		int SubMenuMax;

		Operation *pOperation;
		Setting *pSetting;
	};


	/* *** �Ǎ��֘A  *** */

	// �e�N�X�`�����
	struct Texture {
		char TexFile[0xff];		// �t�@�C���p�X
		int PtnIdx;				// �p�^�[��	�ԍ�
		int PtnX;				//			X�̕�����
		int PtnY;				//			Y�̕�����
		bool bSet;				// �ݒ肵����
	};

	// ���C�����j���[�̏��
	struct MaineMenu {
		char Text[0xff];		// �e�L�X�g
		Texture Tex;

		int nSubMenuID;			// �T�u���j���[�̔ԍ�
	};

	// �T�u���j���[�̏��
	struct SubMenu {
		char Text[0xff];		// �e�L�X�g
		Texture Tex;
	};

	// *** �֐� ***
	CMenuUI();
	~CMenuUI();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	static CMenuUI *Create(CMode::TYPE type);

	// -- ���j���[�֘A -------------------------------------------------------------------
	/* �A�j���[�V����	*/void MenuAnime(void);
	/* ���C���̐���		*/void MenuCreate(void);
	/* �T�u�̐���		*/void SubTextCreate(void);
	/* ���j���[�I��		*/void MenuSelect(void);

	// -- ���͊֘A -----------------------------------------------------------------------
	/* ����	*/void DecisionInput(void);
	/* �I��	*/void SelectInput(void);


	// -- �e�L�X�g�֘A -------------------------------------------------------------------
	/* �e��ނ̉��	*/void TextRelease(TEXT type);

	// -- �t�@�C���֘A -------------------------------------------------------------------
	/* ���C�����j���[	*/void MaineMenuLoad(CMode::TYPE type);
	/* �T�u���j���[		*/void SubMenuLoad(void);

	// -- �擾 ---------------------------------------------------------------------------
	Menu GetInfo(void) { return m_Menu; }

	// *** �ϐ� ***
	bool m_MenuEnd;
	MaineMenu *m_MaineMenu;
	SubMenu *m_SubMenu;
	CFontText *m_pMenu[FONT_TEXT_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
private:
	
	// *** �ϐ� ***
	Menu m_Menu;
};
