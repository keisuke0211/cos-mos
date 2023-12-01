//========================================
// 
// ���[�h:�Q�[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// �O���錾
//****************************************
class CPlayer;
class CFontText;
class CRocketPartsUI;

//****************************************
// �N���X��`
//****************************************
// ���[�h(�Q�[��)�N���X
class CMode_Game :public CMode {
public:
	//========== [[[ �萔��` ]]]
	static const char* TEXT_FILE;				// �e�L�X�g���̃t�@�C���p�X
	static const int PAUSE_LEFT_ANIME  = 20;	// ��ʍ��̃A�j���[�V��������
	static const int PAUSE_RIGHT_ANIME = 20;	// ��ʉE�̃A�j���[�V��������
	static const int FONT_TEXT_MAX = 8;			// �e�L�X�g�̍ő吔
	static const int VOLUME_MSX = 20;			// �T�E���h�̍ő�l
	static const int COOLDOWN = 20;				// �N�[���_�E��

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	// ���j���[
	enum MENU {
		MENU_RESUME = 0,// ������
		MENU_RESET,		// ��蒼��
		MENU_SELECT,	// �X�e�[�W�I��
		MENU_CONTROLLER,// ������@
		MENU_SETTING,	// �ݒ�
		MENU_MAX
	};

	// ������@
	enum CONTROLLER {
		INPUT_TITLE = 0,// �^�C�g��
		INPUT_MOVE,		// �ړ�
		INPUT_JUMP,		// �W�����v
		INPUT_SWAP,		// �X���b�v
		INPUT_DECISION,	// ����
		INPUT_BACK,		// �߂�
		INPUT_MAX
	};

	// �ݒ�
	enum SETTING {
		SETTING_SCREEN = 1,	// �t���X�N���[��
		SETTING_BGM,		// BGM
		SETTING_SE,			// SE
		SETTING_BACK,
		SETTING_BGM_TEXT,
		SETTING_SE_TEXT,
		SETTING_MAX
	};

	// �e�L�X�g
	enum TEXT {
		TEXT_MENU = 0,	// ���j���[
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

	// �|�[�Y���
	struct Pause {
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

	//========== [[[ �֐��錾 ]]]
	static CPlayer* GetPlayer(void);
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);
	// -- �ݒ� -------------------------------------------------------------------
	static void SetStage(int planet, int stage) { m_nPlanetIdx = planet; m_nStageIdx = stage; }
	static void SetBgUpColor(Color color) { m_BgColorUp = color; }
	static void SetBgDownColor(Color color) { m_BgColorDown = color; }
	static void SetRocketParts(CRocketPartsUI *rctpar) { m_rocketparts = rctpar; }
	// -- �擾 -------------------------------------------------------------------
	static int GetStage(void) { return m_nStageIdx; }

private:
	//========== [[[ �֐��錾 ]]]
	void PauseCreate(void);
	void PauseSelect(void);
	void PauseAnime(void);
	void PauseMenu(void);
	void SubTextCreate(void);
	void TextLoad(void);
	void TextRelease(TEXT type);

	//========== [[[ �ϐ��錾 ]]]
	static CPlayer *s_pPlayer;
	static int m_nPlanetIdx;
	static int m_nStageIdx;
	CFontText *m_pMenu[MENU_MAX];
	CFontText *m_pSubMenu[FONT_TEXT_MAX];
	static CRocketPartsUI *m_rocketparts;
	static Color m_BgColorUp, m_BgColorDown;
	short m_wallModelIdx[2];
	Pause m_Pause;
	short m_BGMIdx;
	CSound::CPlay* play;
};