//================================================================================================
// 
// �����蔻�菈��[collision.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "main.h"
#include "Character/player.h"
#include "collision.h"

//=======================================
// �R���X�g���N�^
//=======================================
CCollision::CCollision()
{
	m_pOthColli = NULL;
}

//=======================================
// �f�X�g���N�^
//=======================================
CCollision::~CCollision()
{
	OthColliDelete();
}

//----------------------------
// ���̓����蔻����̃��������
//----------------------------
void CCollision::OthColliDelete(void)
{
	if (m_pOthColli == NULL) return;
	delete m_pOthColli;
	m_pOthColli = NULL;
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
		Pos3D createPos = pInfo->pos;
		Rot3D createRot = INITROT3D;

		if (pInfo->pos.y > 0.0f) {
			createPos.y -= CPlayer::SIZE_HEIGHT * 0.5f;
		}
		else {
			createPos.y += CPlayer::SIZE_HEIGHT * 0.5f;
			createRot.x += D3DX_PI;
		}
		pInfo->landingCounter = 30;
		pInfo->doll->OverwriteMotion(CPlayer::GetMotion().landing);

		RNLib::StandardEffect3D().CreateDustStormOnLanding(createPos, createRot,  Color{ 169,158,93,255 }, 10.0f);
	}

	pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
	pInfo->bJump = false;	// �W�����v�\
	pInfo->fMaxHeight = fMaxY;// �ō��x���W�ݒ�
}

//----------------------------
// �u���b�N�̓����蔻�菈��
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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

			// �\�̐��E�̃v���C���[
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				//�v���C���[�I�v�V�����ݒ�
				LandPlayerOption(pInfo, pColli->minPos.y);
			}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:	FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

			//*********************************
			// ���܂���
			//*********************************
		case ROT::UNKNOWN:*pDeath = true; break;
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
				CPlayer *pPlayer = CMode_Game::GetPlayer();
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
				CPlayer *pPlayer = CMode_Game::GetPlayer();
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
				pColli->Rot = IsBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					pColli->Rot = IsBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

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
		*pDeath = true;
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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
	CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
	CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);
	if (pRocket->GetReady() && pInfo->bRide) return;

	// ���P�b�g�ɓ���
	pInfo->bRide = true;
	pRocket->Ride();
	
	const int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
			else
			{
				//�v���C���[�擾
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				if (*pSide == CPlayer::WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
				else 									pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�

				pInfo->move.y = 0.0f;
				pInfo->bLandPile = true;// �����
				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				pInfo->bJump = false;	// �W�����v�\
			}
		}break;
	}
}

//========================
// �Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
//------------------------
// �����P	self	�F�����̏��
// �����Q	target	�F�Ώۂ̏��
// �����R	value	�F�x�N�g��
// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
//========================
CCollision::ROT CCollision::IsBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec)
{
	// �����̌��݂̍ŏ��E�ő�ʒu
	self.minPos = Pos3D(self.pos.x - self.fWidth, self.pos.y - self.fHeight, 0.0f);
	self.maxPos = Pos3D(self.pos.x + self.fWidth, self.pos.y + self.fHeight, 0.0f);

	// �Ώۂ̌��݂̍ŏ��E�ő�ʒu
	target.minPos = Pos3D(target.pos.x - target.fWidth, target.pos.y - target.fHeight, 0.0f);
	target.maxPos = Pos3D(target.pos.x + target.fWidth, target.pos.y + target.fHeight, 0.0f);

	// �����̉ߋ��̍ŏ��E�ő�ʒu
	const Pos3D OLD_MINPOS = Pos3D(self.posOld.x - self.fWidth, self.posOld.y - self.fHeight, 0.0f);
	const Pos3D OLD_MAXPOS = Pos3D(self.posOld.x + self.fWidth, self.posOld.y + self.fHeight, 0.0f);

	// �Ώۂ̑O��̍ŏ��E�ő�ʒu
	const Pos3D TARGET_MinPosOld = Pos3D(target.posOld.x - target.fWidth, target.posOld.y - target.fHeight, 0.0f);
	const Pos3D TARGET_MaxPosOld = Pos3D(target.posOld.x + target.fWidth, target.posOld.y + target.fHeight, 0.0f);

	// �Փ˃x�N�g���ŏ�������
	switch (vec)
	{
		case CPlayer::VECTOL::X:
			if (self.minPos.y < target.maxPos.y && target.minPos.y < self.maxPos.y)
			{// Y�͈͓�
				const bool isLeft  = self.maxPos.x >= target.minPos.x;
				const bool isRight = self.minPos.x <= target.maxPos.x;

				if (isLeft && OLD_MAXPOS.x <= TARGET_MinPosOld.x)
					return ROT::LEFT;

				if (isRight && OLD_MINPOS.x >= TARGET_MaxPosOld.x)
					return ROT::RIGHT;

				if (isLeft && isRight)
					return ROT::UNKNOWN;
			}

			break;

		case CPlayer::VECTOL::Y:
			if (self.minPos.x < target.maxPos.x && target.minPos.x < self.maxPos.x)
			{// X�͈͓�
				const bool isUnder = self.maxPos.y >= target.minPos.y;
				const bool isOver  = self.minPos.y <= target.maxPos.y;

				if (isUnder && OLD_MAXPOS.y <= TARGET_MinPosOld.y)
					return ROT::UNDER;

				if (isOver && OLD_MINPOS.y >= TARGET_MaxPosOld.y)
					return ROT::OVER;

				if(isUnder && isOver)
					return ROT::UNKNOWN;
			}

			break;
	}

	// ������Ȃ�����
	return ROT::NONE;
}