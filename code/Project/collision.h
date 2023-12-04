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
		Pos3D minPos;   // �ŏ��ʒu
		Pos3D maxPos;   // �ő�ʒu
		Pos3D minPosOld;// �O��ŏ��ʒu
		Pos3D maxPosOld;// �O��ő�ʒu
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
		Pos3D minPos;   // �ŏ��ʒu
		Pos3D maxPos;   // �ő�ʒu
		Pos3D minPosOld;// �O��ŏ��ʒu
		Pos3D maxPosOld;// �O��ő�ʒu
		float fWidth;   // ��
		float fHeight;  // ����
		float fRadius;  // ���a
		ROT   Rot;      // ������������
	};

	CCollision();
	~CCollision();

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
	// �����R	value	�F�x�N�g��
	// �����S	pAngle	�F��������������Ԃ�
	// �Ԃ�l	�Ώە��ɂ߂肱��ł��邩�Ԃ�  true:�߂荞��  false:�߂荞��ł��Ȃ�
	//========================
	static bool CircleToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec, float *pAngle);

	void Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli,CObject *obj, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);

	//�������J��
	void OthColliDelete(void);

private:
	// ���p�[�c�̓����蔻����
	ColliInfo *m_pOthColli;

	//�㉺���E�̓����蔻��ɂ��ʒu�E�ړ��ʏC��
	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight);
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);

	//�㉺�ǂ��炩�ɓ��������i������j�v���C���[�̐ݒ菈��
	void LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY);

	//�ŏ��ő�ʒu�ݒ菈��
	static void SetMinMaxPos(SelfInfo& self);
	static void SetMinMaxPos(ColliInfo& colli);
};