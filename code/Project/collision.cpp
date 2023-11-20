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
	delete[] m_pOthColli;
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
// �u���b�N�̓����蔻�菈��
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// �\�̐��E�̃v���C���[
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:
			// �ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			// �ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// ���܂���
			//*********************************
		case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
	}
}

//----------------------------
// �����߃u���b�N�̓����蔻�菈��
//----------------------------
void CCollision::FillBlock(SelfInfo *pSelfInfo, ROT Rot)
{
	//Death(&pSelfInfo->pos);
}

//----------------------------
// �g�����|�����̓����蔻�菈��
//----------------------------
void CCollision::Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// �\�̐��E�̃v���C���[
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	if (pSelfInfo->posOld.y > pSelfInfo->pos.y)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//		pTrampoline->SetState(CTrampoline::STATE::UP_LAND);
			//		pTrampoline->SetSpringForce(pSelfInfo->fMaxHeight);
			//		pTrampoline->SetCount(CTrampoline::MAX_COUNT);
			//	}
			//
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
			//
			//	if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND) {
			//		//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
			//	}
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	if (pSelfInfo->posOld.y < pSelfInfo->pos.y)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//		pTrampoline->SetState(CTrampoline::STATE::DOWN_LAND);
			//		pTrampoline->SetSpringForce(pSelfInfo->fMaxHeight);
			//		pTrampoline->SetCount(CTrampoline::MAX_COUNT);
			//	}
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
			//
			//	if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND) {
			//		//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
			//	}
			//}
			break;
	}
}

//----------------------------
// �g�Q�̓����蔻�菈��
//----------------------------
void CCollision::Spike(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	switch (pColli->Rot)
	{
		//�㉺�ǂ��炩�ɓ�����Ύ��S
		case ROT::OVER:
		case ROT::UNDER:
			// ���S����
			//Death(&pSelfInfo->pos);
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:
			// �ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			// �ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;
	}
}

//----------------------------
// �ړ����̓����蔻�菈��
//----------------------------
void CCollision::MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli)
{
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �ʒu�E�ړ��ʏC��
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// �\�̐��E�̃v���C���[�̏ꍇ
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	pSelfInfo->move = pMoveBlock->GetMove();
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// �ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	pSelfInfo->pos = pMoveBlock->GetMove();
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:
			// �ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			// �ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

		case ROT::UNKNOWN:
			// �ړ��� -> �v���C���[�ւ̓����蔻�菈�������s
			const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
			const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
			const float fWidth = pMoveBlock->GetWidth() * 0.5f;
			const float fHeight = pMoveBlock->GetHeight() * 0.5f;

			// �ړ�������̓����蔻��
			for (int nCntVec = 0; nCntVec < (int)VEC::MAX; nCntVec++)
			{
				// �v���C���[�̂ǂ̕����ɓ������Ă��邩
				//pColli->Rot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pSelfInfo->pos, pSelfInfo->posOld, pColli->fWidth, pColli->fHeight, (VEC)nCntVec);

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
void CCollision::Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	// ���S����
	//Death(&pSelfInfo->pos);
}

//----------------------------
// ���[�U�[�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli)
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
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// �\�̐��E�̃v���C���[�̏ꍇ
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
				//{
				//	pSelfInfo->move = pRoadTripLaser->GetMove();
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// �\�̐��E�̃v���C���[�̏ꍇ
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
				//{
				//	pSelfInfo->move = pRoadTripLaser->GetMove();
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:
				// �ʒu�E�ړ��ʏC��
				FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:
				// �ʒu�E�ړ��ʏC��
				FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN:
				// ���[�U�[ -> �v���C���[�ւ̓����蔻�菈�������s
				const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
				const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
				const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
				const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;

				// �ړ�������̓����蔻��
				for (int nCntVec = 0; nCntVec < (int)VEC::MAX; nCntVec++)
				{
					// �v���C���[�̂ǂ̕����ɓ������Ă��邩
					//pColli->Rot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pSelfInfo->pos, pSelfInfo->posOld, pColli->fWidth, pColli->fHeight, (VEC)nCntVec);

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
		//Death(&pSelfInfo->pos);
	}
}

//----------------------------
// �k�C�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli)
{
	// ���������������Ƃɏ�����؂�ւ�
	CExtenddog::STATE State = pExtenddog->GetState();

	// �n�E�X
	{
		switch (pColli->Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// �\�̐��E�̃v���C���[
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// ���̐��E�̃v���C���[�Ȃ�W�����v�\
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:
				// �ʒu�E�ړ��ʏC��
				FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:
				// �ʒu�E�ړ��ʏC��
				FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fHeight);
				break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}

	// ��
	{
		CExtenddog::STATE state = pExtenddog->GetState();
		switch (pOthColli[0].Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[0].maxPos.y, &pSelfInfo->move.y, pOthColli[0].fHeight);

				// �\�̐��E�̃v���C���[�̏ꍇ
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE){
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[0].maxPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[0].minPos.y, &pSelfInfo->move.y, pOthColli[0].fHeight);

				// �\�̐��E�̃v���C���[�̏ꍇ
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE){
				//	if (pSelfInfo->bJump == true)
				//	{// ���n����
				//	 // SE�Đ�
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[0].maxPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:
				// �ʒu�E�ړ��ʏC��
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[0].minPos.x, &pSelfInfo->move.x, pOthColli[0].fWidth);
				break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:

				if (state == CExtenddog::STATE::NONE) {
					// �ʒu�E�ړ��ʏC��
					FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[0].maxPos.x, &pSelfInfo->move.x, pOthColli[0].fWidth);
				}
				break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: break;

		}
	}

	// ��
	{
		switch (pOthColli[1].Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[1].maxPos.y, &pSelfInfo->move.y, pOthColli[1].fHeight);

				// �\�̐��E�̃v���C���[
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[1].maxPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[1].minPos.y, &pSelfInfo->move.y, pOthColli[1].fHeight);

				// ���̐��E�̃v���C���[�Ȃ�W�����v�\
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[1].minPos.y;// �ō��x���W�ݒ�
				//}
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:
				// �ʒu�E�ړ��ʏC��
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[1].minPos.x, &pSelfInfo->move.x, pOthColli[1].fWidth);
				break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:
				// �ʒu�E�ړ��ʏC��
				FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[1].maxPos.x, &pSelfInfo->move.x, pOthColli[1].fWidth);
				break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}

	// �K
	{
		switch (pOthColli[2].Rot)
		{
			//*********************************
			// ��ɓ�������
			//*********************************
			case ROT::OVER:
				// �ʒu�E�ړ��ʏC��
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[2].maxPos.y, &pSelfInfo->move.y, pOthColli[2].fHeight);

				// �\�̐��E�̃v���C���[
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {

				//	if (pSelfInfo->bJump == true)
				//		//s_SE.pSound->Play(//s_SE.dog[0], CSound::CATEGORY::SE, false);

				//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[2].maxPos.y;// �ō��x���W�ݒ�
				//}

				//pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
				//pSelfInfo->bExtendDog = true;
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::UNDER:
				// �ʒu�E�ړ��ʏC��
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[2].minPos.y, &pSelfInfo->move.y, pOthColli[2].fHeight);

				// ���̐��E�̃v���C���[�Ȃ�W�����v�\
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {

				//	if (pSelfInfo->bJump == true)
				//		//s_SE.pSound->Play(//s_SE.dog[0], CSound::CATEGORY::SE, false);

				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// �W�����v�\
				//	pSelfInfo->fMaxHeight = pOthColli[2].minPos.y;// �ō��x���W�ݒ�
				//}

				//pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
				//pSelfInfo->bExtendDog = true;
				break;

				//*********************************
				// ���ɓ�������
				//*********************************
			case ROT::LEFT:
				// �ʒu�E�ړ��ʏC��
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[2].minPos.x, &pSelfInfo->move.x, pOthColli[2].fWidth);
				break;

				//*********************************
				// �E�ɓ�������
				//*********************************
			case ROT::RIGHT:
				// �ʒu�E�ړ��ʏC��
				FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[2].maxPos.x, &pSelfInfo->move.x, pOthColli[2].fWidth);
				break;

				//*********************************
				// ���܂���
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}
}

//----------------------------
// �S�[���Q�[�g�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CCollision::GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	//if (!pSelfInfo->bGoal)
	//{
	//	pSelfInfo->bGoal = true;
	//
	//	const int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
	//
	//	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	//	{
	//		Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	//	}
	//}
}

//----------------------------
// �p�[�c�̓����蔻�菈��
//----------------------------
void CCollision::Parts(SelfInfo *pSelfInfo, CParts *pParts)
{
	//if (!pParts->GetDisp()) return;

	//// �擾�����̂ŕ`��OFF
	//pParts->DispSwitch(false);

	//// �擾������
	//CPlayer::s_nNumGetParts++;

	//// �擾���������S�̐��Ɠ����Ȃ�A���P�b�g��ԉ\
	//if (CPlayer::s_nNumGetParts == CParts::GetNumAll()) CPlayer::s_bRideRocket = true;
}

//----------------------------
// ���P�b�g�̓����蔻�菈��
//----------------------------
void CCollision::Rocket(SelfInfo *pSelfInfo, CRocket *pRocket)
{
	//const int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	//if (!CPlayer::s_bRideRocket) return;

	// ���P�b�g�ɓ���
	//pSelfInfo->bRide = true;
	//pRocket->Ride();
	//
	//for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	//{
	//	Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	//}
}

//----------------------------
// �Y�̓����蔻�菈��
//----------------------------
void CCollision::Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile)
{
	// ���������������Ƃɏ�����؂�ւ�
	switch (pColli->Rot)
	{
		//*********************************
		// ��ɓ�������
		//*********************************
		case ROT::OVER:
			// �\�̐��E�̃v���C���[
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}

			//	//������x�̍������痎�����Ă���
			//	if (pSelfInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
			//	{
			//		pPile->CaveInTrunkHeight(pColli->maxPos.y - pSelfInfo->pos.y - CPlayer::SIZE_HEIGHT);
			//	}

			//	pSelfInfo->move.y = 0.0f;
			//	pSelfInfo->bLandPile = true;// �����
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::UNDER:
			// ���̐��E�̃v���C���[
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	if (pSelfInfo->bJump == true)
			//	{// ���n����
			//	 // SE�Đ�
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}

			//	//������x�̍������痎�����Ă���
			//	if (pSelfInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
			//	{
			//		pPile->CaveInTrunkHeight(pColli->minPos.y - pSelfInfo->pos.y + CPlayer::SIZE_HEIGHT);
			//	}

			//	pSelfInfo->move.y = 0.0f;
			//	pSelfInfo->bLandPile = true;// �����
			//	pSelfInfo->bGround = true;	// �n�ʂɐڂ��Ă���
			//	pSelfInfo->bJump = false;	// �W�����v�\
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
			//}
			break;

			//*********************************
			// ���ɓ�������
			//*********************************
		case ROT::LEFT:
			// �ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// �E�ɓ�������
			//*********************************
		case ROT::RIGHT:
			// �ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// ���܂���
			//*********************************
		case ROT::UNKNOWN: break;// if (s_nSwapInterval != 0)	Death(&pSelfInfo->pos);
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
CCollision::ROT CCollision::IsBoxCollider(SelfInfo& self, ColliInfo& target, VEC value)
{
	// �����̌��݂̍ŏ��E�ő�ʒu
	self.minPos = Pos3D(self.pos.x - self.fWidth, self.pos.y - self.fHeight, 0.0f);
	self.maxPos = Pos3D(self.pos.x + self.fWidth, self.pos.y + self.fHeight, 0.0f);

	// �Ώۂ̌��݂̍ŏ��E�ő�ʒu
	target.minPos = Pos3D(target.pos.x - target.fWidth, target.pos.y - target.fHeight, 0.0f);
	target.maxPos = Pos3D(target.pos.x + target.fWidth, target.pos.y + target.fHeight, 0.0f);

	// �߂荞��ł��邩����
	if (self.minPos.x < target.maxPos.x && target.minPos.x < self.maxPos.x &&
		self.minPos.y < target.maxPos.y && target.minPos.y < self.maxPos.y)
	{
		// �����̉ߋ��̍ŏ��E�ő�ʒu
		const Pos3D OLD_MINPOS = Pos3D(self.posOld.x - self.fWidth, self.posOld.y - self.fHeight, 0.0f);
		const Pos3D OLD_MAXPOS = Pos3D(self.posOld.x + self.fWidth, self.posOld.y + self.fHeight, 0.0f);

		// �Ώۂ̑O��̍ŏ��E�ő�ʒu
		const Pos3D TARGET_MinPosOld = Pos3D(target.posOld.x - target.fWidth, target.posOld.y - target.fHeight, 0.0f);
		const Pos3D TARGET_MaxPosOld = Pos3D(target.posOld.x + target.fWidth, target.posOld.y + target.fHeight, 0.0f);

		// �Փ˃x�N�g���ŏ�������
		switch (value)
		{
			case VEC::X:
				// �O��͍�����߂荞��ł��Ȃ��i���͂߂荞��
				if (OLD_MAXPOS.x <= TARGET_MinPosOld.x)			return ROT::LEFT;

				// �O��͉E����߂荞��ł��Ȃ��i���͂߂荞��
				else if (OLD_MINPOS.x >= TARGET_MaxPosOld.x)	return ROT::RIGHT;

				//�Ώۂ����ɓ����Ă���
				else if (target.posOld.x > target.pos.x) return ROT::RIGHT;

				//�Ώۂ��E�ɓ����Ă���
				else if (target.posOld.x < target.pos.x) return ROT::LEFT;
				break;

			case VEC::Y:
				// �O��͏ォ��߂荞��ł��Ȃ��i���͂߂荞��
				if (OLD_MINPOS.y >= TARGET_MaxPosOld.y)			return ROT::OVER;

				// �O��͉�����߂荞��ł��Ȃ��i���͂߂荞��
				else if (OLD_MAXPOS.y <= TARGET_MinPosOld.y)	return ROT::UNDER;

				//�Ώۂ���ɓ����Ă���
				else if (target.posOld.y < target.pos.y) return ROT::OVER;

				//�Ώۂ����ɓ����Ă���
				else if (target.posOld.y > target.pos.y) return ROT::UNDER;
				break;
		}

		// ��������������������Ȃ�
		return ROT::UNKNOWN;
	}

	// ������Ȃ�����
	return ROT::NONE;
}