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
	static const int WORDS_MAX = 7;		// �����̍ő吔

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
		TEX_NUM,		// ����
		TEX_MAX
	};
	enum TEXT {
		TEXT_TITLE = 0,	// �^�C�g��
		TEXT_MENU,		// ���j���[
		TEXT_ALL,		// �S��
		TEXT_MAX
	};
	enum STAGE {
		NONE = 0,	// �����Ȃ�
		POP,		// �o��
		DESPAWN		// ����
	};

	//========== [[[ �\���̒�` ]]]
	// �X�e�[�W��ޏ��
	struct PlanetType{
		int nModel;				// ���f��
	};

	// *** �֐� ***
	void MenuAnime(void);
	void TextAnime(void);
	void CreateStageSelectInfo(void);
	void StageSelect(void);
	void StageDraw(int nPlanet, int nStage, D3DXVECTOR3 poscor,float &RktAnimRt);
	void StagePop(int nPlanet,int &nStage,D3DXVECTOR3 poscor);
	void TextRelease(TEXT type);
	void SwapMode(TITLE aTitle);

	// *** �ÓI�ϐ� ***
	static CMenuUI *m_MenuUI;
	static bool m_bStageSelect;

	// *** �ϐ� ***
	TITLE Title;
	TITLE NextTitle;
	D3DXVECTOR3 m_BgPos[TEX_MAX];
	D3DXVECTOR3 m_RocketPos;
	D3DXVECTOR3 m_RocketposDiff;
	D3DXVECTOR3 m_RocketPosOld;
	D3DXVECTOR3 m_RocketRot;
	D3DXVECTOR3 m_RocketRotOld;
	D3DXVECTOR3 m_RocketRotDiff;
	float m_PlanetAngle;
	int m_TexIdx[TEX_MAX];
	int m_EffTex;
	int m_nSelectTemp;
	int m_nOldSelect;
	int m_nOldnPlanet;
	int m_nDrawPlanet;
	int m_RocketIdx;
	int m_SelIdx;
	int m_StgBoardIdx;
	int m_CoinBoardIdx;
	int m_ArrowIdx;
	int *m_AnimCnt;
	int m_RocketAnimCnt;
	int m_NumAnimCnt;
	int m_ImageStgCnt;
	int m_RotCnt;
	int m_nCnt;
	bool m_bMove[WORDS_MAX];
	bool m_bBackMode;
	bool m_bStageChange;
	bool m_bRocketMove;
	bool m_bRocketRot;
	bool m_bRotDir;
	CWords *m_TITLE[WORDS_MAX];
	CWords *m_TitleShadow[WORDS_MAX];
	CFontText *m_pMenu[MENU_MAX];
	PlanetType *m_PlanetType;
	STAGE m_StgFlag;
	CCoinUI *m_CoinUI;
};