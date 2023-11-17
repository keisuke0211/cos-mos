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
	// *** ��` ***
	static const int PAUSE_LEFT_ANIME = 20;			// ��ʍ��̃A�j���[�V��������
	static const int PAUSE_RIGHT_ANIME = 20;		// ��ʉE�̃A�j���[�V��������

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	// ���j���[
	enum MENU
	{
		MENU_RESUME = 0,// ������
		MENU_RESET,		// ��蒼��
		MENU_SELECT,	// �X�e�[�W�I��
		MENU_CONTROLLER,// ������@
		MENU_SETTING,	// �ݒ�
		MENU_MAX
	};

	// ������@
	enum CONTROLLER
	{
		INPUT_TITLE = 0,// �^�C�g��
		INPUT_MOVE,		// �ړ�
		INPUT_JUMP,		// �W�����v
		INPUT_SWAP,		// �X���b�v
		INPUT_DECISION,	// ����
		INPUT_BACK,		// �߂�
		INPUT_MAX
	};

	// �e�L�X�g
	enum TEXT
	{
		TEXT_MENU = 0,	// ���j���[
		TEXT_INPUT,		// ������@
		TEXT_ALL,		// �S��
		TEXT_MAX
	};

	// �|�[�Y���
	struct Pause
	{
		D3DXVECTOR3 LeftPos;
		D3DXVECTOR3 RightPos;
		D3DXVECTOR3 LeftTargetPos;
		D3DXVECTOR3 RightTargetPos;
		int nCntLeftAnime;
		int nCntRightAnime;
		int nSelect;
		int nRightTextType;
		bool bMenu;
		bool bRightMove;
		bool bRightDisp;
		bool bClose;

		int BoxTex;
	};

	//========== [[[ �֐��錾 ]]]
	      CMode_Game    ();
	      ~CMode_Game   ();
	void  Init          (void);
	void  Uninit        (void);
	void  Update        (void);
	void  ProcessState  (const PROCESS process);

	static CPlayer* GetPlayer(void);

	// -- �ݒ� -------------------------------------------------------------------
	static void SetStage(int planet, int stage) { m_nPlanetIdx = planet; m_nStageIdx = stage; }
	static void SetBgUpColor(Color color) { m_BgColorUp = color; }
	static void SetBgDownColor(Color color) { m_BgColorDown = color; }

	// -- �擾 -------------------------------------------------------------------
	static int GetStage(void)    { return m_nStageIdx;   }
	static CCamera& GetCameraUp(void)   { return *m_cameraUp;   }
	static CCamera& GetCameraDown(void) { return *m_cameraDown; }

private:

	//========== [[[ �֐��錾 ]]]
	void PauseCreate(void);
	void PauseSelect(void);
	void PauseAnime(void);
	void PauseMenu(void);
	void InputText(void);
	void TextRelease(TEXT type);

	//========== [[[ �ϐ��錾 ]]]
	static CPlayer *s_pPlayer;
	static int m_nPlanetIdx;
	static int m_nStageIdx;
	CFontText *m_Menu[MENU_MAX];
	CFontText *m_RightText[INPUT_MAX];
	CRocketPartsUI *m_rocketparts;
	static CCamera* m_cameraUp;
	static CCamera* m_cameraDown;
	static Color m_BgColorUp, m_BgColorDown;
	short m_wallModelIdx[2];
	Pause m_Pause;

	short m_BGMIdx;

	CSound::CPlay* play;


	char m_RightTxt[INPUT_MAX][TXT_MAX] = {
		{"������@"},
		{"�E�ړ��@�@�F���X�e�B�b�N"},
		{"�E�W�����v�F�a"},
		{"�E�X���b�v�F�x"},
		{"�E����@�@�F�`"},
		{"�E�߂�@�@�F�a"},
	};

};