//================================================================================================
// 
// �����蔻�菈��[collision.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "main.h"
#include "Character/player.h"
#include "collision.h"
#include "Sound/stage-sound-player.h"
#include "stage.h"

//=======================================
// �R���X�g���N�^
//=======================================
CCollision::CCollision()
{

}

//=======================================
// �f�X�g���N�^
//=======================================
CCollision::~CCollision()
{

}

//========================
//�����𑪂�֐��i���肷��x�N�g���w��\
//------------------------
//�����P�E�Q  ���肷����W�Ȃ�
//�����R�`�T  �e�x�N�g�����g�p���邩�ǂ���
//========================
float CCollision::Length(Pos3D& vec1, Pos3D& vec2, bool bXVec, bool bYVec, bool bZVec)
{
	Pos3D vectol = INITPOS3D;

	/*X�x�N�g���g�p*/if (bXVec) vectol.x = vec1.x - vec2.x;
	/*Y�x�N�g���g�p*/if (bYVec) vectol.y = vec1.y - vec2.y;
	/*Z�x�N�g���g�p*/if (bZVec) vectol.z = vec1.z - vec2.z;

	//������Ԃ�
	return D3DXVec3Length(&vectol);
}

//========================
//2�_�̋������A�T�C�Y�͈͓̔����ǂ����𔻒�i���肷��x�N�g���w��\
//------------------------
//�����P�E�Q  ���肷����i�ʒu�ƃT�C�Y
//�����R�E�S  �e�x�N�g�����g�p���邩�ǂ����i�T�C�Y�ɉ��s���͖����̂�Z�x�N�g���͎g�p���Ȃ�
//========================
bool CCollision::IsInRange(SelfInfo& self, ColliInfo& target, bool bXVec, bool bYVec, bool bUseSelfRadius, bool bUseTargetRadius)
{
	//�Q�̏��̃T�C�Y���i�[
	Pos3D selfSize = INITPOS3D, targetSize = INITPOS3D;

	/*  �����  */ if (bXVec) { selfSize.x = bUseSelfRadius ? self.fRadius : self.fWidth;  targetSize.x = bUseTargetRadius ? target.fRadius : target.fWidth; }
	/* ������� */ if (bYVec) { selfSize.y = bUseSelfRadius ? self.fRadius : self.fHeight; targetSize.y = bUseTargetRadius ? target.fRadius : target.fHeight; }

	//�ʒu�E�T�C�Y�̋������擾
	CFloat PosLength = Length(self.pos, target.pos, bXVec, bYVec, false);
	CFloat SizeLength = Length(selfSize, targetSize, bXVec, bYVec, false);

	//�͈͓��ɋ��邩�ǂ����Ԃ�
	return PosLength < SizeLength;
}

//----------------------------
// �ォ��̓����蔻��ɂ��ʒu�C��
//----------------------------
void CCollision::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	// �ʒu�E�ړ��ʏC��
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// ������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CCollision::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	// �ʒu�E�ړ��ʏC��
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// ������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CCollision::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	// �ʒu�E�ړ��ʏC��
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// �E����̓����蔻��ɂ��ʒu�C��
//----------------------------
void CCollision::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	// �ʒu�E�ړ��ʏC��
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
//�㉺�ǂ��炩�ɓ��������i������j�v���C���[�̐ݒ菈��
// �T�v ----------------------
//�E���nSE�Đ�(�W�����v���̂�)
//�EbGround = true
//�EbJump = false
//�EfMaxHeight = fMaxY
//----------------------------
void CCollision::LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY)
{
	if (pInfo == NULL) return;

	// ���nSE�Đ�
	if (pInfo->bJump == true) {
		CPlayer::PlaySE(CPlayer::SE_LABEL::LANDING);
		
		// �G�t�F�N�g
		Pos3D createPos = pInfo->pos;
		Rot3D createRot = INITROT3D;

		if (pInfo->pos.y > 0.0f) {
			createPos.y -= CPlayer::SIZE_HEIGHT * 0.5f;
		}
		else {
			createPos.y += CPlayer::SIZE_HEIGHT * 0.5f;
			createRot.x += D3DX_PI;
		}
		RNLib::StandardEffect3D().CreateDustStormOnLanding(createPos, createRot, Color{ 169,158,93,255 }, 20.0f);

		if (pInfo->swapWaitCounter == 0) {

			// ���[�V����
			pInfo->landingCounter = 30;
			pInfo->doll->OverwriteMotion(CPlayer::GetMotion(pInfo->idx).landing);
		}
	}

	pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
	pInfo->bJump = false;	// �W�����v�\
	pInfo->fMaxHeight = fMaxY;// �ō��x���W�ݒ�
}

//----------------------------
// �u���b�N�̓����蔻�菈��
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::Info& Player, CBlock* pBlock, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			if (// �����ڂ̎�ނɂ�铖���蔻��̏��O
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR   && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 && pSelfInfo->pos.y < 0.0f) ||
				false)
				break;

			// ���n
			if (pSelfInfo->pos.y > 0.0f && !Player.bGroundOld)
				StageSoundPlayer::PlayLandingSEIdx(pBlock->GetLooksType());

			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// �\�̐��E�̃v���C���[
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->maxPos.y);
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			if (// �����ڂ̎�ނɂ�铖���蔻��̏��O
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR   && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 && pSelfInfo->pos.y > 0.0f) ||
				false)
				break;

			// ���n
			if (pSelfInfo->pos.y < 0.0f && !Player.bGroundOld)
				StageSoundPlayer::PlayLandingSEIdx(pBlock->GetLooksType());

			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// �\�̐��E�̃v���C���[
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->minPos.y);
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:	
			if (// �����ڂ̎�ނɂ�铖���蔻��̏��O
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				false)
				break;

			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			if (// �����ڂ̎�ނɂ�铖���蔻��̏��O
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				false)
				break;

			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// ���܂���
			//*********************************
		case ROT::UNKNOWN:
			if (// �����ڂ̎�ނɂ�铖���蔻��̏��O
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::PILE_OF_COINS ||
				false)
				break;

			*pDeath = true;

			break;
	}
}

//----------------------------
// �����߃u���b�N�̓����蔻�菈��
//----------------------------
void CCollision::FillBlock(SelfInfo *pSelfInfo, ROT Rot, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	//���S����
	*pDeath = true;
}

//----------------------------
// �g�����|�����̓����蔻�菈��
//----------------------------
void CCollision::Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// �\�̐��E�̃v���C���[
			if (pSide != NULL &&
				*pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//�v���C���[�擾
				CPlayer *pPlayer = Stage::GetPlayer();
				CPlayer::Info *pInfo = pPlayer->GetInfo(*pSide);

				// ���n����
				if (pSelfInfo->posOld.y > pSelfInfo->pos.y)
					pTrampoline->SetBound(CTrampoline::STATE::UP_LAND, pInfo->fMaxHeight);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->maxPos.y);

				//�g�����|�����ɂ�钵�ˏグ����
				if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND)
					pPlayer->SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
			if (pSide != NULL &&
				*pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//�v���C���[�擾
				CPlayer *pPlayer = Stage::GetPlayer();
				CPlayer::Info *pInfo = pPlayer->GetInfo(*pSide);

				// ���n����
				if (pSelfInfo->posOld.y < pSelfInfo->pos.y)
					pTrampoline->SetBound(CTrampoline::STATE::DOWN_LAND, pInfo->fMaxHeight);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->minPos.y);

				//�g�����|�����ɂ�钵�ˏグ����
				if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND)
					pPlayer->SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
			}
			break;
	}
}

//----------------------------
// �g�Q�̓����蔻�菈��
//----------------------------
void CCollision::Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	switch (pColli->Rot)
	{
		//*********************************
		//�㉺�ǂ��炩�ɓ�����Ύ��S
		//*********************************
		case ROT::OVER:
		case ROT::UNDER: 
		case ROT::UNKNOWN: *pDeath = true;break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;
	}
}

//----------------------------
// �ړ����̓����蔻�菈��
//----------------------------
void CCollision::MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// �\�̐��E�̃v���C���[
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->maxPos.y);
				pSelfInfo->move = pMoveBlock->GetMove();
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// ���̐��E�̃v���C���[
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->minPos.y);
				pSelfInfo->move = pMoveBlock->GetMove();
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:	FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

		case ROT::UNKNOWN:
			// �ړ�������̓����蔻��
			for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
			{
				SelfInfo self;						ColliInfo colliInfo;
				self.pos = pColli->pos;				colliInfo.pos = pSelfInfo->pos;
				self.posOld = pColli->posOld;		colliInfo.posOld = pSelfInfo->posOld;
				self.fWidth = pColli->fWidth;		colliInfo.fWidth = pSelfInfo->fWidth;
				self.fHeight = pColli->fHeight;		colliInfo.fHeight = pSelfInfo->fHeight;

				// �v���C���[�̂ǂ̕����ɓ������Ă��邩
				pColli->Rot = IsBoxToBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

				// ����ł�������Ȃ��Ȃ�A�X�L�b�v
				if (pColli->Rot == ROT::NONE || pColli->Rot == ROT::UNKNOWN) continue;

				// �������������i�㉺�E���E�j�𔽓]����
				{
					// ��������������int�^�ɕϊ�
					const int nRot = (int)pColli->Rot;
					pColli->Rot = (ROT)(nRot - 1 + 2 * (nRot % 2));
				}

				// ������x�����蔻��
				MoveBlock(pSelfInfo, pMoveBlock, pColli);
			}
			break;
	}
}

//----------------------------
// 覐΂̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// ���S����
	*pDeath = true;
}

//----------------------------
// ���[�U�[�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// �{��
	{
		// ���������������Ƃɏ�����؂�ւ�
		switch (pColli->Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// �\�̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->maxPos.y);
					pSelfInfo->move = pRoadTripLaser->GetMove();
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// ���̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->minPos.y);
					pSelfInfo->move = pRoadTripLaser->GetMove();
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN:
				// �ړ�������̓����蔻��
				for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
				{
					SelfInfo self;						ColliInfo colliInfo;
					self.pos = pColli->pos;				colliInfo.pos = pSelfInfo->pos;
					self.posOld = pColli->posOld;		colliInfo.posOld = pSelfInfo->posOld;
					self.fWidth = pColli->fWidth;		colliInfo.fWidth = pSelfInfo->fWidth;
					self.fHeight = pColli->fHeight;		colliInfo.fHeight = pSelfInfo->fHeight;

					// �v���C���[�̂ǂ̕����ɓ������Ă��邩
					pColli->Rot = IsBoxToBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

					// ����ł�������Ȃ��Ȃ�A�X�L�b�v
					if (pColli->Rot == ROT::NONE || pColli->Rot == ROT::UNKNOWN) continue;

					// �������������i�㉺�E���E�j�𔽓]����
					{
						// ��������������int�^�ɕϊ�
						const int nRot = (int)pColli->Rot;
						pColli->Rot = (ROT)(nRot - 1 + 2 * (nRot % 2));
					}

					// ������x�����蔻��
					Laser(pSelfInfo, pRoadTripLaser, pColli, pOthColli);
				}
				break;
		}
	}

	// ���[�U�[
	{
		// ���S����
		//*pDeath = true;
	}
}

//----------------------------
// �k�C�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// �n�E�X
	{
		switch (pColli->Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// �\�̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// ���̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->minPos.y);
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: *pDeath = true; break;
		}
	}

	//�ʂ̓����蔻��̃T�C�Y���擾�i�ʏ�ʂ�Ȃ�y���E�́E���z��3�ɂȂ�
	const int SIZE = sizeof pOthColli / sizeof ColliInfo;
	for (int nCntColli = 0; nCntColli < SIZE; nCntColli++)
	{
		switch (pOthColli[nCntColli].Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[nCntColli].maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// ���̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[nCntColli].minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// ���̐��E�̃v���C���[
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//�v���C���[�擾
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->minPos.y);
				}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[nCntColli].minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[nCntColli].maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: *pDeath = true; break;
		}
	}
}

//----------------------------
// �S�[���Q�[�g�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli, CObject *obj, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	if (pSide == NULL) return;

	//�v���C���[�擾
	CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

	if (!pInfo->bGoal)
	{
		//�I�u�W�F�N�g���L���X�g
		CGoalGate *GoalGateObj = dynamic_cast<CGoalGate*>(obj);
		GoalGateObj->SetEntry(true);

		pInfo->bGoal = true;
		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::GOAL_EFFECT), pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
		}
	}
}

//----------------------------
// �p�[�c�̓����蔻�菈��
//----------------------------
void CCollision::Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	if (!pParts->GetDisp()) return;

	// �擾�������E�`��OFF
	pParts->DispSwitch(false);
	pParts->GetParts();

	// �擾���������S�̐��Ɠ����Ȃ�A���P�b�g��ԉ\
	if (CParts::IsReady()) CRocket::ReadyRide();
}

//----------------------------
// ���P�b�g�̓����蔻�菈��
//----------------------------
void CCollision::Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	RNLib::Text2D().PutDebugLog("���P�b�g�ɓ������Ă���");

	CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);
	if (!pRocket->GetReady() && !pInfo->bRide) return;

	// ���P�b�g�ɓ���
	if (!CPlayer::IsKeyConfigTrigger(pInfo->idx, *pSide, CPlayer::KEY_CONFIG::JUMP) ||
		pInfo->nRideInterval != 0) return;

	pInfo->bRide = true;
	pInfo->nRideInterval = CRocket::RIDE_ONOFF_INTERVAL;
	pRocket->RideOn();
	
	CInt ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	{
		Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	}
}

//----------------------------
// �Y�̓����蔻�菈��
//----------------------------
void CCollision::Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// �㉺�ɓ�������
		//*********************************
		case ROT::OVER:
		case ROT::UNDER:
			if (pSide != NULL)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				// �\�̐��E�̃v���C���[
				if (*pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//������x�̍������痎�����Ă���
					if (pInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
						pPile->CaveInTrunkHeight(pColli->maxPos.y - pSelfInfo->pos.y - CPlayer::SIZE_HEIGHT);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}

				// ���̐��E�̃v���C���[
				else
				{
					//������x�̍������痎�����Ă���
					if (pInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
						pPile->CaveInTrunkHeight(pColli->minPos.y - pSelfInfo->pos.y + CPlayer::SIZE_HEIGHT);

					//�v���C���[�I�v�V�����ݒ�
					LandPlayerOption(pInfo, pColli->minPos.y);
				}

				pInfo->bLandPile = true;// �����
				pInfo->move.y = 0.0f;
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:
			// �ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			// �ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// ���܂���
			//*********************************
		case CCollision::ROT::UNKNOWN:
		{
			if (pSide == NULL) return;

			if (CPlayer::GetSwapInterval() != 0) *pDeath = true;
			else if(pColli->vec == (int)CPlayer::VECTOL::Y)
			{
				//��������������K���ɏ�ɂ��ďC��
				pColli->Rot = ROT::OVER;
				Pile(pSelfInfo, pColli, pPile, pSide, pDeath);

				//�v���C���[�擾
				//CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);
				//
				//if (*pSide == CPlayer::WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
				//else 									pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
				//
				//pInfo->move.y = 0.0f;
				//pInfo->bLandPile = true;// �����
				//pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//pInfo->bJump = false;	// �W�����v�\
			}
		}break;
	}
}

//========================
// ��`�Ƌ�`�̓����蔻��
//------------------------
// �����P	self	�F�����̏��
// �����Q	target	�F�Ώۂ̏��
// �����R	value	�F�x�N�g��
// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
//========================
CCollision::ROT CCollision::IsBoxToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec)
{
	// �����̌��݂ƑO��̍ŏ��E�ő�ʒu
	SetMinMaxPos(self);

	// �Ώۂ̌��݂ƑO��̍ŏ��E�ő�ʒu
	SetMinMaxPos(target);

	// �Փ˃x�N�g���ŏ�������
	switch (vec)
	{
		case CPlayer::VECTOL::X:
			if (RNLib::Number().GetIsLs(self.minPos.y, target.maxPos.y) && RNLib::Number().GetIsLs(target.minPos.y, self.maxPos.y))
			{// Y�͈͓�
				const bool isLeft  = RNLib::Number().GetIsGtOrEq(self.maxPos.x, target.minPos.x);
				const bool isRight = RNLib::Number().GetIsLsOrEq(self.minPos.x, target.maxPos.x);

				if (isLeft && (RNLib::Number().GetIsLsOrEq(self.maxPosOld.x, target.minPosOld.x) || RNLib::Number().GetIsLsOrEq(self.posOld.x, target.minPosOld.x)))
					return ROT::LEFT;

				if (isRight && (RNLib::Number().GetIsGtOrEq(self.minPosOld.x, target.maxPosOld.x) || RNLib::Number().GetIsGtOrEq(self.posOld.x, target.maxPosOld.x)))
					return ROT::RIGHT;

				if (isLeft && isRight)
					return ROT::UNKNOWN;
			}
			break;

		case CPlayer::VECTOL::Y:
			if (RNLib::Number().GetIsLs(self.minPos.x, target.maxPos.x) && RNLib::Number().GetIsLs(target.minPos.x, self.maxPos.x))
			{// X�͈͓�
				const bool isUnder = RNLib::Number().GetIsGtOrEq(self.maxPos.y, target.minPos.y);
				const bool isOver =  RNLib::Number().GetIsLsOrEq(self.minPos.y, target.maxPos.y);

				if (isUnder && (RNLib::Number().GetIsLsOrEq(self.maxPosOld.y, target.minPosOld.y) || RNLib::Number().GetIsLsOrEq(self.posOld.y, target.minPosOld.y)))
					return ROT::UNDER;

				if (isOver && (RNLib::Number().GetIsGtOrEq(self.minPosOld.y, target.maxPosOld.y) || RNLib::Number().GetIsGtOrEq(self.posOld.y, target.maxPosOld.y)))
					return ROT::OVER;

				if (isUnder && isOver)
					return ROT::UNKNOWN;
			}
			break;
	}

	// ������Ȃ�����
	return ROT::NONE;
}

//========================
// �~�Ƌ�`�̓����蔻��
//------------------------
// �����P	self	�F�����̏��
// �����Q	target	�F�Ώۂ̏��
// �����R	pAngle	�F��������������Ԃ�
// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
//========================
bool CCollision::CircleToBoxCollider(SelfInfo& self, ColliInfo& target, float *pAngle)
{
	// �Ώۂ̌��݂ƑO��̍ŏ��E�ő�ʒu
	SetMinMaxPos(target);

	//�������A�����̔��a�{�Ώۂ̑Ίp���̒������傫��
	if (IsInRange(self, target, true, true, true, false)) return false;

	//��������Ώۂ܂ł̊p�x
	const Pos3D PosDiff = self.pos - target.pos;
	CFloat Angle = atan2f(-PosDiff.x, -PosDiff.y);
	const Pos2D NearPos = Pos2D(sinf(Angle) * self.fRadius + self.pos.x,
								cosf(Angle) * self.fRadius + self.pos.y);

	//�Ώۂ̒��ɁA��������Ώۂ܂ł̍ŒZ�|�C���g���߂荞��ł���
	if (target.minPos.x <= NearPos.x && NearPos.x <= target.maxPos.x &&
		target.minPos.y <= NearPos.y && NearPos.y <= target.maxPos.y)
	{
		//��������������������
		if (pAngle != NULL)
		{
			//�Ώۂ��玩���܂ł̊p�x
			*pAngle = atan2f(PosDiff.x, PosDiff.y);
		}

		//��������
		return true;
	}

	//������Ȃ�����
	return false;
}

//========================
//�ŏ��ő�ʒu�ݒ菈��
//========================
void CCollision::SetMinMaxPos(SelfInfo& self)
{
	// �����̌��݂ƑO��̍ŏ��E�ő�ʒu
	self.minPos = Pos2D(self.pos.x - self.fWidth, self.pos.y - self.fHeight);
	self.maxPos = Pos2D(self.pos.x + self.fWidth, self.pos.y + self.fHeight);
	self.minPosOld = Pos2D(self.posOld.x - self.fWidth, self.posOld.y - self.fHeight);
	self.maxPosOld = Pos2D(self.posOld.x + self.fWidth, self.posOld.y + self.fHeight);
}

//========================
//�ŏ��ő�ʒu�ݒ菈��
//========================
void CCollision::SetMinMaxPos(ColliInfo& colli)
{
	colli.minPos = Pos2D(colli.pos.x - colli.fWidth, colli.pos.y - colli.fHeight);
	colli.maxPos = Pos2D(colli.pos.x + colli.fWidth, colli.pos.y + colli.fHeight);
	colli.minPosOld = Pos2D(colli.posOld.x - colli.fWidth, colli.posOld.y - colli.fHeight);
	colli.maxPosOld = Pos2D(colli.posOld.x + colli.fWidth, colli.posOld.y + colli.fHeight);
}