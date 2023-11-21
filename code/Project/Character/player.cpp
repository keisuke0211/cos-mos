//================================================================================================
// 
// �v���C���[����[player.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../main.h"
#include "player.h"
#include "../collision.h"
#include "../../_RNLib/Basis/Calculation/number.h"
#include "../Object/Gimmick/pile.h"

// �X���b�v�C���^�[�o��
const int	CPlayer::SWAP_INTERVAL = 90;	// �X���b�v�C���^�[�o��
int			CPlayer::s_nSwapInterval = 0;	// �c��X���b�v�C���^�[�o��
bool		CPlayer::s_bSwapAnim = false;	//�X���b�v�A�j���[�V���������ǂ���
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//�A�j���[�V�����\��

const float CPlayer::SIZE_WIDTH = 7.0f;	// ����
const float CPlayer::SIZE_HEIGHT = 8.0f;// ����

const float CPlayer::MOVE_SPEED = 0.3f;		// �ړ���
const float CPlayer::MAX_MOVE_SPEED = 2.3f;	// �ő�ړ���

const float CPlayer::JUMP_POWER = 5.0f;		// ��{�W�����v��
const float CPlayer::GRAVITY_POWER = -0.3f;	// ��{�d�͉����x

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// �X���b�v�}�[�N
	"data\\TEXTURE\\Effect\\eff_Star_000.png",	// �X���b�v�p�[�e�B�N��
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// ���S�}�[�N
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// ���S�p�[�e�B�N��
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// �S�[���E���P�b�g��Ԏ��̃G�t�F�N�g
};
int			CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE CPlayer::s_SE = {};	//�T�E���h�p�\����
CCollision *CPlayer::s_pColli = NULL;

//=======================================
// �R���X�g���N�^
//=======================================
CPlayer::CPlayer()
{
	// �c��X���b�v�C���^�[�o��
	s_nSwapInterval = 0;
	s_bSwapAnim = false;
	s_AnimState = SWAP_ANIM::PROLOGUE;

	s_SE.pSound = NULL;
	s_SE.jump = 0;
	s_SE.landing = 0;
	s_SE.Swap = 0;
	for each(short &dog in s_SE.dog)
	{
		dog = 0;
	}

	for each(Info &Player in m_aInfo)
	{
		Player.StartPos = INITD3DXVECTOR3;		// �J�n�ʒu
		Player.pos = INITD3DXVECTOR3;			// �ʒu
		Player.posOld = INITD3DXVECTOR3;		// �O��ʒu
		Player.rot = INITD3DXVECTOR3;			// ����
		Player.move = INITD3DXVECTOR3;			// �ړ���
		Player.color = INITCOLOR;				// �F
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA;	// �X���b�v�}�[�N�̃��l
		Player.fSwapPosY = 0.0f;				// �X���b�v��̂x���W
		Player.fSwapMoveY = 0.0f;				// �X���b�v�ړ����̑��x
		Player.bGround = false;					// �n�ʂɐڂ��Ă��邩
		Player.bGroundOld = false;				// �n�ʂɐڂ��Ă��邩(�ߋ�)
		Player.bJump = false;					// �W�����v
		Player.bRide = false;					// ���P�b�g�ɏ���Ă��邩�ǂ���
		Player.bGoal = false;					// �S�[���������ǂ���
		Player.fJumpPower = 0.0f;				// �W�����v��
		Player.fGravity = 0.0f;					// �d��
		Player.fMaxHeight = 0.0f;				// �ō��x���W
		Player.nTramJumpCounter = 0;			// �g�����|�����ɂ���Ē��ˏオ��ō����B�n�_
		Player.fTramTargetPosY = 0.0f;			// �g�����|�����p�̖ڕW�ʒu
		Player.bTramJump = false;				// �g�����|�����p�̓���W�����v
		//Player.TramColliRot = CCollision::ROT::NONE;	// �g�����|�����p�̔���
		Player.bExtendDog = false;				// �k�C�p�̐ڐG�t���O
		Player.bLandPile = false;				// �Y�ɏ���Ă��邩�ǂ���
		Player.nModelIdx = NONEDATA;			// ���f���ԍ�
		Player.side = WORLD_SIDE::FACE;			// �ǂ���̐��E�ɑ��݂��邩
	}

	s_pColli = NULL;	// �����蔻��N���X
}

//=======================================
// �f�X�g���N�^
//=======================================
CPlayer::~CPlayer()
{
	if (s_pColli != NULL)
	{
		s_pColli->OthColliDelete();
		delete s_pColli;
	}
	s_pColli = NULL;
}

//=======================================
// ��������
//=======================================
CPlayer *CPlayer::Create(void)
{
	// �v���C���[�C���X�^���X�̐���
	CPlayer *pPlayer = new CPlayer;

	// ����������
	pPlayer->Init();

	// �v���C���[��Ԃ�
	return pPlayer;
}

//=====================================================================================================================
// ����������
//=====================================================================================================================
HRESULT CPlayer::Init(void)
{
	// �P�o�������
	m_aInfo[0].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Human.x");

	// �Q�o�������
	m_aInfo[1].nModelIdx = RNLib::Model().Load("data\\MODEL\\Player_Octopus.x");
	m_aInfo[1].rot = CStageObject::INVERSEVECTOR3;

	// �L�[�R���t�B�O������
	InitKeyConfig();

	//�p�[�e�B�N���e�N�X�`���Ǎ�
	CTexture *pTexture = &RNLib::Texture();
	for (int nCntTex = 0; nCntTex < (int)PARTI_TEX::MAX; nCntTex++)
	{
		s_ParticleTex[nCntTex] = pTexture->Load(PARTICLE_TEX_PATH[nCntTex]);
	}

	s_SE.pSound	= &RNLib::Sound();
	s_SE.jump	= s_SE.pSound->Load("data\\SOUND\\SE\\jamp_01.wav");
	s_SE.landing = s_SE.pSound->Load("data\\SOUND\\SE\\jamp.wav");
	s_SE.dog[0]	= s_SE.pSound->Load("data\\SOUND\\SE\\heron2.wav");		// ����
	s_SE.dog[1]	= s_SE.pSound->Load("data\\SOUND\\SE\\shrink.wav");		// �k��
	s_SE.dog[2]	= s_SE.pSound->Load("data\\SOUND\\SE\\extend.wav");		// �L�т�
	s_SE.dog[3]	= s_SE.pSound->Load("data\\SOUND\\SE\\vibration.wav");	// �k����
	s_SE.Swap	= s_SE.pSound->Load("data\\SOUND\\SE\\swap.wav");

	// �������ݒ�
	Death(NULL);

	// �����l�ݒ�
	// �� ����Ȃ��ƃX�e�[�W����O�Ɉ�u���nSE���Ȃ�
	m_aInfo[0].bJump = m_aInfo[1].bJump = false;

	if(s_pColli == NULL)
	s_pColli = new CCollision;

	// ����������
	return S_OK;
}

//=======================================
// �e�v���C���[�̃L�[�{�[�h�E�W���C�p�b�h�̃L�[�R���t�B�O�������ݒ�
//=======================================
void CPlayer::InitKeyConfig(void)
{
	// �ǂ���̐��E�ł����ʂ̃L�[�z�u�ݒ�
	for (int nCntSide = 0; nCntSide < (int)WORLD_SIDE::MAX; nCntSide++)
	{
		// �P�o�̃L�[�{�[�h�z�u
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_A;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_D;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[0].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		// �Q�o�̃L�[�{�[�h�z�u
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_LEFT] = DIK_LEFTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::MOVE_RIGHT] = DIK_RIGHTARROW;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::DECIDE] = DIK_RETURN;
		m_aInfo[1].Keyborad[nCntSide][(int)KEY_CONFIG::PAUSE] = DIK_P;

		// ���E�ŕς��L�[�z�u�ݒ�
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

	// �W���C�p�b�h�̐ݒ�͗��ҋ���
	for each(Info &Player in m_aInfo)
	{
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = CInput::BUTTON::LEFT;  // ���ړ�
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = CInput::BUTTON::RIGHT; // �E�ړ�
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = CInput::BUTTON::A;     // �W�����v
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = CInput::BUTTON::X;     // �X���b�v
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = CInput::BUTTON::A;     // ����
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = CInput::BUTTON::START; // �|�[�Y
	}
}

//=====================================================================================================================
// �I������
//=====================================================================================================================
void CPlayer::Uninit(void)
{

}

//=======================================
// �O��ʒu�X�V����
//=======================================
void CPlayer::SetPosOld(void)
{
	// �v���C���[�̑O��ʒu�X�V
	for each (Info &Player in m_aInfo)
	{
		// ���P�b�g�ɏ���Ă��Ȃ��Ȃ�@or �S�[�����Ă��Ȃ��Ȃ�
		if (!Player.bRide || Player.bGoal)
		Player.posOld = Player.pos;
		Player.bGroundOld = Player.bGround;
	}
}

//=====================================================================================================================
// �X�V����
//=====================================================================================================================
void CPlayer::Update(void)
{
	//�X���b�v�A�j���[�V������
	if (s_bSwapAnim)
	{
		SwapAnimation();
		return;
	}

	// �O��ʒu�X�V
	SetPosOld();

	// ���쏈��
	ActionControl();

	// ���҂Ƃ��ɃS�[�����ĂȂ�������
	if (!m_aInfo[0].bGoal && !m_aInfo[1].bGoal)
	{
		// �X���b�v
		Swap();
	}
	else if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
	{
		const int planet = Manager::StgEd()->GetPlanetIdx();
		const int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
		Manager::StgEd()->SwapStage(stage + 1);
	}

	// �����蔻��܂Ƃ�
	CollisionToStageObject();

	// ���X�V
	UpdateInfo();

	RNLib::Text2D().PutDebugLog(CreateText("FPS:%d", RNSystem::GetFPS()));
}

//----------------------------
// ���X�V����
//----------------------------
void CPlayer::UpdateInfo(void)
{
	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;
		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		// �ʒu�ݒ�
		RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
			->SetOutLine(true)
			->SetCol(Player.color);

		// �X���b�v��̃}�[�N��`�悷��ʒu
		D3DXVECTOR3 MarkPos = Player.pos;
		MarkPos.z = -10.0f;
		MarkPos.y *= -1.0f;

		RNLib::Polygon3D().Put(PRIORITY_EFFECT, MarkPos, INITD3DXVECTOR3)
			->SetSize(20.0f, 20.0f)
			->SetBillboard(true)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_MARK))
			->SetCol(Color{ Player.color.r,Player.color.g,Player.color.b, (UShort)Player.nSwapAlpha });

		// �ō��x���W�X�V
		switch (Player.side)
		{
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
		RNLib::Text2D().PutDebugLog(CreateText("%dP�ō�Y���W�F%f    ����Y���W�F%f", nCntPlayer + 1, Player.fMaxHeight, Player.pos.y));
	}
}

//----------------------------
// ���쏈��
//----------------------------
void CPlayer::ActionControl(void)
{
	// �v���C���[�ԍ�
	int nIdxPlayer = -1;

	for each (Info &Player in m_aInfo)
	{
		// ���̃v���C���[�ԍ���
		nIdxPlayer++;

		// �������S�[�����Ă��Ȃ���Ώo��
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer +1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bRide = false;
			Player.bGoal = false;
			Player.move.x *= -2.0f;
		}

		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		// �W�����v���́i�󒆂���Ȃ��j
		if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			Player.bGround = false;				// �n�ʂ��痣�ꂽ
			Player.move.y = Player.fJumpPower;	// �W�����v�ʑ��
			Player.bJump = true;				// �W�����v����

												// SE�Đ�
			s_SE.pSound->Play(s_SE.jump, CSound::CATEGORY::SE, false);
		}

		// �E�Ɉړ�
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer))
			Player.move.x += MOVE_SPEED;

		// ���Ɉړ�
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer))
			Player.move.x -= MOVE_SPEED;

		// �X���b�v����
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
		{
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI), Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
			Player.nSwapAlpha = 255;
		}
		//�X���b�v��̃}�[�N�J���[��ύX
		else Player.nSwapAlpha = NORMAL_SWAP_ALPHA;
	}
}

//#################################################
// �X���b�v����
//#################################################
void CPlayer::Swap(void)
{
	// ���҂Ƃ��ɃX���b�v�{�^���������Ă���܂��͂ǂ��炩�����P�b�g�ɏ���Ă���
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) || m_aInfo[0].bRide) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) || m_aInfo[1].bRide))
	{
		// �X���b�v�A�j���[�V�����ݒ�
		s_AnimState = SWAP_ANIM::PROLOGUE;
		s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
		s_bSwapAnim = true;

		//���P�b�g�ɏ���Ă��Ȃ��Ƃ��ɃT�E���h�Đ�
		if(CRocket::GetCounter() != NUM_PLAYER)	s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);

		for each (Info &Player in m_aInfo)
		{
			// ���P�b�g�ɏ���Ă���X�L�b�v
			if (Player.bRide) continue;

			for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI), Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}

			// �ʒu�E�d�͉����x�E�W�����v�ʁE���݂��鐢�E�𔽓]
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
			//Player.TramColliRot = CCollision::ROT::NONE;
			Player.bTramJump = false;
		}

		// �O��ʒu�X�V
		SetPosOld();
	}
}

//*************************************************
// �X���b�v�A�j���[�V����
//*************************************************
void CPlayer::SwapAnimation(void)
{
	RNLib::Text2D().PutDebugLog(CreateText("�C���^�[�o���F%d", s_nSwapInterval));
	switch (s_AnimState)
	{
		case CPlayer::SWAP_ANIM::PROLOGUE:RNLib::Text2D().PutDebugLog("�v�����[�O"); break;
		case CPlayer::SWAP_ANIM::MIDDLE:  RNLib::Text2D().PutDebugLog("����"); break;
		case CPlayer::SWAP_ANIM::EPILOGUE:RNLib::Text2D().PutDebugLog("�G�s���[�O"); break;
	}

	//if (!RNLib::Input().GetKeyTrigger(DIK_RETURN) &&
	//	!RNLib::Input().GetKeyPress(DIK_RSHIFT)) return;

	//�C���^�[�o������
	s_nSwapInterval--;

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//�v���C���[���Q�Ɓi�ړ��������p�[�e�B�N���̈ʒu����O�ɏo��
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer);break;//�v�����[�O
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;//����
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer);break;//�G�s���[�O
		}

		for (int nCntPar = 0; nCntPar < 1; nCntPar++)
		{
			Color setCol;
			if (nCntPlayer == 0){
				setCol = Color{ (UShort)(215 + rand() % 40),(UShort)(135 + rand() % 40),(UShort)(39 + rand() % 40),255 };
			}
			else{
				setCol = Color{ (UShort)(45 + rand() % 40),(UShort)(203 + rand() % 40),(UShort)(190 + rand() % 40),255 };
			}
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_MARK), Player.pos, Vector3D(16.0f, 16.0f, 0.0f), setCol);
		}
	}
}

//*************************************************
//�v�����[�O����
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, const int nIdxPlayer)
{
	// �ʒu�ݒ�
	RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
		->SetOutLine(true)
		->SetCol(Player.color);

	//���̃C���^�[�o����
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_MIDDLE_INTERVAL;
	s_AnimState = SWAP_ANIM::MIDDLE;
}

//*************************************************
//���ԏ���
//*************************************************
void CPlayer::SwapAnim_Middle(Info& Player, const int nIdxPlayer)
{
	//�X���b�v��ֈړ�
	Player.pos.y += Player.fSwapMoveY;

	//���̃C���^�[�o����
	if (s_nSwapInterval > 0) return;
	Player.pos.y = Player.fSwapPosY;
	Player.move.y *= -1.0f;
	Player.fGravity *= -1.0f;
	Player.fJumpPower *= -1.0f;
	Player.side = (WORLD_SIDE)(((int)Player.side + 1) % (int)WORLD_SIDE::MAX);
	if (Player.side == WORLD_SIDE::FACE)
		Player.rot.z = 0.0f;
	else Player.rot.z = D3DX_PI;

	//�Ō�̃v���C���[�̂Ƃ��ɃA�j���[�V������Ԃ�J��
	if (nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_EPILOGUE_INTERVAL;
	s_AnimState = SWAP_ANIM::EPILOGUE;
}

//*************************************************
//�G�s���[�O����
//*************************************************
void CPlayer::SwapAnim_Epilogue(Info& Player, const int nIdxPlayer)
{
	// �ʒu�ݒ�
	RNLib::Model().Put(PRIORITY_OBJECT, Player.nModelIdx, Player.pos, Player.rot, false)
		->SetOutLine(true)
		->SetCol(Player.color);

	//�Ō�̃v���C���[�̂Ƃ��ɃX���b�v�A�j���[�V�����I��
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_bSwapAnim = false;
}

//----------------------------
// ���S����
//----------------------------
void CPlayer::Death(D3DXVECTOR3 *pDeathPos)
{
	if (pDeathPos != NULL)
	{
		Manager::EffectMgr()->EffectCreate(GetParticleIdx(PARTI_TEX::DEATH_MARK), *pDeathPos, INIT_EFFECT_SCALE, Color{ 255,0,255,255 });

		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::DEATH_PARTI), *pDeathPos, INIT_EFFECT_SCALE * 0.5f, Color{ 255,0,0,255 });
		}
	}

	// �P�o�p�������
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;

	// �Q�o�p�������
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;

	// ���ҋ��ʏ������
	for each (Info &Player in m_aInfo)
	{
		Player.posOld = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
	}

	//���悵�Ă���v���C���[�̐����Z�b�g
	CRocket::ResetCounter();
}

//----------------------------
// �ړ�����
//----------------------------
void CPlayer::Move(VECTOL vec)
{
	// �v���C���[�̈ʒu�X�V
	for each (Info &Player in m_aInfo)
	{
		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

		// �ߋ��̈ʒu��ݒ�
		Player.posOld = Player.pos;

		// �ړ��ʔ��f
		switch (vec)
		{
		case VECTOL::X:
			// ��������
			Player.move.x += (0.0f - Player.move.x) * 0.12f;

			// �]�̈ړ��ʂ��C��
			FloatControl(&Player.move.x, MAX_MOVE_SPEED, -MAX_MOVE_SPEED);

			// �ʒu�X�V
			Player.pos.x += Player.move.x;
			break;

			// �d�͏���
		case VECTOL::Y:

			// �g�����|�����ɂ�����W�����v��
			if (Player.bTramJump)
			{// �J�E���^�[�����炵�āA�O�ɂȂ��������W�����v�I��
				
				float diff = Player.fTramTargetPosY - Player.pos.y;

				if (--Player.nTramJumpCounter <= 0)
				{
					Player.bTramJump = false;
					Player.move.y = diff;
				}
				else
				{
					Player.move.y = diff / Player.nTramJumpCounter;
				}
			}
			// �ʏ펞�Ȃ�A�d�͏����łx�̈ړ��ʂ��v�Z
			else Player.move.y += Player.fGravity;

			// �ʒu�X�V
			Player.pos.y += Player.move.y;
			break;
		}

		//�ʒu����
		CtrlPos(&Player, vec);
	}
}

//----------------------------
// ���䏈��
//----------------------------
void CPlayer::CtrlPos(Info *pInfo, VECTOL vec)
{
	int RowMax = Manager::StgEd()->GetRowMax() - 1;
	int LineMax = Manager::StgEd()->GetLineMax() - 1;

	D3DXVECTOR3 MinPos = Manager::StgEd()->GetPos(0, 0);
	D3DXVECTOR3 MaxPos = Manager::StgEd()->GetPos(RowMax, LineMax);

	// 1�}�X���炷
	MinPos.y = -MinPos.y - CStageObject::SIZE_OF_1_SQUARE * 0.5f;
	MaxPos.y = -MaxPos.y + CStageObject::SIZE_OF_1_SQUARE * 0.5f;

	// �v���C���[�̈ʒu�X�V
	switch (vec)
	{
			//�w���W
		case CPlayer::VECTOL::X:
			if		(pInfo->pos.x <= MinPos.x)	pInfo->pos.x = MinPos.x;
			else if (pInfo->pos.x > MaxPos.x)	pInfo->pos.x = MaxPos.x;
			break;

			//�x���W
		case CPlayer::VECTOL::Y:
			if		(pInfo->pos.y <= MinPos.y)	pInfo->pos.y = MinPos.y;
			else if (pInfo->pos.y > MaxPos.y)	pInfo->pos.y = MaxPos.y;
			break;
	}
}

//========================================
// �����蔻�菈��
// Author:HIRASAWA SHION
// Arrange:RIKU NISHIMURA  OTONO KEISUKE
//========================================
void CPlayer::CollisionToStageObject(void)
{
	// ��U���v���C���[�Ƃ��ɃW�����v�s��
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++) {

		// �Փ˃x�N�g���ɕϊ�
		const VECTOL vec = (VECTOL)nCntVec;

		// �ړ��E�ʒu���䏈��
		Move(vec);

		// �I�u�W�F�N�g1��1�����Ă���
		CObject* obj = NULL;
		while (Manager::StageObjectMgr()->ListLoop(&obj)) {

			// �擾�����I�u�W�F�N�g���L���X�g
			CStageObject* stageObj = (CStageObject*)obj;

			// ��ގ擾
			const CStageObject::TYPE type = stageObj->GetType();

			// �I�u�W�F�N�g�̓����蔻�����ݒ�
			CCollision::ColliInfo m_colliInfo;
			m_colliInfo.pos     = stageObj->GetPos();
			m_colliInfo.posOld  = m_colliInfo.pos;
			m_colliInfo.fWidth  = stageObj->GetWidth() * 0.5f;
			m_colliInfo.fHeight = stageObj->GetHeight() * 0.5f;
			m_colliInfo.minPos  = D3DXVECTOR3(m_colliInfo.pos.x - m_colliInfo.fWidth, m_colliInfo.pos.y - m_colliInfo.fHeight, 0.0f);
			m_colliInfo.maxPos  = D3DXVECTOR3(m_colliInfo.pos.x + m_colliInfo.fWidth, m_colliInfo.pos.y + m_colliInfo.fHeight, 0.0f);

			// �v���C���[���ɏ������Ă���
			for each(Info& Player in m_aInfo) {
				// ���P�b�g�ɏ���Ă邩�S�[�����Ă�����܂�Ԃ�
				if (Player.bRide || Player.bGoal)
					continue;

				// �v���C���[�̋߂��ɃI�u�W�F�N�g�����邩����
				// ������I�u�W�F�N�g������
				if (type != CStageObject::TYPE::TRAMPOLINE && 
					type != CStageObject::TYPE::LASER && 
					type != CStageObject::TYPE::EXTEND_DOG) {

					if (D3DXVec3Length(&(m_colliInfo.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(m_colliInfo.fWidth + SIZE_WIDTH, m_colliInfo.fHeight + SIZE_HEIGHT)))
						continue;
				}

				//�����̏��
				CCollision::SelfInfo Self;
				Self.pos = Player.pos;
				Self.posOld = Player.posOld;
				Self.move = Player.move;
				Self.fWidth = SIZE_WIDTH;
				Self.fHeight = SIZE_HEIGHT;

				// ���p�[�c�̓����蔻��t���O
				//bool bOtherColl = false;

				// �ړ�����I�u�W�F�N�g�́A
				// �����蔻��ʒu�ɑO��ʒu��ݒ肷��
				switch (type) {
					// �ړ���
				case CStageObject::TYPE::BLOCK: {
					CBlock* pBlock = (CBlock*)stageObj;
					if (!pBlock->GetCollision())
						continue;
				}break;

					// �ړ���
				case CStageObject::TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock*)stageObj;
					m_colliInfo.posOld = pBlock->GetPosOld();
				}
				break;

					// 覐�
				case CStageObject::TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					m_colliInfo.posOld = pMeteor->GetPosOld();
				}
				break;

					// ���[�U�[
				case CStageObject::TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					//m_pOthColli = new CollInfo;
					//
					//m_pOthColli->pos = pLaser->GetLaserPos();
					//m_pOthColli->posOld = m_pOthColli->pos;
					//m_pOthColli->fWidth = pLaser->GetLaserSize().x * 0.5f;
					//m_pOthColli->fHeight = pLaser->GetLaserSize().y * 0.5f;
					//
					//// �I�u�W�F�N�g�̍ŏ��E�ő�ʒu
					//m_pOthColli->minPos = D3DXVECTOR3(m_pOthColli->pos.x - m_pOthColli->fWidth, m_pOthColli->pos.y - m_pOthColli->fHeight, 0.0f);
					//m_pOthColli->maxPos = D3DXVECTOR3(m_pOthColli->pos.x + m_pOthColli->fWidth, m_pOthColli->pos.y + m_pOthColli->fHeight, 0.0f);
					//
					//// ���������������i�[
					//m_pOthColli->ColliRot = IsBoxCollider(Player.pos, Player.posOld, SIZE_WIDTH, SIZE_HEIGHT, m_pOthColli->pos, m_pOthColli->posOld, m_pOthColli->fWidth, m_pOthColli->fHeight, vec);

					//if (m_pOthColli->ColliRot != CCollision::ROT::NONE) {
					//	bOtherColl = true;
					//}
				}
				break;

					// �k�C
				case CStageObject::TYPE::EXTEND_DOG:{
					//CExtenddog *pDog = (CExtenddog *)stageObj;

					//m_pOthColli = new CollInfo[OBJ_EXTENDDOG];

					//for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
					//	switch (nCnt){
					//	case 0:
					//		m_pOthColli[nCnt].pos = pDog->GetHeadPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetHeadPosOid();
					//		break;
					//	case 1:
					//		m_pOthColli[nCnt].pos = pDog->GetBodyPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetBodyPos();
					//		break;
					//	case 2:
					//		m_pOthColli[nCnt].pos = pDog->GetHipPos();
					//		m_pOthColli[nCnt].posOld = pDog->GetHipPos();
					//		break;
					//	}

					//	m_pOthColli[nCnt].fWidth = pDog->GetWidth() * 0.5f;
					//	m_pOthColli[nCnt].fHeight = pDog->GetHeight() * 0.5f;

					//	if (D3DXVec3Length(&(m_pOthColli[nCnt].pos - Player.pos)) >
					//		D3DXVec2Length(&D3DXVECTOR2(m_pOthColli[nCnt].fWidth + SIZE_WIDTH, m_pOthColli[nCnt].fHeight + SIZE_HEIGHT))) continue;

					//	// �I�u�W�F�N�g�̍ŏ��E�ő�ʒu
					//	m_pOthColli[nCnt].minPos = D3DXVECTOR3(m_pOthColli[nCnt].pos.x - m_pOthColli[nCnt].fWidth, m_pOthColli[nCnt].pos.y - m_pOthColli[nCnt].fHeight, 0.0f);
					//	m_pOthColli[nCnt].maxPos = D3DXVECTOR3(m_pOthColli[nCnt].pos.x + m_pOthColli[nCnt].fWidth, m_pOthColli[nCnt].pos.y + m_pOthColli[nCnt].fHeight, 0.0f);

					//	// ���������������i�[
					//	m_pOthColli[nCnt].ColliRot = s_pColli->IsBoxCollider(*pSelf, m_colliInfo, vec);

					//	//if (m_pOthColli[nCnt].ColliRot != CCollision::ROT::NONE){
					//	//	bOtherColl = true;
					//	//}
					//}

					//CExtenddog::STATE state = pDog->GetState();
					//if ((m_pOthColli[2].ColliRot != CCollision::ROT::UNDER) ||
					//	(m_pOthColli[2].ColliRot != CCollision::ROT::OVER && state == CExtenddog::STATE::DOWN_LAND)) {
					//	Player.bExtendDog = false;
					//}
				}
				break;

					//�Y
				case CStageObject::TYPE::PILE:
				{
					CPile *pPile = (CPile *)stageObj;

					m_colliInfo.pos = pPile->GetPos();
					m_colliInfo.posOld = pPile->GetPosOld();
				}
				break;
				}

				// ���������������i�[
				m_colliInfo.Rot = s_pColli->IsBoxCollider(Self, m_colliInfo, vec);

				// �������Ă��Ȃ�
				if (m_colliInfo.Rot == CCollision::ROT::NONE)
				{
					//�Y�ɓ������Ă��Ȃ��Ȃ����Ă��Ȃ�
					if (type == CStageObject::TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				// ��ނ��ƂɊ֐�����
				switch (type)
				{
				case CStageObject::TYPE::BLOCK:		s_pColli->Block(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::FILLBLOCK:	s_pColli->FillBlock(&Self, m_colliInfo.Rot, &Player.side); break;
				case CStageObject::TYPE::TRAMPOLINE:s_pColli->Trampoline(&Self, &m_colliInfo, (CTrampoline *)stageObj, &Player.side);	break;
				case CStageObject::TYPE::SPIKE:		s_pColli->Spike(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::MOVE_BLOCK:s_pColli->MoveBlock(&Self, (CMoveBlock *)stageObj, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::METEOR:	s_pColli->Meteor(&Self, &m_colliInfo, &Player.side); break;
				case CStageObject::TYPE::LASER:		s_pColli->Laser(&Self, (CRoadTripLaser *)stageObj,&m_colliInfo, NULL, &Player.side);	break;
				case CStageObject::TYPE::EXTEND_DOG:s_pColli->Dog(&Self, (CExtenddog *)stageObj, &m_colliInfo, NULL, &Player.side); break;
				case CStageObject::TYPE::GOALGATE:	s_pColli->GoalGate(&Self, &m_colliInfo, &Player.side);	break;
				case CStageObject::TYPE::PARTS:		s_pColli->Parts(&Self, (CParts *)stageObj, &Player.side); break;
				case CStageObject::TYPE::ROCKET:	s_pColli->Rocket(&Self, (CRocket *)stageObj, &Player.side); break;
				case CStageObject::TYPE::PILE:		s_pColli->Pile(&Self, &m_colliInfo, (CPile *)stageObj, &Player.side); break;
				}

				//�����
				Player.pos = Self.pos;
				Player.move = Self.move;

				// ������Α����̃I�u�W�F�N�g�ɓ������Ă���
				if (type == CStageObject::TYPE::SPIKE || type == CStageObject::TYPE::METEOR || type == CStageObject::TYPE::LASER)
					break;
			}

			//�����蔻��̎��㏈��
			CollisionAfter(stageObj, type);
		}
	}
}

//----------------------------
// �e�v���C���[�̓����蔻�肪�I�������̏���
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type)
{
	// ��ނ��ƂɊ֐�����
	switch (type)
	{
		// �k�C�̏�Ԑݒ�
		case CStageObject::TYPE::EXTEND_DOG:
		{
			if (!m_aInfo[0].bExtendDog && !m_aInfo[1].bExtendDog)
			{
				CExtenddog *pDog = (CExtenddog *)pStageObj;
				pDog->SetState(CExtenddog::STATE::RETURN);
			}

			break;
		}

		//�Y�ɏ���Ă���v���C���[
		case CStageObject::TYPE::PILE:
		{
			CPile *pPile = (CPile *)pStageObj;
			const float CaveInPos = pPile->GetPosCaveIn().y;
			const float Height = pPile->GetHeight() * 0.5f;

			for each (Info &Player in m_aInfo)
			{
				if(!Player.bLandPile) continue;

				switch (Player.side)
				{
					case WORLD_SIDE::FACE:	Player.pos.y = CaveInPos + Height + SIZE_HEIGHT;	break;
					case WORLD_SIDE::BEHIND:Player.pos.y = CaveInPos - Height - SIZE_HEIGHT;	break;
				}
				//���̍Y�Ŕ��肵�Ȃ��悤������
				Player.bLandPile = false;
			}
			break;
		}
	}
}

//----------------------------
// �ォ��̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	// �ʒu�E�ړ��ʏC��
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// ������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	// �ʒu�E�ړ��ʏC��
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// ������̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	// �ʒu�E�ړ��ʏC��
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// �E����̓����蔻��ɂ��ʒu�C��
//----------------------------
void CPlayer::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	// �ʒu�E�ړ��ʏC��
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// �u���b�N�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionBlock(Info *pInfo, CollInfo *pColli)
//{
//	// ���������������Ƃɏ�����؂�ւ�
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// ��ɓ�������
//		//*********************************
//	case CCollision::ROT::OVER:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// �\�̐��E�̃v���C���[
//		if (pInfo->side == WORLD_SIDE::FACE) {
//			if (pInfo->bJump == true)
//			{// ���n����
//				// SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//		}
//		break;
//
//		//*********************************
//		// ���ɓ�������
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//		if (pInfo->side == WORLD_SIDE::BEHIND) {
//			if (pInfo->bJump == true)
//			{// ���n����
//				// SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//		}
//		break;
//
//		//*********************************
//		// ���ɓ�������
//		//*********************************
//	case CCollision::ROT::LEFT:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//		break;
//
//		//*********************************
//		// �E�ɓ�������
//		//*********************************
//	case CCollision::ROT::RIGHT:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//		break;
//
//		//*********************************
//		// ���܂���
//		//*********************************
//	case CCollision::ROT::UNKNOWN:
//		Death(&pInfo->pos);
//		break;
//	}
//}

//----------------------------
// �����߃u���b�N�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionFillBlock(Info *pInfo,CCollision::ROT ColliRot)
//{
//	Death(&pInfo->pos);
//}

//----------------------------
// �g�����|�����̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionTrampoline(Info *pInfo, CollInfo *pColli, CTrampoline *pTrampoline)
//{
//	// ���������������Ƃɏ�����؂�ւ�
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// ��ɓ�������
//		//*********************************
//	case CCollision::ROT::OVER:
//
//		// �ʒu�E�ړ��ʏC��
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// �\�̐��E�̃v���C���[
//		if (pInfo->side == WORLD_SIDE::FACE)
//		{
//			if (pInfo->posOld.y > pInfo->pos.y)
//			{// ���n����
//			 // SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				pTrampoline->SetState(CTrampoline::STATE::UP_LAND);
//				pTrampoline->SetSpringForce(pInfo->fMaxHeight);
//				pTrampoline->SetCount(CTrampoline::MAX_COUNT);
//			}
//
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//
//			if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND) {
//				SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
//			}
//		}
//		break;
//
//		//*********************************
//		// ���ɓ�������
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//		if (pInfo->side == WORLD_SIDE::BEHIND)
//		{
//			if (pInfo->posOld.y < pInfo->pos.y)
//			{// ���n����
//				// SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				pTrampoline->SetState(CTrampoline::STATE::DOWN_LAND);
//				pTrampoline->SetSpringForce(pInfo->fMaxHeight);
//				pTrampoline->SetCount(CTrampoline::MAX_COUNT);
//			}
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//
//			if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND) {
//				SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
//			}
//		}
//		break;
//	}
//}

//----------------------------
// �g�Q�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionSpike(Info *pInfo, CollInfo *pColli)
//{
//	switch (pColli->ColliRot)
//	{
//		//�㉺�ǂ��炩�ɓ�����Ύ��S
//		case CCollision::ROT::OVER:
//		case CCollision::ROT::UNDER:
//			// ���S����
//			Death(&pInfo->pos);
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//	}
//}

//----------------------------
// �ړ����̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionMoveBlock(Info *pInfo, CMoveBlock *pMoveBlock, CollInfo *pColli)
//{
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// ��ɓ�������
//		//*********************************
//	case CCollision::ROT::OVER:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// �\�̐��E�̃v���C���[�̏ꍇ
//		if (pInfo->side == WORLD_SIDE::FACE)
//		{
//			pInfo->move = pMoveBlock->GetMove();
//			if (pInfo->bJump == true)
//			{// ���n����
//			 // SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//		}
//		break;
//
//		//*********************************
//		// ���ɓ�������
//		//*********************************
//	case CCollision::ROT::UNDER:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//		// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//		if (pInfo->side == WORLD_SIDE::BEHIND)
//		{
//			pInfo->pos = pMoveBlock->GetMove();
//			if (pInfo->bJump == true)
//			{// ���n����
//			 // SE�Đ�
//				s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//			}
//			pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//			pInfo->bJump = false;	// �W�����v�\
//			pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//		}
//		break;
//
//		//*********************************
//		// ���ɓ�������
//		//*********************************
//	case CCollision::ROT::LEFT:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, pColli->fWidth);
//		break;
//
//		//*********************************
//		// �E�ɓ�������
//		//*********************************
//	case CCollision::ROT::RIGHT:
//		// �ʒu�E�ړ��ʏC��
//		FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, pColli->fWidth);
//		break;
//
//	case CCollision::ROT::UNKNOWN:
//		// �ړ��� -> �v���C���[�ւ̓����蔻�菈�������s
//		const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
//		const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
//		const float fWidth = pMoveBlock->GetWidth() * 0.5f;
//		const float fHeight = pMoveBlock->GetHeight() * 0.5f;
//
//		// �ړ�������̓����蔻��
//		for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++)
//		{
//			// �v���C���[�̂ǂ̕����ɓ������Ă��邩
//			pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOld, pColli->fWidth, pColli->fHeight, (VECTOL)nCntVec);
//
//			// ����ł�������Ȃ��Ȃ�A�X�L�b�v
//			if (pColli->ColliRot == CCollision::ROT::NONE || pColli->ColliRot == CCollision::ROT::UNKNOWN) continue;
//
//			// �������������i�㉺�E���E�j�𔽓]����
//			{
//				// ��������������int�^�ɕϊ�
//				const int nRot = (int)pColli->ColliRot;
//
//				pColli->ColliRot = (CCollision::ROT)(nRot - 1 + 2 * (nRot % 2));
//			}
//
//			// ������x�����蔻��
//			CollisionMoveBlock(pInfo, pMoveBlock, pColli);
//		}
//		break;
//	}
//}

//----------------------------
// 覐΂̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionMeteor(Info *pInfo, CollInfo *pColli)
//{
//	// ���S����
//	Death(&pInfo->pos);
//}

//----------------------------
// ���[�U�[�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionLaser(Info *pInfo, CRoadTripLaser *pRoadTripLaser, CollInfo *pColli, CollInfo *pOthColli)
//{
//	// �{��
//	{
//		// ���������������Ƃɏ�����؂�ւ�
//		switch (pColli->ColliRot)
//		{
//			//*********************************
//			// ��ɓ�������
//			//*********************************
//		case CCollision::ROT::OVER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[�̏ꍇ
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				pInfo->move = pRoadTripLaser->GetMove();
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[�̏ꍇ
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				pInfo->move = pRoadTripLaser->GetMove();
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN: 
//			// ���[�U�[ -> �v���C���[�ւ̓����蔻�菈�������s
//			const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
//			const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
//			const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
//			const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;
//
//			// �ړ�������̓����蔻��
//			for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++)
//			{
//				// �v���C���[�̂ǂ̕����ɓ������Ă��邩
//				pColli->ColliRot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pInfo->pos, pInfo->posOld, SIZE_WIDTH, SIZE_HEIGHT, (VECTOL)nCntVec);
//
//				// ����ł�������Ȃ��Ȃ�A�X�L�b�v
//				if (pColli->ColliRot == CCollision::ROT::NONE || pColli->ColliRot == CCollision::ROT::UNKNOWN) continue;
//
//				// �������������i�㉺�E���E�j�𔽓]����
//				{
//					// ��������������int�^�ɕϊ�
//					const int nRot = (int)pColli->ColliRot;
//
//					pColli->ColliRot = (CCollision::ROT)(nRot - 1 + 2 * (nRot % 2));
//				}
//
//				// ������x�����蔻��
//				CollisionLaser(pInfo, pRoadTripLaser, pColli, pOthColli);
//			}
//			break;
//		}
//	}
//
//	// ���[�U�[
//	{
//		// ���S����
//		Death(&pInfo->pos);
//	}
//}

//----------------------------
// �k�C�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionDog(Info *pInfo, CExtenddog *pExtenddog, CollInfo *pColli, CollInfo *pOthColli)
//{
//	// ���������������Ƃɏ�����؂�ւ�
//
//	CExtenddog::STATE State = pExtenddog->GetState();
//
//	// �n�E�X
//	{
//		switch (pColli->ColliRot)
//		{
//			//*********************************
//			// ��ɓ�������
//			//*********************************
//		case CCollision::ROT::OVER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_OVER(&pInfo->pos.y, pColli->maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[
//			if (pInfo->side == WORLD_SIDE::FACE) {
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_UNDER(&pInfo->pos.y, pColli->minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//
//	// ��
//	{
//		CExtenddog::STATE state = pExtenddog->GetState();
//		switch (pOthColli[0].ColliRot)
//		{
//			//*********************************
//			// ��ɓ�������
//			//*********************************
//		case CCollision::ROT::OVER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_OVER(&pInfo->pos.y, pOthColli[0].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[�̏ꍇ
//			if (pInfo->side == WORLD_SIDE::FACE){
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[0].maxPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[0].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[�̏ꍇ
//			if (pInfo->side == WORLD_SIDE::FACE){
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[0].maxPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[0].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//
//			if (state == CExtenddog::STATE::NONE) {
//				// �ʒu�E�ړ��ʏC��
//				FixPos_RIGHT(&pInfo->pos.x, pOthColli[0].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			}
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN: break;
//
//		}
//	}
//
//	// ��
//	{
//		switch (pOthColli[1].ColliRot)
//		{
//			//*********************************
//			// ��ɓ�������
//			//*********************************
//		case CCollision::ROT::OVER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_OVER(&pInfo->pos.y, pOthColli[1].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[
//			if (pInfo->side == WORLD_SIDE::FACE) {
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[1].maxPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[1].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[1].minPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[1].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pOthColli[1].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//
//	// �K
//	{
//		switch (pOthColli[2].ColliRot)
//		{
//			//*********************************
//			// ��ɓ�������
//			//*********************************
//		case CCollision::ROT::OVER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_OVER(&pInfo->pos.y, pOthColli[2].maxPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// �\�̐��E�̃v���C���[
//			if (pInfo->side == WORLD_SIDE::FACE) {
//
//				if (pInfo->bJump == true)
//					s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);
//
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[2].maxPos.y;// �ō��x���W�ݒ�
//			}
//
//			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
//			pInfo->bExtendDog = true;
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_UNDER(&pInfo->pos.y, pOthColli[2].minPos.y, &pInfo->move.y, SIZE_HEIGHT);
//
//			// ���̐��E�̃v���C���[�Ȃ�W�����v�\
//			if (pInfo->side == WORLD_SIDE::BEHIND) {
//
//				if (pInfo->bJump == true)
//					s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);
//
//				pInfo->bGround = true;
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pOthColli[2].minPos.y;// �ō��x���W�ݒ�
//			}
//
//			pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
//			pInfo->bExtendDog = true;
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pOthColli[2].minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pOthColli[2].maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN: Death(&pInfo->pos); break;
//		}
//	}
//}

//----------------------------
// �S�[���Q�[�g�̓����蔻�菈��
// Author:KEISUKE OTONO
//----------------------------
//void CPlayer::CollisionGoalGate(Info *pInfo, CollInfo *pColli)
//{
//	if (!pInfo->bGoal)
//	{
//		pInfo->bGoal = true;
//
//		int ParTex = RNLib::Texture().Load(GetParticleIdx(PARTI_TEX::GOAL_EFFECT));
//
//		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
//		{
//			Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
//		}
//	}
//}

//----------------------------
// �p�[�c�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionParts(Info *pInfo, CParts *pParts)
//{
//	if (!pParts->GetDisp()) return;
//
//	// �擾�����̂ŕ`��OFF
//	pParts->DispSwitch(false);
//
//	// �擾������
//	s_nNumGetParts++;
//
//	// �擾���������S�̐��Ɠ����Ȃ�A���P�b�g��ԉ\
//	if (s_nNumGetParts == CParts::GetNumAll()) s_bRideRocket = true;
//}

//----------------------------
// ���P�b�g�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionRocket(Info *pInfo, CRocket *pRocket)
//{
//	int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
//
//	if (!s_bRideRocket) return;
//
//	// ���P�b�g�ɓ���
//	pInfo->bRide = true;
//	pRocket->Ride();
//
//	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
//	{
//		Manager::EffectMgr()->ParticleCreate(ParTex, pInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
//	}
//}

//----------------------------
// �Y�̓����蔻�菈��
//----------------------------
//void CPlayer::CollisionPile(Info *pInfo, CollInfo *pColli, CPile *pPile)
//{
//	// ���������������Ƃɏ�����؂�ւ�
//	switch (pColli->ColliRot)
//	{
//		//*********************************
//		// ��ɓ�������
//		//*********************************
//		case CCollision::ROT::OVER:
//			// �\�̐��E�̃v���C���[
//			if (pInfo->side == WORLD_SIDE::FACE)
//			{
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//
//				//������x�̍������痎�����Ă���
//				if (pInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
//				{
//					pPile->CaveInTrunkHeight(pColli->maxPos.y - pInfo->pos.y - SIZE_HEIGHT);
//				}
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// �����
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::UNDER:
//			// ���̐��E�̃v���C���[
//			if (pInfo->side == WORLD_SIDE::BEHIND)
//			{
//				if (pInfo->bJump == true)
//				{// ���n����
//				 // SE�Đ�
//					s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);
//				}
//
//				//������x�̍������痎�����Ă���
//				if (pInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
//				{
//					pPile->CaveInTrunkHeight(pColli->minPos.y - pInfo->pos.y + SIZE_HEIGHT);
//				}
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// �����
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//				pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//			}
//			break;
//
//			//*********************************
//			// ���ɓ�������
//			//*********************************
//		case CCollision::ROT::LEFT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_LEFT(&pInfo->pos.x, pColli->minPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// �E�ɓ�������
//			//*********************************
//		case CCollision::ROT::RIGHT:
//			// �ʒu�E�ړ��ʏC��
//			FixPos_RIGHT(&pInfo->pos.x, pColli->maxPos.x, &pInfo->move.x, SIZE_WIDTH);
//			break;
//
//			//*********************************
//			// ���܂���
//			//*********************************
//		case CCollision::ROT::UNKNOWN:
//		{
//			if (s_nSwapInterval != 0) Death(&pInfo->pos);
//			else
//			{
//				if (pInfo->side == WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// �ō��x���W�ݒ�
//				else 								pInfo->fMaxHeight = pColli->minPos.y;// �ō��x���W�ݒ�
//
//				pInfo->move.y = 0.0f;
//				pInfo->bLandPile = true;// �����
//				pInfo->bGround = true;	// �n�ʂɐڂ��Ă���
//				pInfo->bJump = false;	// �W�����v�\
//			}
//		}break;
//	}
//}

//----------------------------
// �v���C���[���ݒ�
// �w�肳�ꂽ�ԍ��̃v���C���[����ݒ肵�܂��B
//----------------------------
void CPlayer::SetInfo(const Info info, const int nNum)
{
	if (0 <= nNum && nNum < NUM_PLAYER)
	{
		// �e�v���C���[���ݒ�
		m_aInfo[nNum] = info;
		m_aInfo[nNum].StartPos = info.pos;
	}
}

//----------------------------
// �v���C���[���ݒ�
//----------------------------
void CPlayer::SetInfo(Info p1, Info p2)
{
	// �e�v���C���[���ݒ�
	m_aInfo[0] = p1;	m_aInfo[0].StartPos = p1.pos;
	m_aInfo[1] = p2;	m_aInfo[1].StartPos = p2.pos;
}

//----------------------------
// �v���C���[�Ƀg�����|�����p�̃W�����v��ݒ�
//----------------------------
void CPlayer::SetTrampolineJump(Info*& pInfo, float fMaxHeight)
{
	SetSwapInterval();

	// �W�����v�ʂ��p��
	const float diff = -fMaxHeight - pInfo->pos.y;
	pInfo->move.y = diff / TRAMPOLINE_JUMP_COUNTER;

	pInfo->fTramTargetPosY = -fMaxHeight;
	pInfo->nTramJumpCounter = TRAMPOLINE_JUMP_COUNTER;
	pInfo->bTramJump = true;
	pInfo->bGround = false;
	pInfo->bJump = true;
}

//----------------------------
// �v���C���[���擾
// �w�肳�ꂽ���E�ɂ���v���C���[�̏���Ԃ��܂�
//----------------------------
CPlayer::Info *CPlayer::GetInfo(WORLD_SIDE side)
{
	// �P�o�̂��鐢�E�ƍ��v������P�o����Ԃ�
	if (m_aInfo[0].side == side) return &m_aInfo[0];

	// �Ⴄ�Ȃ�Q�o����Ԃ�
	else return &m_aInfo[1];
}

//----------------------------
// �v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigTrigger(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// �v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigPress(const int nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
//�v���C���[�pSE�Đ�
//----------------------------
void CPlayer::PlaySE(SE_LABEL label)
{
	switch (label)
	{
		case CPlayer::SE_LABEL::JUMP:	s_SE.pSound->Play(s_SE.jump, CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::LANDING:s_SE.pSound->Play(s_SE.landing, CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_00:	s_SE.pSound->Play(s_SE.dog[0], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_01:	s_SE.pSound->Play(s_SE.dog[1], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_02:	s_SE.pSound->Play(s_SE.dog[2], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::DOG_03:	s_SE.pSound->Play(s_SE.dog[3], CSound::CATEGORY::SE, false);	break;
		case CPlayer::SE_LABEL::SWAP:	s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);	break;
	}
}