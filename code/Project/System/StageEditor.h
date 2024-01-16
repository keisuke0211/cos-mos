//========================================
// 
// �X�e�[�W�G�f�B�b�g�̓ǂݍ���
// Author:KEISUKE OTONO
// 
//========================================
#pragma once
using namespace std;

//****************************************
// �O���錾
//****************************************
class CSVFILE;

//****************************************
// �N���X��`
//****************************************
class CStageEditor {

public:

	// �萔
	static const char* STAGE_INFO_FILE;		// �X�e�[�W���̃t�@�C���p�X

	// *** ���\���� ***

	// �X�e�[�W��ޏ��
	struct StageType
	{
		int		nIdx;				// �X�e�[�W�ԍ�
		char	aFileName[0xff];	// �t�@�C���p�X
		char	aName[0xff];		// �X�e�[�W��

		int		nNumCoin;			// ����ɕK�v�ȃR�C����
		int		nLimitTime;			// ��������
		bool	bRelease;			// �X�e�[�W��������ꂽ��
	};

	// �f����ޏ��
	struct PlanetType
	{
		char	aTexFile[0xff];		// �e�N�X�`���p�X
		Color	color;				// �w�i�F
		StageType *StageType;		// �X�e�[�W���
		int nStageIdx;				// ���݂̃X�e�[�W
		int nStageMax;				// �ő�X�e�[�W
	};

	// �X�e�[�W���
	struct StageInfo
	{
		// �t�@�C���֘A
		int nRow;		// �s��
		int nLine;		// ��
		int nRowMax;	// �s���̍ő�
		int nLineMax;	// �񐔂̍ő�

		// �X�e�[�W�֘A
		D3DXVECTOR3 CameraPos;	// �J�����ʒu
		int nPlanetIdx;			// ���݂̘f��
		char	*aBgFile;		// �w�i
		char	*aSoundFile;	// �T�E���h

		// �ő�l
		int nPlanetMax;	// �ő�f��
		int nLiftMax;	// ���t�g�̍ő�
		int nMateorMax;	// 覐΂̍ő�
		int nLaserMax;	// ���[�U�[�̍ő�
		int nDogMax;	// ���̍ő�l
		int nPileMax;	// �Y�̍ő�l
	};

	// �F
	struct StageColor
	{
		Color Set;
		Color Player1;
		Color Player2;
		Color Block;
		Color FillBlock;
	};

	// ���t�g���
	struct Liftinfo
	{
		D3DXVECTOR3 posV;	// ���_�ʒu
		D3DXVECTOR3 posR;	// �I�_�ʒu
		D3DXVECTOR3 move;	// �ړ���
	};

	// 覐Ώ��
	struct MeteorInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 move;	// �ړ���
		int inteval;		// �����Ԋu
	};

	// ���[�U�[���
	struct LaserInfo
	{
		D3DXVECTOR3 posV;	// ���_�ʒu
		D3DXVECTOR3 posR;	// �I�_�ʒu
		D3DXVECTOR3 move;	// �ړ���
	};

	// Dog���
	struct DogInfo
	{
		D3DXVECTOR3 HeadPos;// ���̈ʒu
		D3DXVECTOR3 HipPos;	// �K�̈ʒu
		int HeightMin;		// ���̂̍��� �Œ�l
		int HeightMax;		// �@�@�@�@�@ �ő�l
		bool bElasticity;	// �L�k�t���O
	};

	// �Y�̏��
	struct PileInfo
	{
		D3DXVECTOR3 pos;	// �ʒu
		int nNumPile;		// �Y�̐�
		float fCaveIn;		// ���肱�ݗ�
	};


	// *** �֐��錾 ***
	CStageEditor();
	~CStageEditor();
	/* �I��*/ void Uninit();

	/* �X�e�[�W�؂�ւ�	*/void SwapStage(int nStageIdx);

	// -- �擾 ---------------------------------------------
	/* �X�e�[�W���		*/void SetStageRel(int planet, int stage,bool bRel);


	// -- �擾 ---------------------------------------------
	/* ����̃R�C����	*/int GetStageCoin(int planet, int stage);
	/* ��������      	*/int GetStageLimittime(int planet, int stage);
	/* �X�e�[�W���		*/bool GetStageRel(int planet, int stage);
	/* �s���̍ő�l		*/int GetRowMax(void) { return m_Info.nRowMax; }
	/* �񐔂̍ő�l		*/int GetLineMax(void) { return m_Info.nLineMax; }
	/* �ő�l			*/int GetPlanetMax(void) { return m_Info.nPlanetMax; }
	/* ���݂̃X�e�[�W	*/int GetPlanetIdx(void) { return m_Info.nPlanetIdx; }
	/* �f����ޏ��		*/PlanetType *GetType(void) { return m_PlanetType; }
	/* �J�����̏����ʒu	*/D3DXVECTOR3 GetCameraPos(void) { return m_Info.CameraPos; }
	/* �w�i				*/char *GetBgFile(void) { return m_Info.aBgFile; }
	/* �T�E���h			*/char *GetSoundFile(void) { return m_Info.aSoundFile; }
	/* �ʒu				*/D3DXVECTOR3 GetPos(int nRow, int nLine);
	/* �X�e�[�W�����擾 */void GetPlanetAndStageMax(CInt planet, int& NumPlanet, int& NumStage);

	// -- �ϊ� ---------------------------------------------
	/* int		*/bool ToData(int &val, CSVFILE *pFile, int nRow, int nLine);
	/* float	*/bool ToData(float &val, CSVFILE *pFile, int nRow, int nLine);
	/* ushort	*/bool ToData(UShort& val, CSVFILE* pFile, int nRow, int nLine);
	/* double	*/bool ToData(double &val, CSVFILE *pFile, int nRow, int nLine);

	// -- �Ǎ� ---------------------------------------------
	/* �t�@�C���p�X	*/void FileLoad(void);
	/* �X�e�[�W�Ǎ�	*/void StageLoad(int planet,int stage);

private:

	// *** �񋓌^ ***
	enum Type
	{
		TYPE_BLOCK         = 0,		// �u���b�N
		TYPE_TRAMPOLINE    = 1,		// �g�����|����
		TYPE_SPIKE         = 2,		// ��
		TYPE_SPIKE_L	   = 24,	// ����
		TYPE_SPIKE_R	   = 25,	// ���E
		TYPE_LIFT		   = 3,		// ���t�g
		TYPE_Meteor        = 4,		// 覐�
		TYPE_Laser         = 5,		// ���[�U�[
		TYPE_Extenddog     = 6,		// �k�C
		TYPE_COIN          = 7,		// �R�C��
		TYPE_MAGMA_BLOCK   = 8,		// �}�O�}�u���b�N
		TYPE_FILL_BLOCK_11 = 11,	// �u���b�N(���� ��) 1 * 1
		TYPE_FILL_BLOCK_12 = 12,	// �u���b�N(���� ��) 1 * 2
		TYPE_FILL_BLOCK_13 = 13,	// �u���b�N(���� ��) 1 * 3
		TYPE_FILL_BLOCK_21 = 21,	// �u���b�N(���� ��) 2 * 1
		TYPE_FILL_BLOCK_22 = 22,	// �u���b�N(���� ��) 2 * 2
		TYPE_FILL_BLOCK_31 = 31,	// �u���b�N(���� ��) 3 * 1
		TYPE_FILL_BLOCK_33 = 33,	// �u���b�N(���� ��) 3 * 3
		TYPE_FILL_BLOCK_44 = 44,	// �u���b�N(���� ��) 4 * 4
		TYPE_PLAYER_0      = 90,	// �P�o
		TYPE_PLAYER_1      = 91,	// �Q�o
		TYPE_GOALGATE      = 97,	// �S�[���Q�[�g
		TYPE_PARTS         = 98,	// �p�[�c
		TYPE_GOAL          = 99,	// �S�[��
		TYPE_MAX
	};

	// *** �֐��錾 ***
	/* �X�e�[�W�F	*/void StgColor(CSVFILE *pFile, int nRow, int nLine);
	/* �F�ݒ�		*/void SetColor(CSVFILE *pFile, int nRow, int nLine);
	/* ���t�g�ݒ�	*/void SetLiftInfo(CSVFILE *pFile, int nRow, int nLine);
	/* 覐ΐݒ�		*/void SetMeteorInfo(CSVFILE *pFile, int nRow, int nLine);
	/* ���[�U�[�ݒ�	*/void SetLaserInfo(CSVFILE *pFile, int nRow, int nLine);
	/* �k�C�ݒ�		*/void SetDogInfo(CSVFILE *pFile, int nRow, int nLine);
	/* �Y�̐ݒ�		*/void SetPileInfo(CSVFILE *pFile, int nRow, int nLine);
	/* OBJ�z�u		*/void ObjPlace(float fSizeX, float fSizeY, D3DXVECTOR3 pos,int nType);

	/* �X�e�[�W���� */void SetStage(int nType);

	// �������J��
	// �f�����܂ŊJ�����邩�ǂ����itrue�ŊJ��
	void Release(bool bPlanetRelease = false);

	// *** �ϐ��錾 ***
	static PlanetType *m_PlanetType;	// �f�����
	static StageColor m_StageColor;		// �F���
	Liftinfo *m_LiftInfo;				// ���t�g���
	MeteorInfo *m_MeteorInfo;			// 覐Ώ��
	LaserInfo *m_LaserInfo;				// ���[�U�[���
	DogInfo *m_DogInfo;					// ���̏��
	PileInfo *m_PileInfo;				// �Y�̏��

	StageInfo m_Info;					// �X�e�[�W���

	bool bRocket;
};
