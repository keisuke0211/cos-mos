#pragma once
//================================================================================================
// 
// �����蔻�菈��[collision.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "Object/stage-object-manager.h"
#include "Character\player.h"

//�����蔻��N���X
class CCollision{
public:
	// �����蔻�肪����������
	enum class ROT {
		NONE = 0,	// �����Ȃ�
		OVER,		// ��
		UNDER,		// ��
		LEFT,		// ��
		RIGHT,		// �E
		UNKNOWN,	// �������Ă��邯�Ǖ�����������Ȃ��i�������鑤�������Ă���\���A��
		MAX
	};

	// �����蔻����s�����̂̏��
	struct SelfInfo
	{
		Pos3D pos;	    // �ʒu
		Pos3D posOld;   // �O��ʒu
		Pos2D minPos;   // �ŏ��ʒu
		Pos2D maxPos;   // �ő�ʒu
		Pos2D minPosOld;// �O��ŏ��ʒu
		Pos2D maxPosOld;// �O��ő�ʒu
		Pos3D move;     // �ړ���
		float fWidth;   // ��
		float fHeight;  // ����
		float fRadius;  // ���a
	};

	// �����蔻����
	struct ColliInfo
	{
		Pos3D pos;      // �ʒu
		Pos3D posOld;   // �O��ʒu
		Pos2D minPos;   // �ŏ��ʒu
		Pos2D maxPos;   // �ő�ʒu
		Pos2D minPosOld;// �O��ŏ��ʒu
		Pos2D maxPosOld;// �O��ő�ʒu
		float fWidth;   // ��
		float fHeight;  // ����
		float fRadius;  // ���a
		ROT   Rot;      // ������������
		int   vec;      // �x�N�g��
	};

	CCollision();
	~CCollision();

	//========================
	//�����𑪂�֐��i���肷��x�N�g���w��\
	//------------------------
	//�����P�E�Q  ���肷����W�Ȃ�
	//�����R�`�T  �e�x�N�g�����g�p���邩�ǂ���
	//========================
	static float Length(Pos3D& vec1, Pos3D& vec2, bool bXVec, bool bYVec, bool bZVec);

	//========================
	//2�_�̋������A�T�C�Y�͈͓̔����ǂ����𔻒�i���肷��x�N�g���w��\
	//------------------------
	//�����P�E�Q  ���肷����i�ʒu�ƃT�C�Y
	//�����R�E�S  �e�x�N�g�����g�p���邩�ǂ����i�T�C�Y�ɉ��s���͖����̂�Z�x�N�g���͎g�p���Ȃ�
	//�����T�E�U  �e���̔��a���g�p���邩�ǂ���
	//�����V      �X���b�v�C���^�[�o�������ǂ���
	//========================
	static bool IsInRange(SelfInfo& self, ColliInfo& target, bool bXVec, bool bYVec, bool bUseSelfRadius = false, bool bUseTargetRadius = false, int ForPlayerInterval = 0);

	//========================
	// ��`�Ƌ�`�̓����蔻��
	//------------------------
	// �����P	self	�F�����̏��
	// �����Q	target	�F�Ώۂ̏��
	// �����R	value	�F�x�N�g��
	// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
	//========================
	static ROT IsBoxToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec);

	//========================
	// �~�Ƌ�`�̓����蔻��
	//------------------------
	// �����P	self	�F�����̏��
	// �����Q	target	�F�Ώۂ̏��
	// �����R	pAngle	�F��������������Ԃ�
	// �Ԃ�l	�Ώە��ɂ߂肱��ł��邩�Ԃ�  true:�߂荞��  false:�߂荞��ł��Ȃ�
	//========================
	static bool CircleToBoxCollider(SelfInfo& self, ColliInfo& target, float *pAngle);

	static void Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::Info& Player, CBlock* pBlock, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli,CObject *obj, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);

private:

	//�㉺���E�̓����蔻��ɂ��ʒu�E�ړ��ʏC��
	static void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight);
	static void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);
	static void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);
	static void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);

	//�㉺�ǂ��炩�ɓ��������i������j�v���C���[�̐ݒ菈��
	static void LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY);

	//�ŏ��ő�ʒu�ݒ菈��
	static void SetMinMaxPos(SelfInfo& self);
	static void SetMinMaxPos(ColliInfo& colli);

	bool IsEqual(float& f11, float& f12, float& f21, float& f22);
};