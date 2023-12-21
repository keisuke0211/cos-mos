//========================================
// 
// �u���b�N����
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// �N���X
//****************************************
class CBlock : public CStageObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class LOOKS_TYPE {
		// �����ڂ̎��
		SOIL_BLOCK					       = 0,		// �y�u���b�N
		SOIL_BLOCK_GRASSY			       = 1,		// ���̐������y�u���b�N
		ANCIENT_STONE_BLOCK			       = 2,		// �Ñ�̐΃u���b�N
		ANCIENT_STONE_BLOCK_PTN_A	       = 3,		// �Ñ�̐΃u���b�NA
		ANCIENT_STONE_BLOCK_PTN_B	       = 4,		// �Ñ�̐΃u���b�NB
		ANCIENT_STONE_BLOCK_PTN_C	       = 5,		// �Ñ�̐΃u���b�NC
		ANCIENT_STONE_PILLAR_UP		       = 6,		// ���̏�
		ANCIENT_STONE_PILLAR_CENTER	       = 7,		// ���̌q��
		ANCIENT_STONE_PILLAR_DOWN	       = 8,		// ���̍��{
		LEAF_BLOCK	                       = 9,		// ���u���b�N
		TREE_TRUNK	                       = 10,	// �؂̊�
		TREE_TRUNK_ROOT                    = 11,	// �؂̍���
		BAOBAB_TREE					       = 12,	// �o�I�o�u�̖�(�w�i�p)
		CHEST                              = 13,	// ��
		PILE_OF_COINS                      = 14,	// �R�ς݂̃R�C��
		STONE_MONUMENT                     = 15,	// ��̐Δ�
		ANCIENT_STONE_BRICK_BLOCK          = 16,	// �Ñ�̐΃����K�u���b�N
		SOIL_AND_ANCIENT_STONE_BLOCK       = 17,	// �y���Ñ�̐΃u���b�N(���ԗp)
		PALMTREE					       = 18,	// ���V�̖�
		TREE_EYES_BLOCK				       = 19,	// �O�ڂ̃u���b�N
		LEAF_BLOCK_NUTS                    = 20,	// ���u���b�N(�؂̎�)
		NUI_HOUSE_BLOCK                    = 21,	// �k�C�̉ƃu���b�N
		ASPHALT_BLOCK				       = 22,	// �A�X�t�@���g�u���b�N
		ASPHALT_BLOCK_CRACKED		       = 23,	// �Ђъ��ꂽ�A�X�t�@���g�u���b�N
		IRON_BAR					       = 24,	// �S�i�q(�����蔻�薳���z��)
		IRON_BAR_1					       = 25,	// �S�i�q1
		IRON_BAR_2					       = 26,	// �S�i�q2
		IRON_BAR_3					       = 27,	// �S�i�q3
		MUD_BLOCK					       = 28,	// �D�u���b�N
		SAND_AND_ASPHALT_BLOCK             = 29,	// �����A�X�t�@���g�u���b�N(���ԗp)
		SAND_BLOCK					       = 30,	// ���u���b�N
		TORCH						       = 31,	// ����
		SNAKE						       = 32,	// �ւ̕ǉ�
		LEAF_LARGE					       = 33,	// �ΐF�̗t����(��)
		LEAF_SMALL					       = 34,	// �ΐF�̗t����(��)
		DRY_LEAF_LARGE				       = 35,	// ���F�̗t����(��)
		DRY_LEAF_SMALL				       = 36,	// ���F�̗t����(��)
		STONE_DRAGON				       = 37,	// ���̐Α�
		STONE_SWORD					       = 38,	// ���̐Α�
		SMOOTH_CONCRETE_BLOCK              = 39,	// �Ȃ߂炩�ȃR���N���[�g�u���b�N
		SMOOTH_CONCRETE_BLOCK_CRACKED      = 40,	// �Ȃ߂炩�ȂЂъ��ꂽ�R���N���[�g�u���b�N
		CONCRETE_BLOCK                     = 41,	// �R���N���[�g�u���b�N
		BACK_SMOOTH_CONCRETE_BLOCK         = 42,	// (���)�Ȃ߂炩�ȃR���N���[�g�u���b�N
		BACK_SMOOTH_CONCRETE_BLOCK_CRACKED = 43,	// (���)�Ȃ߂炩�ȂЂъ��ꂽ�R���N���[�g�u���b�N
		BACK_CONCRETE_BLOCK                = 44,	// (���)�R���N���[�g�u���b�N
		CORAL_REEF_1                       = 45,	// �T���S��1
		CORAL_REEF_2                       = 46,	// �T���S��2
		CORAL_REEF_3                       = 47,	// �T���S��3
		STONE_1                            = 48,	// ��1
		STONE_2                            = 49,	// ��2
		BUILDING_1                         = 50,	// ����1
		BUILDING_2                         = 51,	// ����2
		WOOD_FENCE						   = 52,	// �؂̍�
		OLD_HOUSE						   = 53,	// ����
		POWER_POLE						   = 54,	// �d��(�d���Ȃ�)
		POWER_POLE_L					   = 55,	// �d��(�����ɓd��)
		POWER_POLE_R					   = 56,	// �d��(�E���ɓd��)
		POWER_POLE_B					   = 57,	// �d��(�����ɓd��)
		BUS_STOP						   = 58,	// �o�X��
		STREET_LIGHT					   = 59,	// �X��
		SIGN							   = 60,	// �Ŕ�
		MAX,
	};
	enum class OTHER_TEXTURE {
		// ���̑��e�N�X�`��
		EFFECT,
		MAX,
	};
	enum class OTHER_MODEL {
		// ���̑����f��
		SOIL_BLOCK_GRASSY,
		MAX,
	};
	enum class OTHER_SETUP3D {
		// ���̑��Z�b�g�A�b�v3D
		BAOBAB_TREE,
		CHEST,
		PALM_TREE,
		MAX,
	};
	enum class OTHER_MOTION3D {
		// ���̑����[�V����3D
		CHEST_STEPPED,
		PLAMTREE_SHAKE,
		MAX,
	};
	enum class OTHER_SOUND {
		// ���̑��T�E���h
		COIN,
		LIGHT_A,
		LIGHT_B,
		MAX,
	};
	enum class MODEL_TYPE {
		// ���f���^�C�v
		PUT,
		MATERIAL_MESH,
		MESH,
	};
	enum class SET_TYPE {
		// �ݒ�^�C�v
		FORWARD,
		BACKWARD,
		FORWARD_AND_BACKWARD,
	};

	//========== [[[ �֐��錾 ]]]
	static void Load(void);
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetCollision(void) { return LOOKS_DATAS[(int)m_looksType].isCollision; }
	LOOKS_TYPE GetLooksType(void) { return m_looksType; }

	//========== [[[ ���A�N�V�����p�֐��錾 ]]]
	bool IsGetOld(const int nRot) { return m_isHitOlds[nRot]; }

	void IsReaction_HitsRot(const int nRot) { m_isHits[nRot] = true; }
	void IsReaction_Move(bool bFrag)  { ; } //��ňړ����Ă���
	void IsReaction_Land(bool bFrag)  { ; } //��ɒ��n�����u��
	void IsReaction_TakeOff(bool bFrag){ ;}//�ォ��W�����v�ŗ��ꂽ�u��

	//========== [[[ �\���̒�` ]]]
	struct LooksData {
		const char* modelPath;
		const char* pasteTexPath;
		Color col;
		MODEL_TYPE modelType;
		SET_TYPE setType;
		float height;
		float depth;
		bool isCollision;
	};

private:
	//========== [[[ �萔�錾 ]]]
	static const LooksData LOOKS_DATAS[(int)LOOKS_TYPE::MAX];
	static const char* OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX];
	static const char* OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX];
	static const char* OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX];
	static const char* OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX];
	static const char* OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX];

	//========== [[[ �ϐ��錾 ]]]
	static UShort  m_num;
	static Scale2D m_eyescale;
	static short   m_modelIdxes[(int)LOOKS_TYPE::MAX];
	static short   m_pasteTexIdxes[(int)LOOKS_TYPE::MAX];
	static short   m_otherTexIdxes[(int)OTHER_TEXTURE::MAX];
	static short   m_otherModelIdxes[(int)OTHER_MODEL::MAX];
	static short   m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
	static short   m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
	static short   m_otherSoundIdxes[(int)OTHER_SOUND::MAX];
	CDoll3D*   m_doll;
	LOOKS_TYPE m_looksType;		// ���
	bool       m_isHitOlds[6];
	bool       m_isHits[6];
	Pos3D      m_targetAddPos;
	Pos3D      m_oldAddPos;
	Pos3D      m_addPos;
	short      m_counter;
	short      m_counterMax;
	int		   m_nTexIdx;
};