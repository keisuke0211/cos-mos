//========================================================
// 
// ���S�p�[�e�B�N������
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

//=======================================
// �R���X�g���N�^
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);
}

//=======================================
// �f�X�g���N�^
//=======================================
CEffect_Death::~CEffect_Death()
{
	Manager::EffectMgr()->SubList(this);
}

//=======================================
// �ݒ菈��
//=======================================
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int nLife, const int nTex, const TYPE type)
{
	m_Info.pos = pos;   m_Info.posOld = posOld; m_Info.move = move;
	m_Info.rot = rot;   m_Info.spin   = spin;
	m_Info.size = size; m_Info.color  = color;
	m_Info.nTex = nTex; m_Info.type   = type;
	m_Info.nLife = nLife;

	m_Info.CreateCounter = CREATE_COUNTER;

	if (type == TYPE::BALL)
	{
		m_Info.move.x = sinf(rot.z) * -8.0f;
		m_Info.move.y = cosf(rot.z) * -8.0f;
	}
}

//=======================================
//�X�V����
//=======================================
void CEffect_Death::Update(void)
{
	//�O��ʒu�X�V
	m_Info.posOld = m_Info.pos;

	//��ޕʍX�V����
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); break;
		case TYPE::INK: UpdateType_Ink();  break;
	}
	
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, INITVECTOR3D)
		->SetBillboard(true)
		->SetCol(INITCOLOR)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	Pos3D rot = m_Info.rot;
	rot.x = rot.z;
	rot.y = rot.z;
	rot.z = 0.0f;

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		//->SetBillboard(true)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	//�m�����n���ʍX�V����
	Life();
}

//=======================================
//�y�{�[���z�X�V����
//=======================================
void CEffect_Death::UpdateType_Ball(void)
{
	//��������Ă���̃J�E���^�[����
	if (m_Info.CreateCounter != 0)
		m_Info.CreateCounter--;

	//���ȏ��ݒ�
	CCollision::SelfInfo SelfInfo;

	//�����蔻��̏����i�[
	CCollision::ColliInfo colliInfo;
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
	{
		const CPlayer::VECTOL vec = (CPlayer::VECTOL)nCntVec;

		//�ړ�����
		Move(vec);

		//�v���C���[�̓����蔻��
		if (m_Info.CreateCounter == 0)
		{
			ColliRot = PlayerCollider(&SelfInfo, &colliInfo, vec);
			if (ColliRot != CCollision::ROT::NONE)
			{
				const Pos3D PosDiff = colliInfo.pos - SelfInfo.pos;

				//�v���C���[�܂ł̊p�x���擾
				const float fRot = atan2f(PosDiff.x, -PosDiff.y);

				//�ړ��ʐݒ�
				m_Info.move.x = sinf(fRot) * 3.0f;
				m_Info.move.y = cosf(fRot) * 3.0f;
			}
		}

		//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec);
		if (ColliRot != CCollision::ROT::NONE)
		{
			switch (ColliRot)
			{
				case CCollision::ROT::OVER:
				case CCollision::ROT::UNDER: m_Info.move.y *= -0.7f; break;
				case CCollision::ROT::LEFT:
				case CCollision::ROT::RIGHT: m_Info.move.x *= -0.7f; break;
			}
		}
	}

	//�ړ��x�N�g���̊p�x���Z�o
	const float fRot = atan2f(m_Info.move.x, -m_Info.move.y);
	m_Info.rot.z = fRot;

	//��]����
	Spin();
}

//=======================================
//�y�C���N�z�X�V����
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{
	
}

//=======================================
//�m�v���C���[�����蔻��n���X�V����
//=======================================
CCollision::ROT CEffect_Death::PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec)
{
	//�����̏��𔽉f
	SetSelfInfo(pSelfInfo);

	//���������������i�[
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//�v���C���[�擾
	CPlayer *pPlayer = CMode_Game::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
	{
		//�v���C���[��񔽉f
		CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);
		//�S�[�����Ă��� or ����ł���
		if (pInfo->bGoal || pInfo->bRide || pInfo->deathCounter != 0 || pInfo->deathCounter2 != 0)continue;
		pColliInfo->pos = pInfo->pos;             pColliInfo->posOld  = pInfo->posOld;
		pColliInfo->fWidth = CPlayer::SIZE_WIDTH; pColliInfo->fHeight = CPlayer::SIZE_HEIGHT;

		//�ʒu�̍�����T�C�Y�̑Ίp�����擾
		const float fposDiffLength = D3DXVec3Length(&(pColliInfo->pos - pSelfInfo->pos));
		const float fSizeLength = D3DXVec2Length(&D3DXVECTOR2(pSelfInfo->fWidth + pColliInfo->fWidth, pSelfInfo->fHeight + pColliInfo->fHeight));

		//�T�C�Y�̑Ίp����苗��������Ă���
		if (fposDiffLength > fSizeLength)
			continue;

		//�����蔻��
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//����������I��
		if (ColliRot != CCollision::ROT::NONE)break;
	}

	//���ʂ�Ԃ�
	return ColliRot;
}

//=======================================
//�m�X�e�[�W�I�u�W�F�N�g�����蔻��n���X�V����
//=======================================
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec)
{
	//�����̏��𔽉f
	SetSelfInfo(pSelfInfo);

	//���������������i�[
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) 
	{
		// �擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		CStageObject::TYPE type = stageObj->GetType();

		//�u���b�N�ȊO�͔��肵�Ȃ�
		if (stageObj->GetType() != CStageObject::TYPE::BLOCK) continue;

		//��񔽉f
		pColliInfo->pos    = stageObj->GetPos();   pColliInfo->posOld = stageObj->GetPosOld();
		pColliInfo->fWidth = stageObj->GetWidth() * 0.5f;
		pColliInfo->fHeight = stageObj->GetHeight() * 0.5f;

		//�����蔻��
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//����������I��
		if (ColliRot != CCollision::ROT::NONE)break;
	}

	//���ʂ�Ԃ�
	return ColliRot;
}

//=======================================
//�m�ړ��n���X�V����
//=======================================
void CEffect_Death::Move(CPlayer::VECTOL vec)
{
	//�ړ�
	switch (vec)
	{
			//�]�x�N�g���ړ�
		case CPlayer::VECTOL::X:
			m_Info.pos.x += m_Info.move.x;

			//�ړ��ʂ�����������
			m_Info.move.x += (0.0f - m_Info.move.x) * 0.01f;
			break;

			//�x�x�N�g���ړ�
		case CPlayer::VECTOL::Y:
			m_Info.pos.y += m_Info.move.y;

			//�\�̐��E�ɂ���
			if (m_Info.pos.y > 0.0f)
				m_Info.move.y -= 0.03f;
			//���̐��E�ɂ���
			else m_Info.move.y += 0.03f;
			break;
	}
}

//=======================================
//�m��]�n���X�V����
//=======================================
void CEffect_Death::Spin(void)
{
	m_Info.rot += m_Info.spin;
}

//=======================================
//�m�����n���X�V����
//=======================================
void CEffect_Death::Life(void)
{
	//���������݂��A�s����
	if (m_Info.nLife > 0 &&	--m_Info.nLife <= 0)
		CObject::Delete();

	if(RNLib::Input().GetKeyTrigger(DIK_O))
		CObject::Delete();
}

//=======================================
//���g�̓����蔻����ݒ�
//=======================================
void CEffect_Death::SetSelfInfo(CCollision::SelfInfo *pSelfInfo)
{
	//�����̏��𔽉f
	pSelfInfo->pos = m_Info.pos;    pSelfInfo->posOld = m_Info.posOld; pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = m_Info.size.x * 0.5f;
	pSelfInfo->fHeight = m_Info.size.y * 0.5f;
}