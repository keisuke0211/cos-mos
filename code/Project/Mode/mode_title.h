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
class CMenuUI;
class CFontText;
class CWords;
class CCoinUI;

//****************************************
// �N���X��`
//****************************************
// ���[�h(�^�C�g��)�N���X
class CMode_Title :public CMode{
public:
	//========== [[[ �萔��` ]]]
	static const char* TITLE_LOGO_FILE;				// �^�C�g�����S���̃t�@�C���p�X
	static const int TITLE_LOGO_MAX = 6;			// �^�C�g�����S�̍ő吔
	static const int NUI_ANIME = 20;				// �k�C�̃A�j���[�V��������
	static const int TITLE_TEXT_ANIME = 40;			// �^�C�g���e�L�X�g�̃A�j���[�V��������
	static const int COLOR_CHANGE_TIME = 60;		// �F�̐��ڎ���
	static const int TEXE_ANIME_STAND_TIME = 120;	// TextAnime�̑ҋ@����
	static const int TEXE_ANIME_INTERVAL_TIME = 60;	// TextAnime�̑ҋ@����

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
	static void SetSelect(bool bSelect) { m_bStageSelect = bSelect; }
	CMode_Title();
	~CMode_Title();
	void  Init(void);
	void  Uninit(void);
	void  Update(void);
	void  ProcessState(const PROCESS process);

	static int m_nPlanetIdx;
	static int m_nStageSelect;

private:
	//========== [[[ �񋓌^��` ]]]
	enum TEX{
		TEX_SPACE = 0,	// �F��
		TEX_NUM,		// ����
		TEX_LOCK,		// ���O
		TEX_MAX
	};
	enum TEXT {
		TEXT_MENU = 0,	// ���j���[
		TEXT_ALL,		// �S��
		TEXT_MAX
	};
	enum STAGE {
		NONE = 0,	// �����Ȃ�
		POP,		// �o��
		DESPAWN		// ����
	};

	enum TITLE_ANIME {
		ANIME_NUI = 0,	// �k�C�̏o��
		ANIME_TEXT1,	// �e�L�X�g�̏o��
		ANIME_NUI_LEAN,	// �k�C���X����
		ANIME_TM,		// TM
		ANIME_NONE,		// ����
	};

	enum TEXT_ANIME {
		TEXT_IN = 0,	// �o��
		TEXT_STAND,		// �ҋ@
		TEXT_OUT,		// ����
		TEXT_INTERVAL,	// �Ԋu
	};

	enum TITLE_LOGO {
		LOGO_COS1 = 0,	// COS
		LOGO_MOS1,		// MOS
		LOGO_NUI,		// �k�C
		LOGO_COS2,		// �R�X
		LOGO_MOS2,		// ���X
		LOGO_TM,		// TM
	};

	enum DIRECTION {
		RIGHT = 0,	// �E
		LEFT,		// ��
	};
	//========== [[[ �\���̒�` ]]]
	// �X�e�[�W��ޏ��
	struct PlanetType{
		int nModel;				// ���f��
	};

	// �^�C�g�����S��� 
	struct TitleLogo {
		D3DXVECTOR3 InitPos;// �����ʒu
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR2 size;	// �T�C�Y
		Color color;		// �F
		int	TexIdx;			// �e�N�X�`���ԍ�
		float angle;		// ����

		Pos2D VtxPos[4];	// ���_�ʒu
		Pos2D TexUV[4];		// �e�N�X�`��UV
	};

	// �^�C�g�����S�̋��ʏ��
	struct TitleLogoInfo {
		TitleLogo Logo[TITLE_LOGO_MAX];	// ���S���
		int nCntAnime;					// �A�j���[�V�����J�E���^�[
	};

	// �e�L�X�g�A�j���[�V�����̏��
	struct TextAnimeInfo {
		TEXT_ANIME state = TEXT_IN;	// ���
		int StandTime;				// �ҋ@����
		int Interval;				// �Ԋu
		int nLetterPopCount;		// ���̕���
		int nAppearTime;			// ���̕������ړ�����܂ł̎���
		bool bOut;					// ���Ńt���O
	};

	// *** �֐� ***
	void ColorChange(void);
	void MenuAnime(void);
	void TitleAnime(void);
	void TextAnime(void);
	void CreateStageSelectInfo(void);
	void StageSelect(void);
	void StageDraw(int nPlanet, int nStage, D3DXVECTOR3 poscor,float &RktAnimRt);
	void StagePop(int nPlanet,int &nStage,D3DXVECTOR3 poscor);
	void TextRelease(TEXT type);
	void SwapMode(TITLE aTitle);
	void StageRel(int nPlanet, int nStage);
	void TitleLoad(void);
	void TitleLogoInit(bool bSwitch);

	// *** �ÓI�ϐ� ***
	static CMenuUI *m_MenuUI;
	static bool m_bStageSelect;

	// *** �ϐ� ***
	TITLE Title;
	TITLE NextTitle;
	TITLE_ANIME TitleAnima;

	Color BgColor;
	Color BgOldColor;
	Color BgNextColor;
	int nCntColorChange;
	bool bColorChange;

	D3DXVECTOR3 m_BgPos[TEX_MAX];
	Pos2D m_BgTexPthPos[4];
	Pos2D m_BgTexPthMove[4];
	D3DXVECTOR3 m_RocketPos;
	D3DXVECTOR3 m_RocketposDiff;
	D3DXVECTOR3 m_RocketPosOld;
	D3DXVECTOR3 m_RocketRot;
	D3DXVECTOR3 m_RocketRotOld;
	D3DXVECTOR3 m_RocketRotDiff;
	D3DXVECTOR3 m_rotEff;
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_MapIdx;
	int m_EffTex[3];
	int m_nSelectTemp;
	int m_nOldSelect;
	int m_nOldnPlanet;
	int m_nDrawPlanet;
	int m_RocketIdx;
	int m_SelIdx;
	int m_WldBoardIdx[3];
	int m_CoinBoardIdx;
	int m_ArrowIdx;
	int *m_AnimCnt;
	int m_RocketAnimCnt;
	int m_NumAnimCnt;
	int m_ImageStgCnt;
	int m_RotCnt;
	int m_nCnt;
	int m_nStgStartCnt;
	bool m_bBackMode;
	bool m_bWorldChange;
	bool m_bStageChange;
	bool m_bRocketMove;
	bool m_bRocketRot;
	bool m_bRotDir;
	bool m_bStgEnter;

	TextAnimeInfo m_TextAnime;	// �e�L�X�g�A�j���[�V����

	TitleLogoInfo m_TitleLogo;
	CFontText *m_pText;
	PlanetType *m_PlanetType;
	STAGE m_StgFlag;
	CCoinUI *m_CoinUI;
	CRail3D m_RocketRail;
	DIRECTION m_Direction;
};