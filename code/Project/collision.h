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
		Vector3D pos;		// �ʒu
		Vector3D posOld;	// �O��ʒu
		Vector3D minPos;	// �ŏ��ʒu
		Vector3D maxPos;	// �ő�ʒu
		Vector3D move;		// �ړ���
		float	 fWidth;	// ��
		float	 fHeight;	// ����
	};

	// �����蔻����
	struct ColliInfo
	{
		D3DXVECTOR3 pos;        // �ʒu
		D3DXVECTOR3 posOld;     // �O��ʒu
		D3DXVECTOR3 minPos;		// �ŏ��ʒu
		D3DXVECTOR3 maxPos;		// �ő�ʒu
		float		fWidth;		// ��
		float		fHeight;	// ����
		ROT			Rot;		// ������������
	};

	CCollision();
	~CCollision();

	//========================
	// �Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
	//------------------------
	// �����P	self	�F�����̏��
	// �����Q	target	�F�Ώۂ̏��
	// �����R	value	�F�x�N�g��
	// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
	//========================
	ROT IsBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec);

	void Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot, CPlayer::WORLD_SIDE *pSide = NULL);
	void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide = NULL);
	void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL);
	void Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide = NULL);
	void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide = NULL);
	void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide = NULL);

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
};