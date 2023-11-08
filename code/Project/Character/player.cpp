//================================================================================================
//
//�v���C���[����[player.cpp]
//Author:Hirasawa Shion
//
//================================================================================================
#include "../main.h"
#include "player.h"
#include "../../_RNLib/Basis/input.h"
#include "../../_RNLib/Basis/Calculation/number.h"

//�X���b�v�C���^�[�o��
const int	CPlayer::SWAP_INTERVAL = 30;	//�X���b�v�C���^�[�o��
int			CPlayer::s_nSwapInterval = 0;	//�c��X���b�v�C���^�[�o��

const float CPlayer::SIZE_WIDTH = 8.0f;	//����
const float CPlayer::SIZE_HEIGHT = 8.0f;//����

const float CPlayer::MOVE_SPEED = 0.5f;		//�ړ���
const float CPlayer::MAX_MOVE_SPEED = 2.7f;	//�ő�ړ���

const float CPlayer::JUMP_POWER = 5.0f;		//��{�W�����v��
const float CPlayer::GRAVITY_POWER = -0.3f;	//��{�d�͉����x

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

int			CPlayer::s_nNumGetParts = 0;	//�擾�����p�[�c�̐�
bool		CPlayer::s_bRideRocket = false;	//���P�b�g�ɏ��邩�ǂ���

int CPlayer::s_nSwapMarkTex = 0;  //�X���b�v��̃}�[�N�e�N�X�`���ԍ�
int CPlayer::s_nSwapParticle = 0; //�X���b�v���̃p�[�e�B�N���e�N�X�`���ԍ�

//=======================================
//�R���X�g���N�^
//=======================================
CPlayer::CPlayer()
{
	s_nSwapMarkTex = 0;		//�X���b�v��̃}�[�N�e�N�X�`���ԍ�
	s_nSwapParticle = 0;	//�X���b�v���̃p�[�e�B�N���e�N�X�`���ԍ�
	m_jumpSEIdx = 0;		//�W�����v����SE�ԍ�
	m_landingSEIdx = 0;		//���n����SE�ԍ�
	s_nSwapInterval = 0;	//�c��X���b�v�C���^�[�o��
	s_nNumGetParts = 0;		//�擾�����p�[�c�̐�
	s_bRideRocket = false;	//���P�b�g�ɏ��邩�ǂ���

	for each(Info &Player in m_aInfo)
	{
		Player.StartPos = INITD3DXVECTOR3;		//�J�n�ʒu
		Player.pos = INITD3DXVECTOR3;			//�ʒu
		Player.posOLd = INITD3DXVECTOR3;		//�O��ʒu
		Player.rot = INITD3DXVECTOR3;			//����
		Player.move = INITD3DXVECTOR3;			//�ړ���
		Player.color = INITCOLOR;				//�F
		Player.bGround = false;					//�n�ʂɐڂ��Ă��邩
		Player.bJump = false;					//�W�����v
		Player.bRide = false;					//���P�b�g�ɏ���Ă��邩�ǂ���
		Player.bGoal = false;					//�S�[���������ǂ���
		Player.fJumpPower = 0.0f;				//�W�����v��
		Player.fGravity = 0.0f;					//�d��
		Player.fMaxHeight = 0.0f;				//�ō��x���W
		Player.nTramJumpCounter = 0;			//�g�����|�����ɂ���Ē��ˏオ��ō����B�n�_
		Player.fTramTargetPosY = 0.0f;			//�g�����|�����p�̖ڕW�ʒu
		Player.bTramJump = false;				//�g�����|�����p�̓���W�����v
		Player.TramColliRot = COLLI_ROT::NONE;	//�g�����|�����p�̔���
		Player.bExtendDog = false;				//�k�C�p�̐ڐG�t���O
		Player.nModelIdx = NONEDATA;			//���f���ԍ�
		Player.side = WORLD_SIDE::FACE;			//�ǂ���̐��E�ɑ��݂��邩
	}

	pOthColli = NULL;		// ���p�[�c�̓����蔻����
	bPluralColli = false;	// �p�[�c��ށ@�P�̂�������
}

//=======================================
//�f�X�g���N�^
//=======================================
CPlayer::~CPlayer()
{
	if (pOthColli != NULL) {
		delete[] pOthColli;
		pOthColli = NULL;
	}
}

//=======================================
//��������
//=======================================
CPlayer *CPlayer::Create(void)
{
	//�v���C���[�C���X�^���X�̐���
	CPlayer *pPlayer = new CPlayer;

	//����������
	pPlayer->Init();

	//�v���C���[��Ԃ�
	return pPlayer;
}

//=====================================================================================================================
//����������
//=====================================================================================================================
HRESULT CPlayer::Init(void)
{
	//�P�o�������
	m_aInfo[0].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Human.x");

	//�Q�o�������
	m_aInfo[1].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Octopus.x");
	m_aInfo[1].rot.z = D3DX_PI;

	//�L�[�R���t�B�O������
	InitKeyConfig();

	s_nSwapMarkTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Circle_005.png");
	s_nSwapParticle = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");

	m_jumpSEIdx = RNLib::Sound().Load("data\\SOUND\\SE\\jamp_01.wav");
	m_landingSEIdx = RNLib::Sound().Load("data\\SOUND\\SE\\jamp.wav");

	m_dogSEIdx[0] = RNLib::Sound().Load("data\\SOUND\\SE\\heron2.wav");	//����
	m_dogSEIdx[1] = RNLib::Sound().Load("data\\SOUND\\SE\\shrink.wav");	//�k��
	m_dogSEIdx[2] = RNLib::Sound().Load("data\\SOUND\\SE\\extend.wav");	//�L�т�
	m_dogSEIdx[3] = RNLib::Sound().Load("data\\SOUND\\SE\\vibration.wav");	//�k����

	//�������ݒ�
	Death(NULL);

	//����������
	return S_OK;
}

//=======================================
//�e�v���C���[�̃L�[�{�[�h�E�W���C�p�b�h�̃L�[�R���t�B�O�������ݒ�
//=======================================
void CPlayer::InitKeyConfig(void)
{
	//�ǂ���̐��E�ł����ʂ̃L�[�z�u�ݒ�
	for (int nCntSide = 0; nCntSide < (int)WORLD_SIDE::MAX; nCntSide++)
	{
		//�P�o�̃L�[�{�[�h�z�u
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_A;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_D;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		//�Q�o�̃L�[�{�[�h�z�u
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_LEFTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_RIGHTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		//���E�ŕς��L�[�z�u�ݒ�
		switch ((WORLD_SIDE)nCntSide)
		{
		case WORLD_SIDE::FACE:
			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_W;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_UPARROW;

			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_S;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_DOWNARROW;
			break;

		case WORLD_SIDE::BEHIND:
			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_S;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::JUMP] = DIK_DOWNARROW;

			m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_W;
			m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::SWAP] = DIK_UPARROW;
			break;
		}

	}

	//�W���C�p�b�h�̐ݒ�͗��ҋ���
	for each(Info &Player in m_aInfo)
	{
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = CInput::BUTTON::LEFT;  //���ړ�
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = CInput::BUTTON::RIGHT; //�E�ړ�
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = CInput::BUTTON::A;     //�W�����v
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = CInput::BUTTON::Y;     //�X���b�v
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = CInput::BUTTON::A;     //����
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = CInput::BUTTON::START; //�|�[�Y
	}
}

//=====================================================================================================================
//�I������
//=====================================================================================================================
void CPlayer::Uninit(void)
{

}

//=======================================
//�O��ʒu�X�V����
//=======================================
void CPlayer::SetPosOld(void)
{
	//�v���C���[�̑O��ʒu�X�V
	for each (Info &Player in m_aInfo)
	{
		//���P�b�g�ɏ���Ă��Ȃ��Ȃ�@or �S�[�����Ă��Ȃ��Ȃ�
		if (!Player.bRide || Player.bGoal)
		Player.posOLd = Player.pos;
	}
}

//=====================================================================================================================
//�X�V����
//=====================================================================================================================
void CPlayer::Update(void)
{
	//�O��ʒu�X�V
	SetPosOld();

	//���쏈��
	ActionControl();

	//���҂Ƃ��ɃS�[�����ĂȂ�������
	if (!m_aInfo[0].bGoal && !m_aInfo[1].bGoal)
	{
		//�X���b�v
		Swap();
	}
	else if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
	{
		int planet = Manager::StgEd()->GetPlanetIdx();
		int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

	//�����蔻��܂Ƃ�
	WholeCollision();

	//���X�V
	UpdateInfo();

	RNLib::Text2D().PutDebugLog(CreateText("FPS:%d", RNLib::GetFPSCount()));
}

//----------------------------
//���X�V����
//----------------------------
void CPlayer::UpdateInfo(void)
{
	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;
		//���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		//�ʒu�ݒ�
		RNLib::Model().Put(Player.pos, Player.rot, Player.nModelIdx, false)
			->SetOutLine(true)
			->SetCol(Player.color);

		//�X���b�v��̃}�[�N��`�悷��ʒu
		D3DXVECTOR3 MarkPos = Player.pos;
		MarkPos.z = -10.0f;
		MarkPos.y *= -1.0f;

		RNLib::Polygon3D().Put(MarkPos, INITD3DXVECTOR3)
			->SetSize(20.0f, 20.0f)
			->SetBillboard(true)
			->SetTex(s_nSwapMarkTex)
			->SetCol(Color{ Player.color.r, Player.color.g, Player.color.b, 100 })
			->SetPriority(1);

		//�ō��x���W�X�V
		switch (Player.side)
		{
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
		RNLib::Text2D().PutDebugLog(CreateText("%dP�ō�Y���W�F%f", nCntPlayer, Player.fMaxHeight));
	}
}

//----------------------------
//���쏈��
//----------------------------
void CPlayer::ActionControl(void)
{
	//�v���C���[�ԍ�
	int nIdxPlayer = -1;

	for each (Info &Player in m_aInfo)
	{
		//���̃v���C���[�ԍ���
		nIdxPlayer++;

		// �o��
		if ((Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bRide = false;
			Player.bGoal = false;
			Player.pos.x = Player.pos.x - 20.0f;
		}

		//���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		//�W�����v���́i�󒆂���Ȃ��j
		if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bGround = false;				//�n�ʂ��痣�ꂽ
			Player.move.y = Player.fJumpPower;	//�W�����v�ʑ��
			Player.bJump = true;				//�W�����v����

												//SE�Đ�
			RNLib::Sound().Play(m_jumpSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
		}

		//�E�Ɉړ�
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer))
			Player.move.x += MOVE_SPEED;

		//���Ɉړ�
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer))
			Player.move.x -= MOVE_SPEED;

		//�X���b�v����
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
			Manager::EffectMgr()->ParticleCreate(s_nSwapParticle, Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
	}
}

//############################
//�X���b�v����
//############################
void CPlayer::Swap(void)
{
	//�C���^�[�o��������Ό��������ďI��
	if (s_nSwapInterval > 0)
	{
		s_nSwapInterval--;
		return;
	}


	//���҂Ƃ��ɃX���b�v�{�^���������Ă���܂��͂ǂ��炩�����P�b�g�ɏ���Ă���
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) || m_aInfo[0].bRide) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) || m_aInfo[1].bRide))
	{
		//�C���^�[�o���ݒ�
		s_nSwapInterval = SWAP_INTERVAL;

		for each (Info &Player in m_aInfo)
		{
			//���P�b�g�ɏ���Ă���X�L�b�v
			if (Player.bRide) continue;

			for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(s_nSwapParticle, Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}

			//�ʒu�E�d�͉����x�E�W�����v�ʁE���݂��鐢�E�𔽓]
			Player.pos.y *= -1.0f;
			Player.fGravity *= -1.0f;
			Player.fJumpPower *= -1.0f;
			Player.side = (WORLD_SIDE)(((int)Player.side + 1) % (int)WORLD_SIDE::MAX);

			if (Player.side == WORLD_SIDE::FACE)
				Player.rot.z = 0.0f;
			else if (Player.side == WORLD_SIDE::BEHIND)
				Player.rot.z = D3DX_PI;
		}

		//�O��ʒu�X�V
		SetPosOld();
	}
}

//----------------------------
//���S����
//----------------------------
void CPlayer::Death(D3DXVECTOR3 *pDeathPos)
{
	int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	if (pDeathPos != NULL)
	{
		Manager::EffectMgr()->EffectCreate(EffTex, *pDeathPos, INIT_EFFECT_SCALE, Color{ 255,0,255,255 });

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(ParTex, *pDeathPos, INIT_EFFECT_SCALE * 0.5f, Color{ 255,0,0,255 });
		}
	}

	//�P�o�p�������
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;

	//�Q�o�p�������
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;

	//���ҋ��ʏ������
	for each (Info &Player in m_aInfo)
	{
		Player.posOLd = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
	}
}

//----------------------------
//�ړ�����
//----------------------------
void CPlayer::Move(COLLI_VEC vec)
{
	//�v���C���[�̈ʒu�X�V
	for each (Info &Player in m_aInfo)
	{
		//���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		//�ړ��ʔ��f
		switch (vec)
		{
		case COLLI_VEC::X:
			//��������
			Player.move.x += (0.0f - Player.move.x) * 0.1f;

			//�]�̈ړ��ʂ��C��
			FloatControl(&Player.move.x, MAX_MOVE_SPEED, -MAX_MOVE_SPEED);

			//�ʒu�X�V
			Player.pos.x += Player.move.x;
			break;

			//�d�͏���
		case COLLI_VEC::Y:

			//�g�����|�����ɂ�����W�����v��
			if (Player.bTramJump)
			{//�J�E���^�[�����炵�āA�O�ɂȂ��������W�����v�I��
				
				float diff = Player.fTramTargetPosY - Player.pos.y;

				if (--Player.nTramJumpCounter <= 0)
				{
					Player.bTramJump = false;
					Player.move.y = diff / 1;
				}
				else
				{
					Player.move.y = diff / Player.nTramJumpCounter;
				}
			}
			//�ʏ펞�Ȃ�A�d�͏����łx�̈ړ��ʂ��v�Z
			else Player.move.y += Player.fGravity;

			//�ʒu�X�V
			Player.pos.y += Player.move.y;

			break;
		}
	}
}

//----------------------------
//�����蔻��܂Ƃ�
//----------------------------
void CPlayer::WholeCollision(void)
{
	//��U���v���C���[�Ƃ��ɃW�����v�s��
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++) {

		//�Փ˃x�N�g�����L���X�g
		const COLLI_VEC vec = (COLLI_VEC)nCntVec;

		//�ړ�����
		Move(vec);

		//�I�u�W�F�N�g�̃|�C���^���i�[
		CObject *obj = NULL;

		//�I�u�W�F�N�g���擾
		while (Manager::BlockMgr()->ListLoop(&obj)) {
			//�擾�����I�u�W�F�N�g���L���X�g
			CStageObject* stageObj = (CStageObject*)obj;

			//�����蔻��̏��
			Colli m_aColli;

			//�I�u�W�F�N�g�̓����蔻����擾
			m_aColli.pos = stageObj->GetPos();
			m_aColli.fWidth = stageObj->GetWidth() * 0.5f;
			m_aColli.fHeight = stageObj->GetHeight() * 0.5f;

			//�I�u�W�F�N�g�̍ŏ��E�ő�ʒu
			m_aColli.MinPos = D3DXVECTOR3(m_aColli.pos.x - m_aColli.fWidth, m_aColli.pos.y - m_aColli.fHeight, 0.0f);
			m_aColli.MaxPos = D3DXVECTOR3(m_aColli.pos.x + m_aColli.fWidth, m_aColli.pos.y + m_aColli.fHeight, 0.0f);

			for each(Info& Player in m_aInfo)
			{
				//���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
				if (Player.bRide || Player.bGoal) continue;

				//��ގ擾
				const CStageObject::TYPE type = stageObj->GetType();

				//�v���C���[�̋߂��ɃI�u�W�F�N�g�����邩����
				//�����I�u�W�F�N�g�ȊO�̔���
				if (type != CStageObject::TYPE::TRAMPOLINE && type != CStageObject::TYPE::LASER && type != CStageObject::TYPE::EXTEND_DOG) {
					if (D3DXVec3Length(&(m_aColli.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(m_aColli.fWidth + SIZE_WIDTH, m_aColli.fHeight + SIZE_HEIGHT))) continue;
				}

				//�O��ʒu
				m_aColli.posOLd = m_aColli.pos;

				//���p�[�c�̓����蔻��t���O
				bool bOtherColl = false;

				//�ړ�����I�u�W�F�N�g�́A�O��ʒu����ʂɐݒ�
				switch (type)
				{
					//�ړ���
				case CStageObject::TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock *)stageObj;
					m_aColli.pos = pBlock->GetPosOld();
				}
				break;

				// �g�����|����
				case CStageObject::TYPE::TRAMPOLINE:{
					CTrampoline *pTrampoline = (CTrampoline *)stageObj;

					OthColliDelete();
					bPluralColli = true;
					pOthColli = new Colli[OBJ_TRAMPOLINE];

					for (int nCnt = 0; nCnt < OBJ_TRAMPOLINE; nCnt++) {

						pOthColli[nCnt].pos = pTrampoline->GetSpringPos(nCnt);
						pOthColli[nCnt].posOLd = pOthColli[nCnt].pos;
						pOthColli[nCnt].fWidth = pTrampoline->GetWidth() * 1.0f;
						pOthColli[nCnt].fHeight = pTrampoline->GetHeight() * 1.0f;
						pOthColli[nCnt].ColliRot = COLLI_ROT::NONE;

						if (D3DXVec3Length(&(pOthColli[nCnt].pos - Player.pos)) >
							D3DXVec2Length(&D3DXVECTOR2(pOthColli[nCnt].fWidth + SIZE_WIDTH, pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

						pOthColli[nCnt].fWidth = pTrampoline->GetWidth() * 0.5f;
						pOthColli[nCnt].fHeight = pTrampoline->GetHeight() * 0.5f;

						//�I�u�W�F�N�g�̍ŏ��E�ő�ʒu
						pOthColli[nCnt].MinPos = D3DXVECTOR3(pOthColli[nCnt].pos.x - pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y - pOthColli[nCnt].fHeight, 0.0f);
						pOthColli[nCnt].MaxPos = D3DXVECTOR3(pOthColli[nCnt].pos.x + pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y + pOthColli[nCnt].fHeight, 0.0f);

						//���������������i�[
						COLLI_ROT ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli[nCnt].pos, pOthColli[nCnt].posOLd, pOthColli[nCnt].fWidth, pOthColli[nCnt].fHeight, vec);

						if (ColliRot != COLLI_ROT::NONE && !bOtherColl) {
							bOtherColl = true;
							Player.TramColliRot = ColliRot;
							pOthColli[nCnt].ColliRot = ColliRot;
						}
						else if (ColliRot == COLLI_ROT::NONE && !bOtherColl){
							Player.TramColliRot = COLLI_ROT::NONE;
						}
					}
				}
				break;

				//覐�
				case CStageObject::TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					m_aColli.pos = pMeteor->GetPosOld();
				}
				break;

				//���[�U�[
				case CStageObject::TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					OthColliDelete();
					pOthColli = new Colli;
					bPluralColli = false;

					pOthColli->pos = pLaser->GetLaserPos();
					pOthColli->posOLd = pOthColli->pos;
					pOthColli->fWidth = pLaser->GetLaserSize().x * 0.5f;
					pOthColli->fHeight = pLaser->GetLaserSize().y * 0.5f;

					if (D3DXVec3Length(&(pOthColli->pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(pOthColli->fWidth + SIZE_WIDTH, pOthColli->fHeight + SIZE_HEIGHT))) continue;

					//�I�u�W�F�N�g�̍ŏ��E�ő�ʒu
					pOthColli->MinPos = D3DXVECTOR3(pOthColli->pos.x - pOthColli->fWidth, pOthColli->pos.y - pOthColli->fHeight, 0.0f);
					pOthColli->MaxPos = D3DXVECTOR3(pOthColli->pos.x + pOthColli->fWidth, pOthColli->pos.y + pOthColli->fHeight, 0.0f);

					//���������������i�[
					pOthColli->ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli->pos, pOthColli->posOLd, pOthColli->fWidth, pOthColli->fHeight, vec);

					if (pOthColli->ColliRot != COLLI_ROT::NONE && !bOtherColl) {
						bOtherColl = true;
					}
				}
				break;

				//�k�C
				case CStageObject::TYPE::EXTEND_DOG:{
					CExtenddog *pDog = (CExtenddog *)stageObj;

					OthColliDelete();
					pOthColli = new Colli[OBJ_EXTENDDOG];
					bPluralColli = true;

					for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
						switch (nCnt){
						case 0:
							pOthColli[nCnt].pos = pDog->GetHeadPos();
							pOthColli[nCnt].posOLd = pDog->GetHeadPosOid();
							break;
						case 1:
							pOthColli[nCnt].pos = pDog->GetBodyPos();
							pOthColli[nCnt].posOLd = pDog->GetBodyPos();
							break;
						case 2:
							pOthColli[nCnt].pos = pDog->GetHipPos();
							pOthColli[nCnt].posOLd = pDog->GetHipPos();
							break;
						}

						pOthColli[nCnt].fWidth = pDog->GetWidth() * 0.5f;
						pOthColli[nCnt].fHeight = pDog->GetHeight() * 0.5f;

						if (D3DXVec3Length(&(pOthColli[nCnt].pos - Player.pos)) >
							D3DXVec2Length(&D3DXVECTOR2(pOthColli[nCnt].fWidth + SIZE_WIDTH, pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

						//�I�u�W�F�N�g�̍ŏ��E�ő�ʒu
						pOthColli[nCnt].MinPos = D3DXVECTOR3(pOthColli[nCnt].pos.x - pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y - pOthColli[nCnt].fHeight, 0.0f);
						pOthColli[nCnt].MaxPos = D3DXVECTOR3(pOthColli[nCnt].pos.x + pOthColli[nCnt].fWidth, pOthColli[nCnt].pos.y + pOthColli[nCnt].fHeight, 0.0f);

						//���������������i�[
						pOthColli[nCnt].ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, pOthColli[nCnt].pos, pOthColli[nCnt].posOLd, pOthColli[nCnt].fWidth, pOthColli[nCnt].fHeight, vec);

						if (pOthColli[nCnt].ColliRot != COLLI_ROT::NONE && !bOtherColl){
							bOtherColl = true;
						}
					}

					CExtenddog::STATE state = pDog->GetState();
					if (pOthColli[2].ColliRot != COLLI_ROT::OVER && Player.bExtendDog && state == CExtenddog::STATE::DOWN_LAND) {
						Player.bExtendDog = false;
					}
				}
				break;
				}

				//���������������i�[
				m_aColli.ColliRot = IsBoxCollider(Player.pos, Player.posOLd, m_aColli.fWidth, m_aColli.fHeight, m_aColli.pos, m_aColli.pos, m_aColli.fWidth, m_aColli.fHeight, vec);

				//�������Ă��Ȃ���΃X�L�b�v
				if (m_aColli.ColliRot == COLLI_ROT::NONE && !bOtherColl) continue;

				//��ނ��ƂɊ֐�����
				switch (type)
				{
				case CStageObject::TYPE::BLOCK:			CollisionBlock(&Player, &m_aColli);	break;
				case CStageObject::TYPE::FILLBLOCK:		CollisionFillBlock(&Player, m_aColli.ColliRot); break;
				case CStageObject::TYPE::TRAMPOLINE:	CollisionTrampoline(&Player, &m_aColli, pOthColli);	break;
				case CStageObject::TYPE::SPIKE:			CollisionSpike(&Player, &m_aColli);	break;
				case CStageObject::TYPE::MOVE_BLOCK:	CollisionMoveBlock(&Player, (CMoveBlock *)stageObj, &m_aColli);	break;
				case CStageObject::TYPE::METEOR:		CollisionMeteor(&Player, &m_aColli); break;
				case CStageObject::TYPE::LASER:			CollisionLaser(&Player, (CRoadTripLaser *)stageObj,&m_aColli, pOthColli);	break;
				case CStageObject::TYPE::EXTEND_DOG:	CollisionDog(&Player, (CExtenddog *)stageObj, &m_aColli, pOthColli); break;
				case CStageObject::TYPE::GOALGATE:		CollisionGoalGate(&Player, &m_aColli);	break;
				case CStageObject::TYPE::PARTS:			CollisionParts(&Player, (CParts *)stageObj); break;
				case CStageObject::TYPE::ROCKET:		CollisionRocket(&Player, (CRocket *)stageObj); break;
				}

				//������Α����̃I�u�W�F�N�g�ɓ������Ă���
				if (type == CStageObject::TYPE::SPIKE || type == CStageObject::TYPE::METEOR || type == CStageObject::TYPE::LASER)
					break;

				OthColliDelete();
			}

			// �k�C�̏�Ԑݒ�
			CExtenddog *pDog = (CExtenddog *)stageObj;
			CExtenddog::STATE state = pDog->GetState();
			if (!m_aInfo[0].bExtendDog && !m_aInfo[1].bExtendDog && state == CExtenddog::STATE::DOWN_LAND)
			{
				pDog->SetState(CExtenddog::STATE::RETURN);
			}
		}
	}
}

//----------------------------
//�ォ��̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	//�ʒu�E�ړ��ʏC��
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
//������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	//�ʒu�E�ړ��ʏC��
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
//������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	//�ʒu�E�ړ��ʏC��
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
//�E����̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	//�ʒu�E�ړ��ʏC��
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// �u���b�N�̓����蔻�菈��
//----------------------------
void CPlayer::CollisionBlock(Info *pInfo, Colli *pColli)
{
	//���������������Ƃɏ�����؂�ւ�
	switch (pColli->ColliRot)
	{
		//*********************************
		//��ɓ�������
		//*********************************
	case COLLI_ROT::OVER:
		//�ʒu�E�ړ��ʏC��
		FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

		//�\�̐��E�̃v���C���[
		if (pInfo->side == WORLD_SIDE::FACE) {
			if (pInfo->bJump == true)
			{//���n����
				//SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pColli->MaxPos.y;//�ō��x���W�ݒ�pInfo->bJump = false
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::UNDER:
		//�ʒu�E�ړ��ʏC��
		FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

		//���̐��E�̃v���C���[�Ȃ�W�����v�\
		if (pInfo->side == WORLD_SIDE::BEHIND) {
			if (pInfo->bJump == true)
			{//���n����
				//SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pColli->MinPos.y;//�ō��x���W�ݒ�
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::LEFT:
		//�ʒu�E�ړ��ʏC��
		FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//�E�ɓ�������
		//*********************************
	case COLLI_ROT::RIGHT:
		//�ʒu�E�ړ��ʏC��
		FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//���܂���
		//*********************************
	case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
	}
}

//----------------------------
// �����߃u���b�N�̓����蔻�菈��
//----------------------------
void CPlayer::CollisionFillBlock(Info *pInfo,COLLI_ROT ColliRot)
{
	Death(&pInfo->pos);
}

//----------------------------
//�g�����|�����̓����蔻�菈��
//----------------------------
void CPlayer::CollisionTrampoline(Info *pInfo, Colli *pColli, Colli *pOthColli)
{
	//���������������Ƃɏ�����؂�ւ�
	switch (pColli->ColliRot)
	{
		//*********************************
		//��ɓ�������
		//*********************************
	case COLLI_ROT::OVER:
		//�ʒu�E�ړ��ʏC��
		FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y, pOthColli[0].fHeight);
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::UNDER:
		//�ʒu�E�ړ��ʏC��
		FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);
		break;
	}

	// �΂ˏ�
	switch (pOthColli[0].ColliRot)
	{
		//*********************************
		//��ɓ�������
		//*********************************
	case COLLI_ROT::OVER:
		//�ʒu�E�ړ��ʏC��
		FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y,pOthColli[0].fHeight);

		//�\�̐��E�̃v���C���[
		if (pInfo->side == WORLD_SIDE::FACE) {
			if (pInfo->bJump == true)
			{//���n����

				//SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//�ō��x���W�ݒ�
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::LEFT:
		//�ʒu�E�ړ��ʏC��
		FixPos_LEFT(&pInfo->pos.x, pOthColli[0].MinPos.x, &pInfo->move.x, pOthColli[0].fWidth);
		break;

		//*********************************
		//�E�ɓ�������
		//*********************************
	case COLLI_ROT::RIGHT:
		//�ʒu�E�ړ��ʏC��
		FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].MaxPos.x, &pInfo->move.x, pOthColli[0].fWidth);
		break;
	}

	// �΂ˉ�
	switch (pOthColli[1].ColliRot)
	{
		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::UNDER:
		//�ʒu�E�ړ��ʏC��
		FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);

		//���̐��E�̃v���C���[�Ȃ�W�����v�\
		if (pInfo->side == WORLD_SIDE::BEHIND) {
			if (pInfo->bJump == true)
			{//���n����
			 //SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pOthColli[1].MinPos.y;//�ō��x���W�ݒ�
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::LEFT:
		//�ʒu�E�ړ��ʏC��
		FixPos_LEFT(&pInfo->pos.x, pOthColli[1].MinPos.x, &pInfo->move.x, pOthColli[1].fWidth);
		break;

		//*********************************
		//�E�ɓ�������
		//*********************************
	case COLLI_ROT::RIGHT:
		//�ʒu�E�ړ��ʏC��
		FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].MaxPos.x, &pInfo->move.x, pOthColli[1].fWidth);
		break;
	}
}

//----------------------------
//�g�Q�̓����蔻�菈��
//----------------------------
void CPlayer::CollisionSpike(Info *pInfo, Colli *pColli)
{
	//���S����
	Death(&pInfo->pos);
}

//----------------------------
// �ړ����̓����蔻�菈��
//----------------------------
void CPlayer::CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, Colli *pColli)
{
	switch (pColli->ColliRot)
	{
		//*********************************
		//��ɓ�������
		//*********************************
	case COLLI_ROT::OVER:
		//�ʒu�E�ړ��ʏC��
		FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

		//�\�̐��E�̃v���C���[�̏ꍇ
		if (pInfo->side == WORLD_SIDE::FACE)
		{
			pInfo->move = pMoveBlock->GetMove();
			if (pInfo->bJump == true)
			{//���n����
			 //SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pColli->MaxPos.y;//�ō��x���W�ݒ�
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::UNDER:
		//�ʒu�E�ړ��ʏC��
		FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

		//���̐��E�̃v���C���[�Ȃ�W�����v�\
		if (pInfo->side == WORLD_SIDE::BEHIND)
		{
			pInfo->pos = pMoveBlock->GetMove();
			if (pInfo->bJump == true)
			{//���n����
			 //SE�Đ�
				RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
			}
			pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
			pInfo->bJump = false;	//�W�����v�\
			pInfo->fMaxHeight = pColli->MinPos.y;//�ō��x���W�ݒ�
		}
		break;

		//*********************************
		//���ɓ�������
		//*********************************
	case COLLI_ROT::LEFT:
		//�ʒu�E�ړ��ʏC��
		FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
		break;

		//*********************************
		//�E�ɓ�������
		//*********************************
	case COLLI_ROT::RIGHT:
		//�ʒu�E�ړ��ʏC��
		FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
		break;

	case COLLI_ROT::UNKNOWN:
		//�ړ��� -> �v���C���[�ւ̓����蔻�菈�������s
		const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
		const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
		const float fWidth = pMoveBlock->GetWidth() * 0.5f;
		const float fHeight = pMoveBlock->GetHeight() * 0.5f;

		//�ړ�������̓����蔻��
		for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++)
		{
			//�v���C���[�̂ǂ̕����ɓ������Ă��邩
			pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOLd, pColli->fWidth, pColli->fHeight, (COLLI_VEC)nCntVec);

			//����ł�������Ȃ��Ȃ�A�X�L�b�v
			if (pColli->ColliRot == COLLI_ROT::NONE || pColli->ColliRot == COLLI_ROT::UNKNOWN) continue;

			//�������������i�㉺�E���E�j�𔽓]����
			{
				//��������������int�^�ɕϊ�
				const int nRot = (int)pColli->ColliRot;

				pColli->ColliRot = (COLLI_ROT)(nRot - 1 + 2 * (nRot % 2));
			}

			//������x�����蔻��
			CollisionMoveBlock(pInfo, pMoveBlock, pColli);
		}
		break;
	}
}

//----------------------------
// 覐΂̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionMeteor(Info *pInfo, Colli *pColli)
{
	//���S����
	Death(&pInfo->pos);
}

//----------------------------
// ���[�U�[�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, Colli *pColli, Colli *pOthColli)
{
	// �{��
	{
		//���������������Ƃɏ�����؂�ւ�
		switch (pColli->ColliRot)
		{
			//*********************************
			//��ɓ�������
			//*********************************
		case COLLI_ROT::OVER:
			//�ʒu�E�ړ��ʏC��
			FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

			//�\�̐��E�̃v���C���[�̏ꍇ
			if (pInfo->side == WORLD_SIDE::FACE)
			{
				pInfo->move = pRoadTripLaser->GetMove();
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pColli->MaxPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::UNDER:
			//�ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

			//�\�̐��E�̃v���C���[�̏ꍇ
			if (pInfo->side == WORLD_SIDE::FACE)
			{
				pInfo->move = pRoadTripLaser->GetMove();
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pColli->MinPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::LEFT:
			//�ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//�E�ɓ�������
			//*********************************
		case COLLI_ROT::RIGHT:
			//�ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//���܂���
			//*********************************
		case COLLI_ROT::UNKNOWN: 
			//���[�U�[ -> �v���C���[�ւ̓����蔻�菈�������s
			const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
			const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
			const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
			const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;

			//�ړ�������̓����蔻��
			for (int nCntVec = 0; nCntVec < (int)COLLI_VEC::MAX; nCntVec++)
			{
				//�v���C���[�̂ǂ̕����ɓ������Ă��邩
				pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOLd, pColli->fWidth, pColli->fHeight, (COLLI_VEC)nCntVec);

				//����ł�������Ȃ��Ȃ�A�X�L�b�v
				if (pColli->ColliRot == COLLI_ROT::NONE || pColli->ColliRot == COLLI_ROT::UNKNOWN) continue;

				//�������������i�㉺�E���E�j�𔽓]����
				{
					//��������������int�^�ɕϊ�
					const int nRot = (int)pColli->ColliRot;

					pColli->ColliRot = (COLLI_ROT)(nRot - 1 + 2 * (nRot % 2));
				}

				//������x�����蔻��
				CollisionLaser(pInfo, pRoadTripLaser, pColli, pOthColli);
			}
			break;
		}
	}

	// ���[�U�[
	{
		//���S����
		Death(&pInfo->pos);
	}
}

//----------------------------
// �k�C�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionDog(Info *pInfo, CExtenddog *pExtenddog, Colli *pColli, Colli *pOthColli)
{
	//���������������Ƃɏ�����؂�ւ�

	CExtenddog::STATE State = pExtenddog->GetState();

	// �n�E�X
	{
		switch (pColli->ColliRot)
		{
			//*********************************
			//��ɓ�������
			//*********************************
		case COLLI_ROT::OVER:
			//�ʒu�E�ړ��ʏC��
			FixPos_OVER(&pInfo->pos.y, pColli->MaxPos.y, &pInfo->move.y, pColli->fHeight);

			//�\�̐��E�̃v���C���[
			if (pInfo->side == WORLD_SIDE::FACE) {
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pColli->MinPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::UNDER:
			//�ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pInfo->pos.y, pColli->MinPos.y, &pInfo->move.y, pColli->fHeight);

			//���̐��E�̃v���C���[�Ȃ�W�����v�\
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pColli->MinPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::LEFT:
			//�ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pInfo->pos.x, pColli->MinPos.x, &pInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			//�E�ɓ�������
			//*********************************
		case COLLI_ROT::RIGHT:
			//�ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pInfo->pos.x, pColli->MaxPos.x, &pInfo->move.x, pColli->fHeight);
			break;

			//*********************************
			//���܂���
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}

	// ��
	{
		CExtenddog::STATE state = pExtenddog->GetState();
		switch (pOthColli[0].ColliRot)
		{
			//*********************************
			//��ɓ�������
			//*********************************
		case COLLI_ROT::OVER:
			//�ʒu�E�ړ��ʏC��
			FixPos_OVER(&pInfo->pos.y, pOthColli[0].MaxPos.y, &pInfo->move.y, pOthColli[0].fHeight);

			//�\�̐��E�̃v���C���[�̏ꍇ
			if (pInfo->side == WORLD_SIDE::FACE){
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::UNDER:
			//�ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pInfo->pos.y, pOthColli[0].MinPos.y, &pInfo->move.y, pOthColli[0].fHeight);

			//�\�̐��E�̃v���C���[�̏ꍇ
			if (pInfo->side == WORLD_SIDE::FACE){
				if (pInfo->bJump == true)
				{//���n����
				 //SE�Đ�
					RNLib::Sound().Play(m_landingSEIdx, CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);
				}
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[0].MaxPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::LEFT:
			//�ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pInfo->pos.x, pOthColli[0].MinPos.x, &pInfo->move.x, pOthColli[0].fWidth);
			break;

			//*********************************
			//�E�ɓ�������
			//*********************************
		case COLLI_ROT::RIGHT:

			if (state == CExtenddog::STATE::NONE) {
				//�ʒu�E�ړ��ʏC��
				FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].MaxPos.x, &pInfo->move.x, pOthColli[0].fWidth);
			}
			break;

			//*********************************
			//���܂���
			//*********************************
		case COLLI_ROT::UNKNOWN: break;

		}
	}

	// ��
	{
		switch (pOthColli[1].ColliRot)
		{
			//*********************************
			//��ɓ�������
			//*********************************
		case COLLI_ROT::OVER:
			//�ʒu�E�ړ��ʏC��
			FixPos_OVER(&pInfo->pos.y, pOthColli[1].MaxPos.y, &pInfo->move.y, pOthColli[1].fHeight);

			//�\�̐��E�̃v���C���[
			if (pInfo->side == WORLD_SIDE::FACE) {
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[1].MaxPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::UNDER:
			//�ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pInfo->pos.y, pOthColli[1].MinPos.y, &pInfo->move.y, pOthColli[1].fHeight);

			//���̐��E�̃v���C���[�Ȃ�W�����v�\
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				pInfo->bGround = true;
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[1].MinPos.y;//�ō��x���W�ݒ�
			}
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::LEFT:
			//�ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pInfo->pos.x, pOthColli[1].MinPos.x, &pInfo->move.x, pOthColli[1].fWidth);
			break;

			//*********************************
			//�E�ɓ�������
			//*********************************
		case COLLI_ROT::RIGHT:
			//�ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].MaxPos.x, &pInfo->move.x, pOthColli[1].fWidth);
			break;

			//*********************************
			//���܂���
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}

	// �K
	{
		switch (pOthColli[2].ColliRot)
		{
			//*********************************
			//��ɓ�������
			//*********************************
		case COLLI_ROT::OVER:
			//�ʒu�E�ړ��ʏC��
			FixPos_OVER(&pInfo->pos.y, pOthColli[2].MaxPos.y, &pInfo->move.y, pOthColli[2].fHeight);

			//�\�̐��E�̃v���C���[
			if (pInfo->side == WORLD_SIDE::FACE) {
				pInfo->bGround = true;	//�n�ʂɐڂ��Ă���
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[2].MaxPos.y;//�ō��x���W�ݒ�
			}

			if (State == CExtenddog::STATE::NONE)
				//SE�Đ�
				RNLib::Sound().Play(m_dogSEIdx[0], CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);

			pExtenddog->SetState(CExtenddog::STATE::UP_LAND);
			pInfo->bExtendDog = true;
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::UNDER:
			//�ʒu�E�ړ��ʏC��
			FixPos_UNDER(&pInfo->pos.y, pOthColli[2].MinPos.y, &pInfo->move.y, pOthColli[2].fHeight);

			//���̐��E�̃v���C���[�Ȃ�W�����v�\
			if (pInfo->side == WORLD_SIDE::BEHIND) {
				pInfo->bGround = true;
				pInfo->bJump = false;	//�W�����v�\
				pInfo->fMaxHeight = pOthColli[2].MinPos.y;//�ō��x���W�ݒ�
			}

			if (State == CExtenddog::STATE::NONE)
				//SE�Đ�
				RNLib::Sound().Play(m_dogSEIdx[0], CSound::CATEGORY::SE, false, CSound::SPACE::NONE, INITPOS3D, 0.0f);

			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
			pInfo->bExtendDog = true;
			break;

			//*********************************
			//���ɓ�������
			//*********************************
		case COLLI_ROT::LEFT:
			//�ʒu�E�ړ��ʏC��
			FixPos_LEFT(&pInfo->pos.x, pOthColli[2].MinPos.x, &pInfo->move.x, pOthColli[2].fWidth);
			break;

			//*********************************
			//�E�ɓ�������
			//*********************************
		case COLLI_ROT::RIGHT:
			//�ʒu�E�ړ��ʏC��
			FixPos_RIGHT(&pInfo->pos.x, pOthColli[2].MaxPos.x, &pInfo->move.x, pOthColli[2].fWidth);
			break;

			//*********************************
			//���܂���
			//*********************************
		case COLLI_ROT::UNKNOWN: Death(&pInfo->pos); break;
		}
	}
}

//----------------------------
// �S�[���Q�[�g�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
void CPlayer::CollisionGoalGate(Info *pInfo, Colli *pColli)
{
	if (!pInfo->bGoal)
	{
		pInfo->bGoal = true;

		int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
		int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
		}
	}
}

//----------------------------
//�p�[�c�̓����蔻�菈��
//----------------------------
void CPlayer::CollisionParts(Info *pInfo, CParts *pParts)
{
	if (!pParts->GetDisp()) return;

	//�擾�����̂ŕ`��OFF
	pParts->DispSwitch(false);

	//�擾������
	s_nNumGetParts++;

	//�擾���������S�̐��Ɠ����Ȃ�A���P�b�g��ԉ\
	if (s_nNumGetParts == CParts::GetNumAll()) s_bRideRocket = true;
}

//----------------------------
//���P�b�g�̓����蔻�菈��
//----------------------------
void CPlayer::CollisionRocket(Info *pInfo, CRocket *pRocket)
{
	int EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_Skull_000.png");
	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	if (!s_bRideRocket) return;

	//���P�b�g�ɓ���
	pInfo->bRide = true;
	pRocket->Ride();

	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	{
		Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	}
}

//========================
//�Ώە��̒��ɂ߂荞��ł��邩�ǂ�������
//------------------------
// �����P	pos			�F���݈ʒu
// �����Q	posOld		�F�O��ʒu
// �����R	fWidth		�F��
// �����S	fHeight		�F����
// �����T	TargetPos	�F�Ώۂ̌��݈ʒu
// �����U	TargetPosOld�F�Ώۂ̑O��ʒu�i�I�u�W�F�N�g��PosOld�ϐ��������Ȃ�A���݈ʒu��������OK
// �����V	TargetWidth	�F�Ώۂ̕�
// �����W	TargetHeight�F�Ώۂ̍���
// �����X	value		�F�x�N�g��
// �Ԃ�l	�Ώە��ɂ߂肱��ł��������Ԃ��iNONE�Ȃ瓖�����Ă��Ȃ�
//========================
CPlayer::COLLI_ROT CPlayer::IsBoxCollider(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight, D3DXVECTOR3 TargetPos, D3DXVECTOR3 TargetPosOld, float TargetWidth, float TargetHeight, COLLI_VEC value)
{
	//�����̌��݂̍ŏ��E�ő�ʒu
	const D3DXVECTOR2 MinPos = D3DXVECTOR2(pos.x - fWidth, pos.y - fHeight);
	const D3DXVECTOR2 MaxPos = D3DXVECTOR2(pos.x + fWidth, pos.y + fHeight);

	//�Ώۂ̌��݂̍ŏ��E�ő�ʒu
	const D3DXVECTOR2 TARGET_MinPos = D3DXVECTOR2(TargetPos.x - fWidth, TargetPos.y - fHeight);
	const D3DXVECTOR2 TARGET_MaxPos = D3DXVECTOR2(TargetPos.x + fWidth, TargetPos.y + fHeight);

	//�߂荞��ł��邩����
	if (MinPos.x < TARGET_MaxPos.x && TARGET_MinPos.x < MaxPos.x &&
		MinPos.y < TARGET_MaxPos.y && TARGET_MinPos.y < MaxPos.y)
	{
		//�����̉ߋ��̍ŏ��E�ő�ʒu
		const D3DXVECTOR2 OLD_MINPOS = D3DXVECTOR2(posOld.x - fWidth, posOld.y - fHeight);
		const D3DXVECTOR2 OLD_MAXPOS = D3DXVECTOR2(posOld.x + fWidth, posOld.y + fHeight);

		//�Ώۂ̑O��̍ŏ��E�ő�ʒu
		const D3DXVECTOR2 TARGET_MinPosOld = D3DXVECTOR2(TargetPosOld.x - fWidth, TargetPosOld.y - fHeight);
		const D3DXVECTOR2 TARGET_MaxPosOld = D3DXVECTOR2(TargetPosOld.x + fWidth, TargetPosOld.y + fHeight);

		//�Փ˃x�N�g���ŏ�������
		switch (value)
		{
		case COLLI_VEC::X:
			//�O��͍�����߂荞��ł��Ȃ��i���͂߂荞��
			if (OLD_MAXPOS.x <= TARGET_MinPosOld.x)			return COLLI_ROT::LEFT;

			//�O��͉E����߂荞��ł��Ȃ��i���͂߂荞��
			else if (OLD_MINPOS.x >= TARGET_MaxPosOld.x)	return COLLI_ROT::RIGHT;
			break;

		case COLLI_VEC::Y:
			//�O��͏ォ��߂荞��ł��Ȃ��i���͂߂荞��
			if (OLD_MINPOS.y >= TARGET_MaxPosOld.y)			return COLLI_ROT::OVER;

			//�O��͉�����߂荞��ł��Ȃ��i���͂߂荞��
			else if (OLD_MAXPOS.y <= TARGET_MinPosOld.y)	return COLLI_ROT::UNDER;
			break;
		}

		//��������������������Ȃ�
		return COLLI_ROT::UNKNOWN;
	}

	//������Ȃ�����
	return COLLI_ROT::NONE;
}

//----------------------------
//�v���C���[���ݒ�
//�w�肳�ꂽ�ԍ��̃v���C���[����ݒ肵�܂��B
//----------------------------
void CPlayer::SetInfo(const Info info, const int nNum)
{
	if (0 <= nNum && nNum < NUM_PLAYER)
	{
		//�e�v���C���[���ݒ�
		m_aInfo[nNum] = info;
		m_aInfo[nNum].StartPos = info.pos;
	}
}

//----------------------------
//�v���C���[���ݒ�
//----------------------------
void CPlayer::SetInfo(Info p1, Info p2)
{
	//�e�v���C���[���ݒ�
	m_aInfo[0] = p1;	m_aInfo[0].StartPos = p1.pos;
	m_aInfo[1] = p2;	m_aInfo[1].StartPos = p2.pos;
}

//----------------------------
//�v���C���[�Ƀg�����|�����p�̃W�����v��ݒ�
//----------------------------
void CPlayer::SetTrampolineJump(Info*& pInfo, float fMaxHeight)
{
	SetSwapInterval();

	fMaxHeight += 20.0f;

	//�W�����v�ʂ��p��
	float diff = -fMaxHeight - pInfo->pos.y;
	pInfo->move.y = pInfo->move.y = diff / TRAMPOLINE_JUMP_COUNTER;

	pInfo->fTramTargetPosY = -fMaxHeight;
	pInfo->nTramJumpCounter = TRAMPOLINE_JUMP_COUNTER;
	pInfo->bTramJump = true;
	pInfo->bGround = false;
}

//----------------------------
//�v���C���[���擾
//�w�肳�ꂽ���E�ɂ���v���C���[�̏���Ԃ��܂�
//----------------------------
CPlayer::Info *CPlayer::GetInfo(WORLD_SIDE side)
{
	//�P�o�̂��鐢�E�ƍ��v������P�o����Ԃ�
	if (m_aInfo[0].side == side) return &m_aInfo[0];

	//�Ⴄ�Ȃ�Q�o����Ԃ�
	else return &m_aInfo[1];
}

//----------------------------
//�v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//�v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//���̓����蔻����̃��������
//----------------------------
void CPlayer::OthColliDelete(void)
{
	if (pOthColli != NULL)
	{
		if (!bPluralColli)
		{
			delete pOthColli;
		}
		else if (bPluralColli)
		{
			delete[] pOthColli;
		}
		pOthColli = NULL;
	}
}
