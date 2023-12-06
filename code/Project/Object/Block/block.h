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
	// �����ڂ̎��
	enum class LOOKS_TYPE {
		SOIL_BLOCK					 = 0,	// �y�u���b�N
		SOIL_BLOCK_GRASSY			 = 1,	// ���̐������y�u���b�N
		ANCIENT_STONE_BLOCK			 = 2,	// �Ñ�̐΃u���b�N
		ANCIENT_STONE_BLOCK_PTN_A	 = 3,	// �Ñ�̐΃u���b�NA
		ANCIENT_STONE_BLOCK_PTN_B	 = 4,	// �Ñ�̐΃u���b�NB
		ANCIENT_STONE_BLOCK_PTN_C	 = 5,	// �Ñ�̐΃u���b�NC
		ANCIENT_STONE_PILLAR_UP		 = 6,	// ���̏�
		ANCIENT_STONE_PILLAR_CENTER	 = 7,	// ���̌q��
		ANCIENT_STONE_PILLAR_DOWN	 = 8,	// ���̍��{
		LEAF_BLOCK	                 = 9,	// ���u���b�N
		TREE_TRUNK	                 = 10,	// �؂̊�
		TREE_TRUNK_ROOT              = 11,	// �؂̍���
		BAOBAB_TREE					 = 12,	// �o�I�o�u�̖�(�w�i�p)
		CHEST                        = 13,	// ��
		PILE_OF_COINS                = 14,	// �R�ς݂̃R�C��
		STONE_MONUMENT               = 15,	// ��̐Δ�
		ANCIENT_STONE_BRICK_BLOCK    = 16,	// �Ñ�̐΃����K�u���b�N
		SOIL_AND_ANCIENT_STONE_BLOCK = 17,	// �y���Ñ�̐΃u���b�N(���ԗp)
		PALMTREE					 = 18,	// ���V�̖�
		TREE_EYES_BLOCK				 = 19,	// �O�ڂ̃u���b�N
		LEAF_BLOCK_NUTS              = 20,	// ���u���b�N(�؂̎�)
		MAX
	};

	// ���̑��e�N�X�`��
	enum class OTHER_TEXTURE {
		EFFECT,
		MAX,
	};

	// ���̑����f��
	enum class OTHER_MODEL {
		LEAF_INSIDE,
		MAX,
	};

	// ���̑��Z�b�g�A�b�v3D
	enum class OTHER_SETUP3D {
		BAOBAB_TREE,
		CHEST,
		PALM_TREE,
		MAX,
	};

	// ���̑����[�V����3D
	enum class OTHER_MOTION3D {
		CHEST_STEPPED,
		PLAMTREE_SHAKE,
		MAX,
	};

	// ���̑��T�E���h
	enum class OTHER_SOUND {
		COIN,
		LIGHT_A,
		LIGHT_B,
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	static void Load(void);
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCollision(bool isCollision) { m_isCollision = isCollision; }
	bool& GetCollision(void) { return m_isCollision; }

	//========== [[[ ���A�N�V�����p�֐��錾 ]]]
	bool IsGetOld(const int nRot) { return m_isHitOlds[nRot]; }

	void IsReaction_HitsRot(const int nRot) { m_isHits[nRot] = true; }
	void IsReaction_Move(bool bFrag)  { ; } //��ňړ����Ă���
	void IsReaction_Land(bool bFrag)  { ; } //��ɒ��n�����u��
	void IsReaction_TakeOff(bool bFrag){ ;}//�ォ��W�����v�ŗ��ꂽ�u��

private:
	//========== [[[ �萔�錾 ]]]
	static const char* MODEL_PATHS[(int)LOOKS_TYPE::MAX];
	static const char* OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX];
	static const char* OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX];
	static const char* OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX];
	static const char* OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX];
	static const char* OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX];
	//========== [[[ �ϐ��錾 ]]]
	static UShort m_num;
	static Scale2D m_eyescale;
	static short m_modelIdxes[(int)LOOKS_TYPE::MAX];
	static short m_otherTextureIdxes[(int)OTHER_TEXTURE::MAX];
	static short m_otherModelIdxes[(int)OTHER_MODEL::MAX];
	static short m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
	static short m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
	static short m_otherSoundIdxes[(int)OTHER_SOUND::MAX];
	CDoll3D*   m_doll;
	LOOKS_TYPE m_looksType;		// ���
	bool       m_isCollision;
	bool       m_isHitOlds[6];
	bool       m_isHits[6];
	Pos3D      m_targetAddPos;
	Pos3D      m_oldAddPos;
	Pos3D      m_addPos;
	short      m_counter;
	short      m_counterMax;
	int		   m_nTexIdx;
};