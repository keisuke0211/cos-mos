//================================================================================================
// 
// �v���C���[����[player.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

#include "../Object/stage-object.h"

// �O���錾
class CStageObject;
class CMoveBlock;
class CParts;
class CRocket;
class CRoadTripLaser;
class CExtenddog;
class CPile;
class CCollision;
class CGoalGate;

// �v���C���[�N���X
class CPlayer {
public:
	// �\���ǂ���̐��E�ɑ��݂��邩
	enum class WORLD_SIDE {
		FACE = 0,	// �\
		BEHIND,		// ��
		MAX,
	};

	// �����蔻��̏��ԗ�
	enum class VECTOL {
		X,	// �]�x�N�g��
		Y,	// �x�x�N�g��
		MAX
	};

	// ����̊��蓖��
	enum class KEY_CONFIG {
		MOVE_LEFT = 0,	// ���ړ�
		MOVE_RIGHT,		// �E�ړ�
		JUMP,			// �W�����v
		SWAP,			// �X���b�v
		DECIDE,			// ����
		PAUSE,			// �|�[�Y
		MAX
	};

	// �v���C���[���
	struct Info
	{
		int      idx;
		Pos3D    StartPos;// �J�n�ʒu
		CDoll3D* doll;
		Pos3D    pos;     // �ʒu
		Pos3D    posOld;  // �O��ʒu
		Scale3D  scale;
		Pos3D    rot;     // ����
		Pos3D    move;    // �ړ���
		Color    color;   // �F

		//-------------------------------
		//�X���b�v�p
		//-------------------------------
		UShort nSwapAlpha; // �X���b�v�}�[�N�̃��l
		float  fSwapPosY;  // �X���b�v��̂x���W
		float  fSwapMoveY; // �X���b�v�ړ����̑��x
		float  fGuideTexVPos;   // �K�C�h�̃e�N�X�`���u���W
		float  fGuideTexVSize;  // �K�C�h�̃e�N�X�`���u�T�C�Y
		float  fGuideMoveSpeed; // �K�C�h�̃e�N�X�`���ړ��X�s�[�h

		//-------------------------------
		// �W�����v�p
		//-------------------------------
		bool  bGround;    // �n�ʂɐڂ��Ă��邩
		bool  bGroundOld; // �n�ʂɐڂ��Ă��邩(�ߋ�)
		short landingCounter;
		bool  bJump;      // �W�����v
		float fJumpPower; // �W�����v��
		float fGravity;   // �d��
		float fMaxHeight; // �ō��x���W

		//-------------------------------
		//����̃M�~�b�N�p
		//-------------------------------
		bool  bRide;         // ���P�b�g�ɏ���Ă��邩�ǂ���
		int   nRideInterval; //���P�b�g�ɏ��~�肵�����̃C���^�[�o��
		bool  bGoal;         // �S�[���������ǂ���
		CGoalGate *pGoalGate;
		int   nTramJumpCounter; // �g�����|�����ɂ���Ē��ˏオ�鎞��
		float fTramTargetPosY;  // �g�����|�����p�̖ڕW�ʒu
		bool  bTramJump;        // �g�����|�����p�̓���W�����v
		bool  bExtendDog;       // �k�C�p�̐ڐG�t���O
		bool  bLandPile;        // �Y�ɏ���Ă��邩�ǂ���

		//-------------------------------
		//���S�A�j���[�V�����p
		//-------------------------------
		int  expandCounter; // �c��݃J�E���^�[
		bool isDeath;       // ���S�t���O
		int  deathCounter;  // ���S�J�E���^�[
		int  deathCounter2; // ���S�J�E���^�[2
		int  DeathType;     // ���S�^�C�v

		//-------------------------------
		//SWAP�҂��̐����o���p
		//-------------------------------
		int  swapWaitCounter;

		// �ǂ���̐��E�ɑ��݂��邩
		WORLD_SIDE side;

		//-------------------------------
		//�L�[�R���t�B�O
		//-------------------------------
		int Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; // �L�[�{�[�h�̃L�[�z�u
		_RNC_Input::BUTTON JoyPad[(int)KEY_CONFIG::MAX];              // �W���C�p�b�h�̃{�^���z�u
	};

	static CFloat SIZE_WIDTH;	// ����
	static CFloat SIZE_HEIGHT;	// ����

	static CInt SWAP_INTERVAL;	// �X���b�v�C���^�[�o��
	static CInt NUM_PLAYER = 2;// �v���C���[�̐�

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//----------------------------
	// �v���C���[����
	// �v���C���[�̃|�C���^��Ԃ��܂�
	//----------------------------
	static CPlayer* Create(void);

	// �v���C���[���ݒ�
	// �w�肳�ꂽ�ԍ��̃v���C���[����ݒ肵�܂��B
	void SetInfo(const Info info, CInt nNum);

	// �v���C���[���ݒ�
	// �e�v���C���[�̈ʒu���Ȃǂ������ɓn���Ă��������B
	// �\���́uInfo�v�Ɋւ��Ă�player.h�̃N���X�錾���m�F���Ă��������B
	void SetInfo(Info p1, Info p2);

	// �v���C���[�ʒu���ݒ�
	// �w�肵���v���C���[�̈ʒu���������ɓn���Ă��������B
	void SetPos(CInt nNum, D3DXVECTOR3 pos) { m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = m_aInfo[nNum].posOld = pos; }

	// �v���C���[�F���ݒ�
	// �w�肵���v���C���[�̐F���������ɓn���Ă��������B
	void SetColor(CInt nNum, Color color) { m_aInfo[nNum].color = color; }

	// �v���C���[�Ƀg�����|�����p�̃W�����v��ݒ�
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

	// �v���C���[���擾
	// �e�����Ƀv���C���[���̃A�h���X��n���܂�
	void GetInfo(Info*& pP1, Info*& pP2) { pP1 = &m_aInfo[0]; pP2 = &m_aInfo[1]; }

	// �v���C���[���擾
	// �w�肳�ꂽ�ԍ��̃v���C���[���̃A�h���X��Ԃ��܂�
	static Info* GetInfo(int nNum) { return &m_aInfo[nNum]; }

	// �v���C���[���擾
	// �w�肳�ꂽ���E�ɂ���v���C���[�̏���Ԃ��܂�
	static Info* GetInfo(WORLD_SIDE side);

	// �X���b�v�C���^�[�o����ݒ�
	// ���ɃC���^�[�o��������ΐݒ肵�Ȃ�
	static void SetSwapInterval(void) { s_nSwapInterval = s_nSwapInterval == 0 ? SWAP_INTERVAL : s_nSwapInterval; }

	// �X���b�v�C���^�[�o�����擾
	static int GetSwapInterval(void) { return s_nSwapInterval; }

	// �X���b�v�҂��t���O���擾
	static bool GetIsSwapWait(void) { return m_aInfo[0].swapWaitCounter > 0 || m_aInfo[1].swapWaitCounter > 0; }

	//SE���x��
	enum class SE_LABEL {
		JUMP = 0,// �W�����vSE
		LANDING, // ���nSE
		DOG_00,  // ����
		DOG_01,  // �k��
		DOG_02,  // �L�т�
		DOG_03,  // �k����
		SWAPING, // �X���b�vSE
		SWAPEND, // �X���b�v�I��
		MAX
	};

	//�v���C���[�pSE�Đ�
	static void PlaySE(SE_LABEL label);

	//�p�[�e�B�N���e�N�X�`��
	enum class PARTI_TEX {
		SWAP_MARK = 0,  // �X���b�v�}�[�N
		SWAP_PARTI00,   // �X���b�v�p�[�e�B�N��00
		SWAP_PARTI01,   // �X���b�v�p�[�e�B�N��01
		SWAP_PARTI02,   // �X���b�v�p�[�e�B�N��02
		SWAP_GUIDE,     // �X���b�v�K�C�h
		CHARACTER,      // �L�����N�^�[
		DEATH_MARK,     // ���S�}�[�N
		DEATH_INK,      // ���S�C���N
		DEATH_PARTI,    // ���S�p�[�e�B�N��
		GOAL_EFFECT,    // �S�[���E���P�b�g��Ԏ��̃G�t�F�N�g
		SMOKE_EFFECT,   // ���̃G�t�F�N�g
		MAX
	};

	//�p�[�e�B�N���ԍ��擾
	static int GetParticleIdx(PARTI_TEX tex) { return s_ParticleTex[(int)tex]; };

	struct Motion {
		short neutral;
		short walk;
		short jump;
		short fall;
		short landing;
		short dance;
		short getup;
	};

	//���[�V�������擾
	static Motion GetMotion(int idx) { return s_motion[idx]; }

	//�X���b�v�����擾
	static bool GetSwapEnd(void) { return ms_bSwapEnd; }
	static bool GetSwapAnim(void) { return s_bSwapAnim; }

	// ���S�擾
	static bool GetDeath(void) { return m_aInfo[0].isDeath || m_aInfo[1].isDeath; }

	// Guide�J�E���^�[
	static UShort GetGuideCounter(void) { return ms_guideCounter; }
	static void SetGuideCounter(UShort guideCounter) { ms_guideCounter = guideCounter; }

	// �Y�[���A�b�v�J�E���^�[
	static int GetZoomUpCounter(void) { return s_zoomUpCounter; }

	static bool IsKeyConfigTrigger(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	static bool IsKeyConfigTrigger(KEY_CONFIG KeyConfig);
	static bool IsKeyConfigPress(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	static bool IsKeyConfigPress(KEY_CONFIG KeyConfig);

private:
	//��ނ̗��̂�ݒ�
	typedef CStageObject::TYPE OBJECT_TYPE;

	//****************************************************************************
	//�X���b�v�A�j���[�V�����Z�b�g���X�g
	//���ԁF�ڍׁy���̉��o�ɂ����鎞�ԕϐ��z
	//----------------------------------------------------------------------------
	//�v�����[�O�F�e�v���C���[�����ɕ�܂�A�v���C���[�������Ȃ��Ȃ�ySWAP_START_INTERVAL�z
	//   ����   �F���͒��S�ɏW�܂�A�X���b�v��܂Œ����ňړ�����ySWAP_MOVE_INTERVAL�z
	//�G�s���[�O�F�X���b�v�悩�������юU��v���C���[����������	�ySWAP_END_INTERVAL�z
	//****************************************************************************
	//�A�j���[�V�����\��
	enum class SWAP_ANIM {
		PROLOGUE = 0,	//�v�����[�O
		MIDDLE,			//����
		EPILOGUE,		//�G�s���[�O
		MAX
	};
	static CInt SWAP_PROLOGUE_INTERVAL = 10; // �X���b�v�J�n�`�ړ��܂ł̎���
	static CInt SWAP_MIDDLE_INTERVAL   = 50; // �ړ��`�ړI�n�����܂ł̎���
	static CInt SWAP_EPILOGUE_INTERVAL = 10; // �ړI�n�����`�I���܂ł̎���
	static CInt NORMAL_SWAP_ALPHA = 100;     // �ʏ펞�̃X���b�v�}�[�N�̃��l

	static CFloat GUIDE_WIDTH;      // �K�C�h�̕�
	static CFloat GUIDE_HEIGHT;     // �K�C�h�̍���
	static CFloat MAX_GUIDE_SPEED;  // �K�C�h�A�j���[�V�����̍ő呬�x

	static CInt EXPAND_TIME = 60;   // �c��݂ɂ����鎞��
	static CInt DEATH_TIME = 60;    // ���S����
	static CInt DEATH_TIME2 = 120;  // ���S����2

	static CInt SWAP_WAIT_BALLOON_TIME = 5;  // �X���b�v�҂������o������
	static SWAP_ANIM s_AnimState;        // �A�j���[�V�����\��
	static       int s_nSwapInterval;    // �c��X���b�v�C���^�[�o��
	static       bool s_bSwapAnim;       // �X���b�v�A�j���[�V���������ǂ���

	static CInt GOAL_INTERVAL = 120;// �S�[����̗]�C
	static CInt POP_CLEARTIME = 60; // �N���A�^�C���\������
	static       int s_nGoalInterval;    // �S�[����̗]�C�J�E���^�[

	static CInt ZOOM_UP_TIME       = 120; // �Y�[���A�b�v�ɂ����鎞��
	static CInt ZOOM_UP_FIXED_TIME = 90;  // �Y�[���A�b�v�̌Œ莞��
	static       int s_zoomUpCounter;          // �Y�[���A�b�v�J�E���^�[
	static       int s_zoomUpFixedCounter;    // �Y�[���A�b�v�Œ�J�E���^�[2

	void Swap(void);
	void SwapAnimation(void);
	void SwapAnim_Prologue(Info& Player, CInt nIdxPlayer); // �v�����[�O����
	void SwapAnim_Middle(Info& Player, CInt nIdxPlayer);   // ���ԏ���
	void SwapAnim_Epilogue(Info& Player, CInt nIdxPlayer); // �G�s���[�O����
	void SwapGuide(Info& Player);                          // �K�C�h�\��
	bool SwapGuideText(void);

	static const char *PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX];
	static int s_ParticleTex[(int)PARTI_TEX::MAX];

	static CFloat MOVE_SPEED;		// �ړ���
	static CFloat MAX_MOVE_SPEED;	// �ő�ړ���

	static CFloat JUMP_POWER;		// ��{�W�����v��
	static CFloat GRAVITY_POWER;	// ��{�d�͉����x

	static CInt TRAMPOLINE_JUMP_COUNTER;

	static CInt OBJ_TRAMPOLINE = 2;// �I�u�W�F�N�g�̍ő吔
	static CInt OBJ_EXTENDDOG = 3;	// �I�u�W�F�N�g�̍ő吔

	static bool ms_bSwapEnd;
	static UShort ms_guideCounter;

	void InitKeyConfig(void);// �e�v���C���[�̃L�[�{�[�h�E�W���C�p�b�h�̃L�[�R���t�B�O�������ݒ�
	void InitInfo(void);
	void ActionControl(void);
	void Move(VECTOL vec, int cntPlayer);
	void Death(Info& Player, const OBJECT_TYPE type);// ���񂾏ꏊ�������Ɏw��i���S�p�[�e�B�N���Ȃǂ�`�悷��̂Ɏg�p����

	//�S�[����̉��o
	void GoalDirector(void);

	void CollisionToStageObject(void);
	bool UniqueColliOpption(CStageObject *pObj, const OBJECT_TYPE type, Info &Player, Pos3D *pPos, Pos3D *pPosOld, float *pWidth, float *pHeight);

	// �e�v���C���[�̓����蔻�肪�I�������̏���
	void CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, CInt *pColliRot, Info& info);

	// ���X�V�����i�X�V�����̍Ō�Ɉʒu���Ȃǂ�ݒ肷��
	void UpdateInfo(void);
	void UpdateDeath(Info& info, CInt& count);

	static Info m_aInfo[NUM_PLAYER];	// �e�v���C���[�̏��

	struct SE {
		short jump;      // �W�����vSE
		short dog[4];    // �ǃM�~�b�N�pSE
		short Swaping;   // �X���b�v��SE
		short SwapEnd;   // �X���b�v�I��SE
		short expand;    // �c���
		short explosion; // �j��

		_RNC_Sound *pSound;	// �T�E���h�N���X�ۊǗp
	};
	static SE s_SE;		//�T�E���h�p�\����

	static Motion s_motion[2];

	static bool s_bAimPlayer;
	static int s_nAimNo;
	static Pos3D s_addPosV;
	static Pos3D s_addPosR;
	static float s_fCorrWidth;
	static float s_fCorrHeight;
	static float s_fAimWorkSpeed;

	int nCnt;
};