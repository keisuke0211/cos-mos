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
const float CEffect_Death::BALL_SIZE[(int)BALL_SIZE_LV::MAX] = { //�{�[���T�C�Y���i�[
	0.3f, 0.6f, 1.0f
};

int         CEffect_Death::s_nNumAllBall = 0;
int         CEffect_Death::s_FusionSE = NONEDATA;
int         CEffect_Death::s_BallModelID[(int)BALL_SIZE_LV::MAX] = { NONEDATA, NONEDATA ,NONEDATA }; //�{�[�����f���ԍ����i�[
const char *CEffect_Death::BALL_MODEL_PATH[(int)BALL_SIZE_LV::MAX] = { //�{�[�����f���p�X���i�[
	"data\\MODEL\\Effect\\Ball_S.x",
	"data\\MODEL\\Effect\\Ball_M.x",
	"data\\MODEL\\Effect\\Ball_L.x"
};

//=======================================
// �R���X�g���N�^
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);

	m_type = CStageObject::TYPE::DEATH_EFF;
}

//=======================================
// �f�X�g���N�^
//=======================================
CEffect_Death::~CEffect_Death()
{
	if (m_Info.type == TYPE::BALL) {
		//�{�[���T�C�Y�p���x���J��
		if (m_Info.pLv != NULL) {
			delete m_Info.pLv;
			m_Info.pLv = NULL;
		}

		s_nNumAllBall--;
	}

	Manager::EffectMgr()->SubList(this);
}

//=======================================
// �ݒ菈��
//=======================================
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const int nIdx, const TYPE type)
{
	//��{���ݒ�
	m_pos = pos;        m_posOld    = posOld; m_Info.move = move;
	m_rot = rot;        m_Info.spin = spin;
	m_Info.size = size; m_color     = color;
	m_Info.nIdx = nIdx; m_Info.type = type;
	m_Info.nLife = nLife;

	//�������Ă����v���C���[�ɓ�����Ȃ��悤�ɃJ�E���^�[���
	m_Info.ColliderInterval = CREATE_INTERVAL;
	m_Info.bDeath = false;

	if (type == TYPE::BALL)
	{//�{�[���̏ꍇ�A�g�U�͂�ݒ�
		m_Info.move.x = sinf(rot.z) * CREATE_SPREAD_POWER;
		m_Info.move.y = cosf(rot.z) * CREATE_SPREAD_POWER;
		m_Info.nBallID = s_nNumAllBall++;
		if (s_FusionSE == NONEDATA)
			s_FusionSE = RNLib::Sound().Load("data\\SOUND\\SE\\fusion.wav");
		
		for (int nCntID = 0; nCntID < (int)BALL_SIZE_LV::MAX; nCntID++)
		{
			if (s_BallModelID[nCntID] == NONEDATA)
				s_BallModelID[nCntID] = RNLib::Model().Load(BALL_MODEL_PATH[nCntID]);
		}
	}
}

//=======================================
//�X�V����
//=======================================
void CEffect_Death::Update(void)
{
	//�O��ʒu�X�V
	m_posOld = m_pos;

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
	//�T�C�Y�w�肳��Ă��Ȃ������玀�S�t���O�𗧂ĂďI��
	if (m_Info.pLv == NULL) {
		m_Info.bDeath = true;
		m_Info.size = 0.0f;
		return;
	}

	//��������Ă���̃J�E���^�[����
	if (m_Info.ColliderInterval != 0)
		m_Info.ColliderInterval--;

	//���ȏ��ݒ�
	CCollision::SelfInfo SelfInfo;

	//�����蔻��̏����i�[
	CCollision::ColliInfo colliInfo;
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//�v���C���[�̓����蔻��
	if (m_Info.ColliderInterval == 0)
	{
		//�v���C���[�擾
		CPlayer *pPlayer = CMode_Game::GetPlayer();

		for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
		{
			//�v���C���[��񔽉f
			CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);

			//�S�[�����Ă��� or ����ł���
			if (pInfo->bGoal || pInfo->bRide || pInfo->deathCounter != 0 || pInfo->deathCounter2 != 0)continue;

			const Pos3D PlayerPos = pInfo->pos;
			const Pos3D PosDiff = PlayerPos - m_pos;
			const float fPosDiffLength = D3DXVec3Length(&PosDiff);
			const float fSizeLength = D3DXVec2Length(&D3DXVECTOR2(m_Info.size + CPlayer::SIZE_WIDTH, m_Info.size + CPlayer::SIZE_HEIGHT));

			if (fPosDiffLength < fSizeLength)
			{
				//�v���C���[�܂ł̊p�x���擾
				float fRot = atan2f(-PosDiff.x, -PosDiff.y);

				m_Info.move.x = sinf(fRot) * PLAYER_COLLI_POWER + pInfo->move.x;
				m_Info.move.y = cosf(fRot) * PLAYER_COLLI_POWER + pInfo->move.y * 0.7f;

				//�v���C���[���n�ʂɂ��鎞�R��΂�
				if (pInfo->bGround &&
					(pInfo->side == CPlayer::WORLD_SIDE::FACE && m_Info.move.y <= 0.0f) ||
					(pInfo->side == CPlayer::WORLD_SIDE::BEHIND && m_Info.move.y >= 0.0f))
				{
					m_Info.move.y *= -1.8f;
				}
				break;
			}
		}
	}

	for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
	{
		const CPlayer::VECTOL vec = (CPlayer::VECTOL)nCntVec;

		//�ړ�����
		Move(vec);

		//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
		CStageObject::TYPE type = CStageObject::TYPE::NONE;
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec, type);
		if (ColliRot != CCollision::ROT::NONE)
		{
			ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec, type);

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
							if (m_color.a <= BALL_ALPHA_DECREASE)
								m_Info.bDeath = true;
							else m_color.a -= BALL_ALPHA_DECREASE;
						}
					}break;

					//�����߃u���b�N�̔���
					//���S
				case CStageObject::TYPE::FILLBLOCK: m_Info.bDeath = true;break;
			}
		}
	}

	//�{�[�����̏���
	BallFusion();

	//�ړ��x�N�g���̊p�x���Z�o
	const float fRot = atan2f(m_Info.move.x, -m_Info.move.y);
	m_rot.z = fRot;

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
		ColliRot = CCollision::IsBoxToBoxCollider(*pSelfInfo, *pColliInfo, vec);

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
		ColliRot = CCollision::IsBoxToBoxCollider(*pSelfInfo, *pColliInfo, vec);

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
			m_pos.x += m_Info.move.x;

			//�ړ��ʂ�����������
			m_Info.move.x += (0.0f - m_Info.move.x) * MOVE_X_CORRECT;
			break;

			//�x�x�N�g���ړ�
		case CPlayer::VECTOL::Y:
			m_pos.y += m_Info.move.y;

			//�\�̐��E�ɂ���
			if (m_pos.y > 0.0f)
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
	m_rot += m_Info.spin;
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
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, m_rot)
		->SetTex(m_Info.nIdx)
		->SetCol(m_color)
		->SetSize(m_Info.size, m_Info.size)
		->SetZTest(false);
}

//=======================================
//���f���z�u����
//=======================================
void CEffect_Death::PutModel(void)
{
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nIdx, m_pos, m_rot)
		->SetCol(m_color);
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
	pSelfInfo->pos = m_pos; pSelfInfo->posOld = m_posOld;
	pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = pSelfInfo->fHeight = CStageObject::SIZE_OF_1_SQUARE * m_Info.size * 0.5f;
}

//******************************
//�{�[���p�ݒ菈��
//******************************
void CEffect_Death::SetBallSize(BALL_SIZE_LV Lv)
{
	//�T�C�Y���܂��w�肳��Ă��Ȃ�
	if (m_Info.pLv == NULL)
		m_Info.pLv = new BALL_SIZE_LV;

	//�T�C�Y�ݒ�
	*m_Info.pLv = Lv;
	m_Info.size = BALL_SIZE[(int)Lv];
	m_Info.nIdx = s_BallModelID[(int)Lv];
}

//******************************
//�{�[�����̏���
//���̃{�[���Ɠ��������ꍇ�A�����͏���
//******************************
void CEffect_Death::BallFusion(void)
{
	if (m_Info.ColliderInterval != 0) return;

	CObject* obj = NULL;
	while (Manager::EffectMgr()->ListLoop(&obj))
	{
		// �擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		//���S�G�t�F�N�g�ȊO�͔��肵�Ȃ�
		const CStageObject::TYPE type = stageObj->GetType();
		if (type != CStageObject::TYPE::DEATH_EFF) continue;

		//���S�G�t�F�N�g�ɃL���X�g
		//�{�[���ȊO�͖���
		CEffect_Death *pEff = (CEffect_Death *)stageObj;
		if (pEff->m_Info.type != CEffect_Death::TYPE::BALL || 
			pEff->m_Info.ColliderInterval != 0 ||
			pEff->m_Info.nBallID == m_Info.nBallID) continue;

		//�T�C�Y�̍��v�l
		const float SumSize = m_Info.size * CStageObject::SIZE_OF_1_SQUARE * 0.5f + pEff->m_Info.size * CStageObject::SIZE_OF_1_SQUARE * 0.5f;

		//�����̍����擾
		const Pos3D PosDiff = m_pos - pEff->m_pos;
		const float PosDiffLen = D3DXVec3Length(&PosDiff);

		//�T�C�Y�̍��v�l��������Ă���
		if (PosDiffLen > SumSize) continue;

		//�ǂ��炩�̃T�C�Y���x�����ő�
		if (*m_Info.pLv == BALL_SIZE_LV::BIG ||
			*pEff->m_Info.pLv == BALL_SIZE_LV::BIG)
		{
			//�����Ɏ��S�t���O�𗧂Ă�
			m_Info.bDeath = pEff->m_Info.bDeath = true;
		}
		else
		{
			//�T�C�Y���x���̍��v�l���擾
			const int SumSizeLv = (int)*m_Info.pLv + (int)*pEff->m_Info.pLv;

			switch ((BALL_SIZE_LV)SumSizeLv)
			{
				//���v�l�����E���̏ꍇ
				//���v�l�����i�グ��
				case BALL_SIZE_LV::SMALL:
				case BALL_SIZE_LV::MIDIUM:
					*pEff->m_Info.pLv = (BALL_SIZE_LV)(SumSizeLv + 1); 
					break;

				//���v�l����ȏ�̏ꍇ
				//�T�C�Y���ɐݒ�
				default:
					*pEff->m_Info.pLv = BALL_SIZE_LV::BIG;
					break;
			}

			//�����͏���
			m_Info.size = 0.0f;
			m_Info.bDeath = true;

			//����͏���ς���
			pEff->m_Info.size = BALL_SIZE[(int)*pEff->m_Info.pLv];
			pEff->m_posOld = pEff->m_pos;
			pEff->m_pos = m_pos + PosDiff * 0.5f; //2�̃{�[���̒��ԂɈʒu�ݒ�
			pEff->m_pos.y += 5.0f * pEff->m_Info.size;
			pEff->m_Info.move = INITVECTOR3D;
			pEff->m_Info.ColliderInterval = FUSION_INTERVAL;
			pEff->UpdateType_Ball();
			pEff->m_Info.nIdx = s_BallModelID[(int)*pEff->m_Info.pLv];
		}

		//SE�Đ�
		RNLib::Sound().Play(s_FusionSE, CSound::CATEGORY::SE, false);
		break;
	}
}