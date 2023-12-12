//========================================
// 
// ���j���[
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

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
	static const char* TEXT_FILE;				// �e�L�X�g���̃t�@�C���p�X
	static const int WORDS_MAX = 7;				// �����̍ő吔
	static const int FONT_TEXT_MAX = 10;		// �e�L�X�g�̍ő吔
	static const int PAUSE_LEFT_ANIME = 15;		// ��ʍ��̃A�j���[�V��������
	static const int PAUSE_RIGHT_ANIME = 15;	// ��ʉE�̃A�j���[�V��������
	static const int VOLUME_MSX = 20;			// �T�E���h�̍ő�l
	static const int COOLDOWN = 20;				// �N�[���_�E��

	//========== [[[ �񋓌^��` ]]]
	enum MENU {
		MENU_GAME = 0,	// �Q�[��
		MENU_CONTROLLER,// ������@
		MENU_SETTING,	// �ݒ�
		MENU_END,		// �I��
		MENU_MAX
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
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
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

		Operation *pOperation;
		Setting *pSetting;
	};

	// *** �֐� ***
	CMenuUI();
	~CMenuUI();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	static CMenuUI *Create(void);

	// -- �e�L�X�g�֘A -------------------------------------------------------------------
	/* �Ǎ�			*/void TextLoad(void);
	/* �e��ނ̉��	*/void TextRelease(TEXT type);

	// -- ���j���[�֘A -------------------------------------------------------------------
	/* �A�j���[�V����	*/void MenuAnime(void);
	/* ���C���̐���		*/void MenuCreate(void);
	/* �T�u�̐���		*/void SubTextCreate(void);
	/* ���j���[�I��		*/void MenuSelect(void);

	// -- �擾 -------------------------------------------------------------------
	Menu GetInfo(void) { return m_Menu; }

	// *** �ϐ� ***
	bool m_MenuEnd;
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
private:
	
	// *** �ϐ� ***
	Menu m_Menu;
};
