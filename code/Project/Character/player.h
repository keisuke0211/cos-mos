//================================================================================================
//
//�v���C���[����[player.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�O���錾
class CStageObject;
class CMoveBlock;
class CParts;
class CRocket;
class CRoadTripLaser;
class CExtenddog;

//�v���C���[�N���X
class CPlayer
{
public:
	//�\���ǂ���̐��E�ɑ��݂��邩
	enum class WORLD_SIDE	{
		FACE = 0,	//�\
		BEHIND,		//��
		MAX,
	};

	//�����蔻��̏��ԗ�
	enum class COLLI_VEC {
		X = 0,	//�]�x�N�g��
		Y,		//�x�x�N�g��
		MAX
	};

	//�����蔻�肪����������
	enum class COLLI_ROT {
		NONE = 0,	//�����Ȃ�
		OVER,		//��
		UNDER,		//��
		LEFT,		//��
		RIGHT,		//�E
		UNKNOWN,	//�������Ă��邯�Ǖ�����������Ȃ��i�������鑤�������Ă���\���A��
		MAX
	};

	//����̊��蓖��
	enum class KEY_CONFIG {
		MOVE_LEFT = 0,	//���ړ�
		MOVE_RIGHT,		//�E�ړ�
		JUMP,			//�W�����v
		SWAP,			//�X���b�v
		DECIDE,			//����
		PAUSE,			//�|�[�Y
		MAX
	};

	//�����蔻����
	struct Colli
	{
		D3DXVECTOR3 pos;        //�@�ʒu
		D3DXVECTOR3 posOLd;     //�@�O��ʒu
		D3DXVECTOR3 MinPos;		// �ŏ��ʒu
		D3DXVECTOR3 MaxPos;		// �ő�ʒu
		float		fWidth;		// ��
		float		fHeight;	// ����

		COLLI_ROT ColliRot;		// �����蔻��
	};

	//�v���C���[���
	struct Info
	{
		D3DXVECTOR3 StartPos;     //�J�n�ʒu

		D3DXVECTOR3 pos;          //�ʒu
		D3DXVECTOR3 posOLd;       //�O��ʒu
		D3DXVECTOR3 rot;          //����
		D3DXVECTOR3 move;         //�ړ���
		Color		color;		  //�F
		bool		bGround;      //�n�ʂɐڂ��Ă��邩
		bool		bJump;        //�W�����v
		bool		bRide;        //���P�b�g�ɏ���Ă��邩�ǂ���
		bool		bGoal;		  //�S�[���������ǂ���
		float		fJumpPower;   //�W�����v��
		float		fGravity;     //�d��
		float		fMaxHeight;   //�ō��x���W
		int			nTramJumpCounter;	//�g�����|�����ɂ���Ē��ˏオ�鎞��
		float		fTramTargetPosY;	//�g�����|�����p�̖ڕW�ʒu
		bool		bTramJump;			//�g�����|�����p�̓���W�����v
		COLLI_ROT	TramColliRot;		//�g�����|�����p�̓����蔻��
		bool		bExtendDog;	  //�k�C�p�̐ڐG�t���O
		int			nModelIdx;    //���f���ԍ�
		WORLD_SIDE  side;         //�ǂ���̐��E�ɑ��݂��邩
		int             Keyborad[(int)WORLD_SIDE::MAX][(int)KEY_CONFIG::MAX]; //�L�[�{�[�h�̃L�[�z�u
		CInput::BUTTON  JoyPad[(int)KEY_CONFIG::MAX];                         //�W���C�p�b�h�̃{�^���z�u
	};

	static const float SIZE_WIDTH;	//����
	static const float SIZE_HEIGHT;	//����

	static const int SWAP_INTERVAL;	//�X���b�v�C���^�[�o��
	static const int NUM_PLAYER = 2;//�v���C���[�̐�

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//----------------------------
	//�v���C���[����
	//�v���C���[�̃|�C���^��Ԃ��܂�
	//----------------------------
	static CPlayer *Create(void);

	//�v���C���[���ݒ�
	//�w�肳�ꂽ�ԍ��̃v���C���[����ݒ肵�܂��B
	void SetInfo(const Info info, const int nNum);

	//�v���C���[���ݒ�
	//�e�v���C���[�̈ʒu���Ȃǂ������ɓn���Ă��������B
	//�\���́uInfo�v�Ɋւ��Ă�player.h�̃N���X�錾���m�F���Ă��������B
	void SetInfo(Info p1, Info p2);

	//�v���C���[�ʒu���ݒ�
	//�w�肵���v���C���[�̈ʒu���������ɓn���Ă��������B
	void SetPos(const int nNum, D3DXVECTOR3 pos) { m_aInfo[nNum].StartPos = m_aInfo[nNum].pos = pos; }

	//�v���C���[�F���ݒ�
	//�w�肵���v���C���[�̐F���������ɓn���Ă��������B
	void SetColor(const int nNum, Color color) { m_aInfo[nNum].color = color; }

	//�v���C���[�Ƀg�����|�����p�̃W�����v��ݒ�
	void SetTrampolineJump(Info*& pInfo, float fMaxHeight);

	//�v���C���[���擾
	//�e�����Ƀv���C���[���̃A�h���X��n���܂�
	void GetInfo(Info*& pP1, Info*& pP2) { pP1 = &m_aInfo[0]; pP2 = &m_aInfo[1]; }

	//�v���C���[���擾
	//�w�肳�ꂽ�ԍ��̃v���C���[���̃A�h���X��Ԃ��܂�
	Info *GetInfo(int nNum) { return &m_aInfo[nNum]; }

	//�v���C���[���擾
	//�w�肳�ꂽ���E�ɂ���v���C���[�̏���Ԃ��܂�
	Info *GetInfo(WORLD_SIDE side);

	//�擾�����p�[�c��
	int GetNumParts(void) { return s_nNumGetParts; }

	//�X���b�v�C���^�[�o����ݒ�
	//���ɃC���^�[�o��������ΐݒ肵�Ȃ�
	static void SetSwapInterval(void) { s_nSwapInterval = s_nSwapInterval == 0 ? SWAP_INTERVAL : s_nSwapInterval; }

private:
	static int	s_nSwapInterval;//�c��X���b�v�C���^�[�o��

	static const float MOVE_SPEED;		//�ړ���
	static const float MAX_MOVE_SPEED;	//�ő�ړ���

	static const float JUMP_POWER;		//��{�W�����v��
	static const float GRAVITY_POWER;	//��{�d�͉����x

	static const int TRAMPOLINE_JUMP_COUNTER;
	static int	s_nNumGetParts;	//�擾�����p�[�c�̐�
	static bool	s_bRideRocket;	//���P�b�g�ɏ��邩�ǂ���

	static const int OBJ_TRAMPOLINE = 2;// �I�u�W�F�N�g�̍ő吔
	static const int OBJ_EXTENDDOG = 3;	// �I�u�W�F�N�g�̍ő吔

	void InitKeyConfig(void);//�e�v���C���[�̃L�[�{�[�h�E�W���C�p�b�h�̃L�[�R���t�B�O�������ݒ�
	void SetPosOld(void);
	void ActionControl(void);
	void Move(COLLI_VEC vec);
	void Swap(void);
	void Death(D3DXVECTOR3 *pDeathPos);//���񂾏ꏊ�������Ɏw��i���S�p�[�e�B�N���Ȃǂ�`�悷��̂Ɏg�p����

	void WholeCollision(void);

	void CntrPos(void);	// �͈͊O�̐���

	//========================
	//�Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
	//------------------------
	// �����P	pos			�F���݈ʒu
	// �����Q	posOld		�F�O��ʒu
	// �����R	fWidth		�F��
	// �����S	fHeight		�F����
	// �����T	TargetPos	�F�Ώۂ̌��݈ʒu
	// �����U	TargetPosOld�F�Ώۂ̑O��ʒu�i�I�u�W�F�N�g��PosOld�ϐ��������Ȃ�A���݈ʒu��������OK
	// �����V	TargetWidth	�F�Ώۂ̕�
	// �����W	TargetHeight�F�Ώۂ̍���
	// �����X	value		�F�x�N�g��
	// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
	//========================
	COLLI_ROT IsBoxCollider(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetPosOld, float TargetWidth, float TargetHeight, COLLI_VEC value);

	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY,float fHeight);	//�ォ��̓����蔻��ɂ��ʒu�E�ړ��ʏC��
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);	//������̓����蔻��ɂ��ʒu�E�ړ��ʏC��
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);	//������̓����蔻��ɂ��ʒu�E�ړ��ʏC��
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);	//�E����̓����蔻��ɂ��ʒu�E�ړ��ʏC��

	void CollisionBlock(Info *pInfo, Colli *pColli);
	void CollisionFillBlock(Info *pInfo,COLLI_ROT ColliRot);
	void CollisionTrampoline(Info *pInfo, Colli *pColli, Colli *pOthColli);
	void CollisionSpike(Info *pInfo, Colli *pColli);
	void CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, Colli *pColli);
	void CollisionMeteor(Info *pInfo, Colli *pColli);
	void CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, Colli *pColli, Colli *pOthColli);
	void CollisionDog(Info *pInfo, CExtenddog *pExtenddog, Colli *pColli, Colli *pOthColli);
	void CollisionGoalGate(Info *pInfo, Colli *pColli);
	void CollisionParts(Info *pInfo, CParts *pParts);
	void CollisionRocket(Info *pInfo, CRocket *pRocket);

	bool IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);
	bool IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig);

	//���X�V�����i�X�V�����̍Ō�Ɉʒu���Ȃǂ�ݒ肷��
	void UpdateInfo(void);

	void OthColliDelete(void);

	Info m_aInfo[NUM_PLAYER];	//�e�v���C���[�̏��
	Colli *pOthColli;			//���p�[�c�̓����蔻����
	bool bPluralColli;			//�p�[�c��ށ@�P�̂�������
	static int s_nSwapMarkTex;  //�X���b�v��̃}�[�N�e�N�X�`���ԍ�
	static int s_nSwapParticle; //�X���b�v���̃p�[�e�B�N���e�N�X�`���ԍ�
	short m_jumpSEIdx;			//�W�����v����SE�ԍ�
	short m_landingSEIdx;		//���n����SE�ԍ�
	short m_dogSEIdx[4];		//�ǃM�~�b�N�pSE�ԍ�
};

#endif