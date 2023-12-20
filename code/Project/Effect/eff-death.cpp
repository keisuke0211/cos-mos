//========================================================
// 
// ���S�p�[�e�B�N������
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"
#include "../stage.h"

CFloat CEffect_Death::CREATE_SPREAD_POWER = -4.0f; //�������̊g�U��
CFloat CEffect_Death::PLAYER_COLLI_POWER = 1.0f;   //�v���C���[�ɓ��������Ƃ��̐�����ї�
CFloat CEffect_Death::PLAYER_KICK_POWER = 2.0f;   //�v���C���[�̃L�b�N��
CFloat CEffect_Death::MOVE_X_CORRECT = 0.03f;      //�]�x�N�g���̈ړ��␳�W��
CFloat CEffect_Death::GRAVITY_POWER = 0.03f;       //�d�͉����x
CFloat CEffect_Death::BOUND_POWER = -0.9f;         //�o�E���h�W��
const short CEffect_Death::BALL_ALPHA_DECREASE = 10;    //�{�[���̃��l�����ʁi�����蔻���Unknown���o���ۂɎg�p
CFloat CEffect_Death::BALL_SIZE[(int)BALL_SIZE_LV::MAX] = { //�{�[���T�C�Y���i�[
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
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const TYPE type)
{
	//��{���ݒ�
	m_pos = pos;        m_posOld    = posOld; m_Info.move = move;
	m_rot = rot;        m_Info.spin = spin;
	m_Info.size = size; m_color     = color;
	m_Info.type = type;
	m_Info.nLife = nLife;

	//�������Ă����v���C���[�ɓ�����Ȃ��悤�ɃJ�E���^�[���
	m_Info.ColliderInterval = CREATE_INTERVAL;
	m_Info.bDeath = false;

	switch (type)
	{
		//�{�[���̏ꍇ�A�g�U�͂�ݒ�
		case TYPE::BALL:
		{
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
		}break;

		//�C���N�̏ꍇ
		case TYPE::INK:
		{
			m_Info.nLife = MAX_INK_LIFE;        // ���C�t�ݒ�
			m_Info.InkSize = Pos2D(size, size); // �T�C�Y�ݒ�
			if (m_Info.nIdx == NONEDATA)
				m_Info.nIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\ink001.png");
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
		CPlayer *pPlayer = Stage::GetPlayer();

		for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
		{
			//�v���C���[��񔽉f
			CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);

			//�S�[�����Ă��� or ����ł���
			if (pInfo->bGoal || pInfo->bRide || pInfo->deathCounter != 0 || pInfo->deathCounter2 != 0)continue;

			//�����蔻����ݒ�
			SetSelfInfo(&SelfInfo);
			colliInfo.pos = pInfo->pos;             colliInfo.posOld = pInfo->posOld;
			colliInfo.fWidth = CPlayer::SIZE_WIDTH; colliInfo.fHeight = CPlayer::SIZE_HEIGHT;

			//���������������i�[
			float fAngle = 0.0f;
			if (!CCollision::CircleToBoxCollider(SelfInfo, colliInfo, &fAngle)) continue;

			//�ʒu�C��
			float wh = D3DXVec2Length(&Pos2D(CPlayer::SIZE_WIDTH, CPlayer::SIZE_HEIGHT));
			CFloat SumSize = SelfInfo.fRadius * 0.5f + D3DXVec2Length(&Pos2D(CPlayer::SIZE_WIDTH, CPlayer::SIZE_HEIGHT));

			//�ړ��x�N�g��
			CFloat VecMove = D3DXVec3Length(&m_Info.move);
			float VecMovePower = VecMove * PLAYER_COLLI_POWER;
			m_Info.move.x = sinf(fAngle) * VecMovePower;
			m_Info.move.y = cosf(fAngle) * VecMovePower * 0.9f;

			//�v���C���[���n�ʂɂ��鎞�R��΂�
			if (pInfo->bGround &&
				(pInfo->side == CPlayer::WORLD_SIDE::FACE && m_Info.move.y <= 0.0f) ||
				(pInfo->side == CPlayer::WORLD_SIDE::BEHIND && m_Info.move.y >= 0.0f))
			{
				m_Info.move.x *= PLAYER_KICK_POWER;
				m_Info.move.y *= -PLAYER_KICK_POWER;
				m_Info.ColliderInterval = KICK_INTERVAL;
			}
			break;
		}
	}

	//�����蔻����ݒ�
	SetSelfInfo(&SelfInfo);

	for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
	{
		const CPlayer::VECTOL vec = (CPlayer::VECTOL)nCntVec;

		//�ړ�����
		Move(vec);

		//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
		CStageObject::TYPE type = CStageObject::TYPE::NONE;
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
					case CCollision::ROT::UNDER: m_Info.move.y *= BOUND_POWER; break;

						//��������������������Ȃ�
					case CCollision::ROT::UNKNOWN:
					{//���l�����������A�O�ȉ��Ŏ��S
						if (m_color.a <= BALL_ALPHA_DECREASE)
							m_Info.bDeath = true;
						else m_color.a -= BALL_ALPHA_DECREASE;
					}break;
				}break;

				//�����߃u���b�N�̔���
				//���S
			case CStageObject::TYPE::FILLBLOCK: m_Info.bDeath = true; break;
		}
	}

	//�{�[�����̏���
	BallFusion();

	//�ړ��x�N�g���̊p�x���Z�o
	//CFloat fRot = atan2f(m_Info.move.x, -m_Info.move.y);
	//m_rot.z = fRot;
}

//=======================================
//�y�C���N�z�X�V����
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{
	CFloat Rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_Info.nLife, MAX_INK_LIFE);
	m_color.a = 255 * Rate;

	//�C���N�������悤�ɃT�C�Y���g��
	m_Info.InkSize.y = m_Info.size + m_Info.size * 0.5f * (1.0f - Rate);

	//�d�͕����Ɋɂ₩�Ɉړ�
	m_pos.y += m_pos.y >= 0.0f ?
		-0.01f :
		0.01f;

	RNLib::Text2D().PutDebugLog(String("���C�t:%d  �����x:%d  ����:%.2f  ����:%.2f", m_Info.nLife, m_color.a, m_Info.InkSize.y, Rate));

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(m_Info.InkSize)
		->SetCol(m_color)
		->SetZTest(false)
		->SetTex(m_Info.nIdx);

	//�����x���O�Ȃ玀�S
	if (m_color.a <= 0) m_Info.bDeath = true;
}

//=======================================
//�m�X�e�[�W�I�u�W�F�N�g�����蔻��n���X�V����
//=======================================
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type)
{
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

		//���������������i�[
		if (!CCollision::CircleToBoxCollider(*pSelfInfo, *pColliInfo, NULL)) continue;
		
		ColliRot = CCollision::IsBoxToBoxCollider(*pSelfInfo, *pColliInfo, vec);
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

			if (fabsf(m_Info.move.x) <= 0.01f)
				m_Info.move.x = 0.0f;
			break;

			//�x�x�N�g���ړ�
		case CPlayer::VECTOL::Y:
			m_pos.y += m_Info.move.y;

			//�\�̐��E�ɂ���
			if (m_pos.y > 0.0f)
				m_Info.move.y -= GRAVITY_POWER;
			//���̐��E�ɂ���
			else m_Info.move.y += GRAVITY_POWER;

			if (fabsf(m_Info.move.y) <= 0.01f)
				m_Info.move.y = 0.0f;
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

#ifndef DEBUG
	if (RNLib::Input().GetKeyTrigger(DIK_O))
		m_Info.bDeath = true;
#endif // DEBUG
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
	pSelfInfo->pos = m_pos; 
	pSelfInfo->posOld = m_posOld;
	pSelfInfo->move = m_Info.move;
	pSelfInfo->fRadius = pSelfInfo->fWidth = pSelfInfo->fHeight = CStageObject::SIZE_OF_1_SQUARE * m_Info.size * 0.5f;
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

		//�����̍����擾
		const Pos3D PosDiff = m_pos - pEff->m_pos;
		CFloat PosDiffLen = D3DXVec3Length(&PosDiff);

		//�T�C�Y�̍��v�l��������Ă���
		if (PosDiffLen > m_Info.size * CStageObject::SIZE_OF_1_SQUARE) continue;

		//�ǂ��炩�̃T�C�Y���x�����ő�
		if (*m_Info.pLv == BALL_SIZE_LV::BIG || *pEff->m_Info.pLv == BALL_SIZE_LV::BIG)
		{
			//�����Ɏ��S�t���O�𗧂Ă�
			m_Info.bDeath = pEff->m_Info.bDeath = true;

			//�W�����`�Ƃ���p�[�e�B�N��
			CInt NUM_PARTI = 16;
			CInt PARTI_TEX = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Circle_005.png");
			const Pos3D PopPos = m_pos + PosDiff * 0.5f;

			for (int nCntParti = 0; nCntParti < NUM_PARTI; nCntParti++)
			{
				//���ݒn����̊g�U��
				const Pos3D Spread(
					(rand() % 2000 - 1000.0f) / 100.0f,
					(rand() % 2000 - 1000.0f) / 100.0f,
					0.0f);

				//�G�t�F�N�g����
				Manager::EffectMgr()->ParticleCreate(PARTI_TEX, PopPos + Spread, INITSCALE3D * (rand() % 700) / 100.0f, m_color,
													 CParticle::TYPE::TYPE_FLOATUP, 1000);
			}
		}
		else
		{
			//�T�C�Y���x���̍��v�l���擾
			BALL_SIZE_LV SumSizeLv = (BALL_SIZE_LV)((int)*m_Info.pLv + (int)*pEff->m_Info.pLv);

			//���v�l�����E���̏ꍇ�A��i�K�グ��
			if(SumSizeLv == BALL_SIZE_LV::SMALL || SumSizeLv == BALL_SIZE_LV::MIDIUM)
				*pEff->m_Info.pLv = (BALL_SIZE_LV)((int)SumSizeLv + 1);

			//����ȊO�́y��z�ɐݒ�
			else *pEff->m_Info.pLv = BALL_SIZE_LV::BIG;

			//*******************************
			// �����͏���
			//*******************************
			m_Info.size = 0.0f;
			m_Info.bDeath = true;

			//*******************************
			//����͏���ς���
			//*******************************
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
		RNLib::Sound().Play(s_FusionSE, _RNC_Sound::CATEGORY::SE, 1.0f, false);
		break;
	}
}