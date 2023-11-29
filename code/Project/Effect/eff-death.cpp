//========================================================
// 
// ���S�p�[�e�B�N������
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

const float CEffect_Death::CREATE_SPREAD_POWER = -8.0f; //�������̊g�U��
const float CEffect_Death::PLAYER_COLLI_POWER = 1.0f;   //�v���C���[�ɓ��������Ƃ��̐�����ї�
const float CEffect_Death::MOVE_X_CORRECT = 0.01f;      //�]�x�N�g���̈ړ��␳�W��
const float CEffect_Death::GRAVITY_POWER = 0.03f;       //�d�͉����x
const float CEffect_Death::BOUND_POWER = -0.7f;         //�o�E���h�W��
const short CEffect_Death::BALL_ALPHA_DECREASE = 10;    //�{�[���̃��l�����ʁi�����蔻���Unknown���o���ۂɎg�p

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
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int nLife, const int nIdx, const TYPE type)
{
	//��{���ݒ�
	m_Info.pos = pos;   m_Info.posOld = posOld; m_Info.move = move;
	m_Info.rot = rot;   m_Info.spin   = spin;
	m_Info.size = size; m_Info.color  = color;
	m_Info.nIdx = nIdx; m_Info.type   = type;
	m_Info.nLife = nLife;

	//�������Ă����v���C���[�ɓ�����Ȃ��悤�ɃJ�E���^�[���
	m_Info.CreateCounter = CREATE_COUNTER;
	m_Info.bDeath = false;

	if (type == TYPE::BALL)
	{//�{�[���̏ꍇ�A�g�U�͂�ݒ�
		m_Info.move.x = sinf(rot.z) * CREATE_SPREAD_POWER;
		m_Info.move.y = cosf(rot.z) * CREATE_SPREAD_POWER;
	}
}

//=======================================
//�X�V����
//=======================================
void CEffect_Death::Update(void)
{
	//�O��ʒu�X�V
	m_Info.posOld = m_Info.pos;

	//��ޕʍX�V�E�z�u����
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); PutModel();   break;//�{�[��
		case TYPE::INK: UpdateType_Ink();  PutPolygon(); break;//�C���N
	}

	//�m�����n���ʍX�V����
	Life();

	//�m���S�n���ʍX�V����
	Death();
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
				const float fRot = atan2f(-PosDiff.x, -PosDiff.y);

				const Pos3D PlayerMove = colliInfo.pos - colliInfo.posOld;

				//�ړ��ʐݒ�
				switch (vec)
				{
					case CPlayer::VECTOL::X:m_Info.move.x = sinf(fRot) * PLAYER_COLLI_POWER + PlayerMove.x;break;
					case CPlayer::VECTOL::Y:m_Info.move.y = cosf(fRot) * PLAYER_COLLI_POWER + PlayerMove.y * 0.7f;break;
				}
			}
		}

		//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
		CStageObject::TYPE type = CStageObject::TYPE::NONE;
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec, type);
		if (ColliRot != CCollision::ROT::NONE)
		{
			switch (type)
			{
					//�u���b�N�̔���
				case CStageObject::TYPE::BLOCK:
					//�������������ɂ�鏈��
					switch (ColliRot)
					{
						//���E�ɓ��������ꍇ
						case CCollision::ROT::LEFT:
						case CCollision::ROT::RIGHT: m_Info.move.x *= BOUND_POWER; break;

						//�㉺�ɓ��������ꍇ
						//�������x���قڂO�Ȃ�O�ɐݒ�
						//�Ⴄ�Ȃ瑬�x����߂A�o�E���h
						case CCollision::ROT::OVER:
						case CCollision::ROT::UNDER: m_Info.move.y *= BOUND_POWER;break;

						//��������������������Ȃ�
						case CCollision::ROT::UNKNOWN:
						{//���l�����������A�O�ȉ��Ŏ��S
							if (m_Info.color.a <= BALL_ALPHA_DECREASE)
								m_Info.bDeath = true;
							else m_Info.color.a -= BALL_ALPHA_DECREASE;
						}
					}break;

					//�����߃u���b�N�̔���
					//���S
				case CStageObject::TYPE::FILLBLOCK: m_Info.bDeath = true;break;
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

		//�v���C���[��񔽉f
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
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type)
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

		type = stageObj->GetType();

		//�u���b�N�ȊO�͔��肵�Ȃ�
		if (type != CStageObject::TYPE::BLOCK &&
			type != CStageObject::TYPE::FILLBLOCK) continue;

		//��񔽉f
		pColliInfo->pos    = stageObj->GetPos();   pColliInfo->posOld = stageObj->GetPosOld();
		pColliInfo->fWidth = stageObj->GetWidth() * 0.5f;
		pColliInfo->fHeight = stageObj->GetHeight() * 0.5f;

		//�����蔻��
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//����������I��
		if (ColliRot != CCollision::ROT::NONE)
			break;
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
			m_Info.move.x += (0.0f - m_Info.move.x) * MOVE_X_CORRECT;
			break;

			//�x�x�N�g���ړ�
		case CPlayer::VECTOL::Y:
			m_Info.pos.y += m_Info.move.y;

			//�\�̐��E�ɂ���
			if (m_Info.pos.y > 0.0f)
				m_Info.move.y -= GRAVITY_POWER;
			//���̐��E�ɂ���
			else m_Info.move.y += GRAVITY_POWER;
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
	if (m_Info.nLife > 0 && --m_Info.nLife <= 0)
		m_Info.bDeath = true;

	if(RNLib::Input().GetKeyTrigger(DIK_O))
		m_Info.bDeath = true;
}

//=======================================
//�|���S���z�u����
//=======================================
void CEffect_Death::PutPolygon(void)
{
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nIdx)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);
}

//=======================================
//���f���z�u����
//=======================================
void CEffect_Death::PutModel(void)
{
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nIdx, m_Info.pos, m_Info.rot, Vector3D(m_Info.size.x, m_Info.size.y, (m_Info.size.x + m_Info.size.y) * 0.5f))
		->SetZTest(false)
		->SetCol(m_Info.color);
}

//=======================================
//�m���S�n���X�V����
//=======================================
void CEffect_Death::Death(void)
{
	//���S�t���O�������Ă���
	if(m_Info.bDeath)
		CObject::Delete();
}

//=======================================
//���g�̓����蔻����ݒ�
//=======================================
void CEffect_Death::SetSelfInfo(CCollision::SelfInfo *pSelfInfo)
{
	//�����̏��𔽉f
	pSelfInfo->pos = m_Info.pos; pSelfInfo->posOld = m_Info.posOld;
	pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = CStageObject::SIZE_OF_1_SQUARE * m_Info.size.x * 0.5f;
	pSelfInfo->fHeight = CStageObject::SIZE_OF_1_SQUARE * m_Info.size.y * 0.5f;
}