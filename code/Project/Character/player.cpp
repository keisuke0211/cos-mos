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
#include"../UI/miss.h"
#include"../Object/Block/Ghost.h"

// �X���b�v�C���^�[�o��
const int	CPlayer::SWAP_INTERVAL = 90;	// �X���b�v�C���^�[�o��
int			CPlayer::s_nSwapInterval = 0;	// �c��X���b�v�C���^�[�o��
bool		CPlayer::s_bSwapAnim = false;	//�X���b�v�A�j���[�V���������ǂ���
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//�A�j���[�V�����\��

int			CPlayer::s_nGoalInterval = 0;//�S�[����̗]�C�J�E���^�[

const float CPlayer::SIZE_WIDTH = 7.0f;	// ����
const float CPlayer::SIZE_HEIGHT = 8.0f;// ����

const float CPlayer::MOVE_SPEED = 0.3f;		// �ړ���
const float CPlayer::MAX_MOVE_SPEED = 2.3f;	// �ő�ړ���

const float CPlayer::JUMP_POWER = 5.0f;		// ��{�W�����v��
const float CPlayer::GRAVITY_POWER = -0.3f;	// ��{�d�͉����x

const int	CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// �X���b�v�}�[�N
	"data\\TEXTURE\\Effect\\eff_Star_000.png",	// �X���b�v�p�[�e�B�N��00
	"data\\TEXTURE\\Effect\\ink001.png",		// �X���b�v�p�[�e�B�N��01
	"data\\TEXTURE\\Effect\\ink002.png",		// �X���b�v�p�[�e�B�N��02
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// ���S�}�[�N
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// ���S�p�[�e�B�N��
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",	// �S�[���E���P�b�g��Ԏ��̃G�t�F�N�g
};
int			CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE CPlayer::s_SE = {};	//�T�E���h�p�\����
CPlayer::Motion CPlayer::s_motion = {};	//���[�V�����p�\����
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

	//�S�[����̗]�C�J�E���^�[
	s_nGoalInterval = 0;

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
		Player.expandCounter = 0;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.StartPos = INITD3DXVECTOR3;		// �J�n�ʒu
		Player.doll = NULL;
		Player.pos = INITD3DXVECTOR3;			// �ʒu
		Player.posOld = INITD3DXVECTOR3;		// �O��ʒu
		Player.rot = INITD3DXVECTOR3;			// ����
		Player.move = INITD3DXVECTOR3;			// �ړ���
		Player.color = INITCOLOR;				// �F
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA;	// �X���b�v�}�[�N�̃��l
		Player.fSwapPosY = 0.0f;				// �X���b�v��̂x���W
		Player.fSwapMoveY = 0.0f;				// �X���b�v�ړ����̑��x
		Player.bGround = false;					// �n�ʂɐڂ��Ă��邩
		Player.bGroundOld = false;				// �n�ʂɐڂ��Ă��邩(�ߋ�)7
		Player.landingCounter = false;
		Player.bJump = false;					// �W�����v
		Player.bRide = false;					// ���P�b�g�ɏ���Ă��邩�ǂ���
		Player.bGoal = false;					// �S�[���������ǂ���
		Player.fJumpPower = 0.0f;				// �W�����v��
		Player.fGravity = 0.0f;					// �d��
		Player.fMaxHeight = 0.0f;				// �ō��x���W
		Player.nTramJumpCounter = 0;			// �g�����|�����ɂ���Ē��ˏオ��ō����B�n�_
		Player.fTramTargetPosY = 0.0f;			// �g�����|�����p�̖ڕW�ʒu
		Player.bTramJump = false;				// �g�����|�����p�̓���W�����v
		Player.bExtendDog = false;				// �k�C�p�̐ڐG�t���O
		Player.bLandPile = false;				// �Y�ɏ���Ă��邩�ǂ���
		Player.side = WORLD_SIDE::FACE;			// �ǂ���̐��E�ɑ��݂��邩
	}

	s_pColli = NULL;	// �����蔻��N���X
}

//=======================================
// �f�X�g���N�^
//=======================================
CPlayer::~CPlayer()
{
	delete m_aInfo[0].doll;
	delete m_aInfo[1].doll;

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
	m_aInfo[0].doll = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"));
	m_aInfo[0].rot = Rot3D(0.0f, D3DX_PI, 0.0f);

	// �Q�o�������
	m_aInfo[1].doll = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"));
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
	s_SE.expand = s_SE.pSound->Load("data\\SOUND\\SE\\death_expand.wav");
	s_SE.explosion = s_SE.pSound->Load("data\\SOUND\\SE\\death_explosion.wav");

	s_motion.neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Default.txt");
	s_motion.walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Walk.txt");
	s_motion.jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Jump.txt");
	s_motion.fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Fall.txt");
	s_motion.landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Landing.txt");

	InitInfo();

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
// ��񏉊�������
//=====================================================================================================================
void CPlayer::InitInfo(void) {

	// �P�o�p�������
	m_aInfo[0].fJumpPower = JUMP_POWER;
	m_aInfo[0].fGravity = GRAVITY_POWER;
	m_aInfo[0].side = WORLD_SIDE::FACE;
	m_aInfo[0].rot.z = 0.0f;
	m_aInfo[0].rot.y = D3DX_PI;
	m_aInfo[0].scale = INITSCALE3D;

	// �Q�o�p�������
	m_aInfo[1].fJumpPower = -JUMP_POWER;
	m_aInfo[1].fGravity = -GRAVITY_POWER;
	m_aInfo[1].side = WORLD_SIDE::BEHIND;
	m_aInfo[1].rot.z = D3DX_PI;
	m_aInfo[1].rot.y = D3DX_PI;
	m_aInfo[1].scale = INITSCALE3D;

	// ���ҋ��ʏ������
	for each (Info & Player in m_aInfo)
	{
		Player.posOld = Player.pos = Player.StartPos;
		Player.move = INITD3DXVECTOR3;
		Player.bGround = false;
		Player.bJump = true;
		Player.bRide = false;
		Player.bGoal = false;
		Player.bTramJump = false;
		Player.expandCounter = 0;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
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
		if (++s_nGoalInterval >= GOAL_INTERVAL)
		{
			const int planet = Manager::StgEd()->GetPlanetIdx();
			const int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
			Manager::StgEd()->SwapStage(stage + 1);
		}
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

		// �ʒu�ݒ�
		if (Player.deathCounter == 0 && Player.deathCounter2 == 0 && !Player.bGoal && !s_bSwapAnim) {
			Player.doll->SetPos(Player.pos - Pos3D(0.0f, (fabsf(Player.pos.y) / Player.pos.y) * SIZE_HEIGHT, 0.0f));
			Player.doll->SetRot(Player.rot);
			Player.doll->SetScale(Player.scale);
		}
		else {
			Player.doll->SetPos(Pos3D(10000.0f, 10000.0f, 10000.0f));
		}

		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal) continue;

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

	// �����~�t���O���Z�o
	bool isControlStop = m_aInfo[0].expandCounter > 0 || m_aInfo[1].expandCounter > 0 || m_aInfo[0].deathCounter > 0 || m_aInfo[1].deathCounter > 0 || m_aInfo[0].deathCounter2 > 0 || m_aInfo[1].deathCounter2 > 0;

	if (!isControlStop) {
		// [[[ �J�������� ]]]
		Pos3D pos = (m_aInfo[0].pos + m_aInfo[1].pos) * 0.5f;
		pos.x *= 0.25f;
		pos.y = 0.0f;
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::GetMainCamera()->GetPosV(), pos);
	}

	for each (Info & Player in m_aInfo)
	{
		// ���̃v���C���[�ԍ���
		nIdxPlayer++;

		// ���S�J�E���^�[&���o
		static Vector3D addVec = INITVECTOR3D;
		static Pos3D posVTemp = INITVECTOR3D;
		static Pos3D posRTemp = INITVECTOR3D;
		if (Player.deathCounter > 0) {
			if (--Player.deathCounter == 0) {
				Player.deathCounter2 = DEATH_TIME2;
				addVec = INITVECTOR3D;
				posVTemp = Manager::GetMainCamera()->GetPosV();
				posRTemp = Manager::GetMainCamera()->GetPosR();

				// �~�X�̃e�L�X�g����
				CMiss::Create();

				// �S�[�X�g�̐���
				CGhost::Create(Player.pos, nIdxPlayer);
			}

			float rate = (float)Player.deathCounter / DEATH_TIME;
			float rateOpp = 1.0f - rate;
			Manager::GetMainCamera()->SetMotionBlurColor(Color{ 255,(UShort)(255 * rateOpp),(UShort)(255 * rateOpp),255 });
			Manager::GetMainCamera()->SetMotionBlurPower(0.25f + rate * 0.25f);
			Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.1f));
		}
		// ���S�J�E���^�[2&���o
		if (Player.deathCounter2 > 0) {

			if (--Player.deathCounter2 == 0) {
				InitInfo();

				//�I�u�W�F�N�g�̃|�C���^���i�[
				CObject *obj = NULL;

				//�I�u�W�F�N�g���擾
				while (Manager::StageObjectMgr()->ListLoop(&obj)) {
					//�擾�����I�u�W�F�N�g���L���X�g
					CStageObject* stageObj = (CStageObject*)obj;

					//��ގ擾
					const CStageObject::TYPE type = stageObj->GetType();

					switch (type)
					{
					case CStageObject::TYPE::MISS:
					{
						//�擾�����I�u�W�F�N�g���L���X�g
						CMiss* Miss = (CMiss*)obj;

						Miss->Delete();	// �폜����
						break;
					}
					case CStageObject::TYPE::GHOST:
					{
						//�擾�����I�u�W�F�N�g���L���X�g
						CGhost* Ghost = (CGhost*)obj;

						Ghost->Delete();	// �폜����
						break;
					}
					}
				}
			}

			bool isReturn = false;
			int counter = Player.deathCounter2;
			if (counter > DEATH_TIME2 / 2)
				counter -= DEATH_TIME2 / 2;
			float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, counter, DEATH_TIME2 / 2);
			if (rate > 0.5f) {
				rate = 0.5f + (0.5f - rate);
				isReturn = true;
			}
			rate /= 0.5f;

			float rate2 = ((float)Player.deathCounter2 / DEATH_TIME2);
			if (rate2 > 0.5f) {
				rate2 = 0.5f + (0.5f - rate2);
			}
			rate2 /= 0.5f;

			addVec += CGeometry::GetRandomVec();
			Manager::GetMainCamera()->SetMotionBlurPower(isReturn ? 0.25f + rate * 0.25f : rate * 0.5f);
			Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.02f));
			Manager::GetMainCamera()->SetPosVAndPosR(
				((Manager::GetMainCamera()->GetPosV() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posVTemp * (1.0f - rate2)),
				((Manager::GetMainCamera()->GetPosR() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posRTemp * (1.0f - rate2)));
		}
		// �c��݃J�E���^�[�����o
		else if (Player.expandCounter > 0) {
			if (--Player.expandCounter == 0) {
				RNLib::Sound().Play(s_SE.explosion, CSound::CATEGORY::SE, false);
				Manager::GetMainCamera()->SetVib(5.0f);

				const int NUM_PARTICLE = 8;
				Pos3D rot = INITVECTOR3D;
				for (int ParCnt = 0; ParCnt < NUM_PARTICLE; ParCnt++)
				{
					rot.z = -D3DX_PI + D3DX_PI_DOUBLE * fRand();
					CEffect_Death *pEff = Manager::EffectMgr()->DeathParticleCreate(
						RNLib::Model().Load("data\\MODEL\\Effect\\Ball.x"), Player.pos, INITVECTOR3D, rot, INITVECTOR3D, 0.0f, Color{ 255, 155, 59,255 }, CEffect_Death::TYPE::BALL);
					
					const CEffect_Death::BALL_SIZE_LV Lv = (CEffect_Death::BALL_SIZE_LV)(rand() % (int)(CEffect_Death::BALL_SIZE_LV::MAX));
					pEff->SetBallSize(Lv);
				}
				Player.deathCounter = DEATH_TIME;
			}
			Player.scale.x =
			Player.scale.y =
			Player.scale.z = 1.0f + (1.0f - CEase::Easing(CEase::TYPE::IN_SINE, Player.expandCounter, EXPAND_TIME)) * 0.2f;
		}

		// �����~�ł���ΐ܂�Ԃ�
		if (isControlStop)
			continue;

		// �������S�[�����Ă��Ȃ���Ώo��
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer + 1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			CGoalGate::EntrySub();
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

			Player.doll->OverwriteMotion(s_motion.jump);
		}

		bool isMove = false;

		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT, nIdxPlayer)) 
		{// �E�Ɉړ�
			Player.move.x += MOVE_SPEED;
			Player.rot.y += CGeometry::FindAngleDifference(Player.rot.y, D3DX_PI * 0.7f) * 0.5f;
			isMove = true;
		}
		else if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
			RNLib::Input().GetStickAnglePress(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT, nIdxPlayer)) 
		{// ���Ɉړ�
			Player.move.x -= MOVE_SPEED;
			Player.rot.y += CGeometry::FindAngleDifference(Player.rot.y, -D3DX_PI * 0.7f) * 0.5f;
			isMove = true;
		}

		if (!Player.bGround) {
			if ((Player.pos.y > 0.0f && Player.move.y < 0.0f) || (Player.pos.y < 0.0f && Player.move.y > 0.0f)) {
				Player.doll->OverwriteMotion(s_motion.fall);
			}
		}
		else if (Player.landingCounter == 0) {
			if (isMove) {
				Player.doll->OverwriteMotion(s_motion.walk);
			}
			else {
				Player.doll->OverwriteMotion(s_motion.neutral);
			}
		}
		else {
			Player.landingCounter--;
		}

		// �X���b�v����
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP))
		{
			//Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI00), Player.pos, INIT_EFFECT_SCALE, Color{ 255,200,0,255 });
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
		//���P�b�g�ɏ���Ă��Ȃ��Ƃ��ɃT�E���h�Đ�
		if (CRocket::GetCounter() != NUM_PLAYER)
		{
			// �X���b�v�A�j���[�V�����ݒ�
			s_AnimState = SWAP_ANIM::PROLOGUE;
			s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
			s_bSwapAnim = true;
			s_SE.pSound->Play(s_SE.Swap, CSound::CATEGORY::SE, false);
		}

		for each (Info &Player in m_aInfo)
		{
			// ���P�b�g�ɏ���Ă���X�L�b�v
			if (Player.bRide) continue;

			/*for (int i = 0; i < 16; i++)
			{
				Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SWAP_PARTI00), Player.pos, INIT_EFFECT_SCALE, INITCOLOR);
			}*/

			// �ʒu�E�d�͉����x�E�W�����v�ʁE���݂��鐢�E�𔽓]
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
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
	//�C���^�[�o������
	s_nSwapInterval--;

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//�v���C���[���Q�Ɓi�ړ��������p�[�e�B�N���̈ʒu����O�ɏo��
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer);break; //�v�����[�O
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;	//����
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer);break;//�G�s���[�O
		}

		for (int nCntPar = 0; nCntPar < 1; nCntPar++)
		{
			Color setCol;
			if (nCntPlayer == 0){
				setCol = Color{ (UShort)(215 + rand() % 40),(UShort)(135 + rand() % 40),(UShort)(39 + rand() % 40),255 };
			}
			else{
				setCol = Color{ (UShort)(45 + rand() % 40),(UShort)(130 + rand() % 125),(UShort)(130 + rand() % 125),255 };
			}

			int nTex = rand() % 2 + 2;

			Manager::EffectMgr()->ParticleCreate(GetParticleIdx((PARTI_TEX)nTex), Player.pos, Vector3D(16.0f, 16.0f, 0.0f), setCol,CParticle::TYPE::TYPE_NORMAL,300,D3DXVECTOR3(0.0f,0.0f,(float)(rand() % 629 - 314) / 100.0f),8, CDrawState::ALPHA_BLEND_MODE::NORMAL);
		}
	}
}

//*************************************************
//�v�����[�O����
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, const int nIdxPlayer)
{
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
	//�Ō�̃v���C���[�̂Ƃ��ɃX���b�v�A�j���[�V�����I��
	if (s_nSwapInterval > 0 && nIdxPlayer == 0) return;
	s_bSwapAnim = false;
}

//----------------------------
// ���S����
//----------------------------
void CPlayer::Death(Info& Player, const OBJECT_TYPE type, const int *pColliRot)
{
	if (Player.expandCounter > 0 || Player.deathCounter > 0 || Player.deathCounter2 > 0)
		return;

	Player.expandCounter = EXPAND_TIME;
	RNLib::Sound().Play(s_SE.expand, CSound::CATEGORY::SE, false);
}

//----------------------------
// �ړ�����
//----------------------------
void CPlayer::Move(VECTOL vec)
{
	if (m_aInfo[0].expandCounter > 0 || m_aInfo[1].expandCounter > 0 || 
		m_aInfo[0].deathCounter > 0 || m_aInfo[1].deathCounter > 0 ||
		m_aInfo[0].deathCounter2 > 0 || m_aInfo[1].deathCounter2 > 0) {
		return;
	}

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
			const OBJECT_TYPE type = stageObj->GetType();

			// �I�u�W�F�N�g�̓����蔻�����ݒ�
			CCollision::ColliInfo colliInfo;
			colliInfo.pos     = stageObj->GetPos();
			colliInfo.posOld  = colliInfo.pos;
			colliInfo.fWidth  = stageObj->GetWidth() * 0.5f;
			colliInfo.fHeight = stageObj->GetHeight() * 0.5f;
			colliInfo.minPos  = D3DXVECTOR3(colliInfo.pos.x - colliInfo.fWidth, colliInfo.pos.y - colliInfo.fHeight, 0.0f);
			colliInfo.maxPos  = D3DXVECTOR3(colliInfo.pos.x + colliInfo.fWidth, colliInfo.pos.y + colliInfo.fHeight, 0.0f);

			int nColliRot[NUM_PLAYER] = {};
			int nCntPlayer = NONEDATA;
			// �v���C���[���ɏ������Ă���
			for each(Info& Player in m_aInfo) {
				nCntPlayer++;
				// ���P�b�g�ɏ���Ă邩�S�[�����Ă�����܂�Ԃ�
				if (Player.bRide || Player.bGoal)
					continue;

				// �v���C���[�̋߂��ɃI�u�W�F�N�g�����邩����
				// ������I�u�W�F�N�g������
				if (type != OBJECT_TYPE::TRAMPOLINE && 
					type != OBJECT_TYPE::LASER && 
					type != OBJECT_TYPE::EXTEND_DOG) {

					if (D3DXVec3Length(&(colliInfo.pos - Player.pos)) >
						D3DXVec2Length(&D3DXVECTOR2(colliInfo.fWidth + SIZE_WIDTH, colliInfo.fHeight + SIZE_HEIGHT)))
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

				//�ʂ̓����蔻����
				CCollision::ColliInfo *OtherInfo = NULL;

				// �ړ�����I�u�W�F�N�g�́A
				// �����蔻��ʒu�ɑO��ʒu��ݒ肷��
				switch (type) {
					// �u���b�N
				case OBJECT_TYPE::BLOCK: {
					CBlock* pBlock = (CBlock*)stageObj;
					if (!pBlock->GetCollision())
						continue;
				}break;

					// �S�[���Q�[�g
				case OBJECT_TYPE::GOALGATE: {
					CGoalGate* pGoalGate = (CGoalGate*)stageObj;
					if (pGoalGate->GetStartGate())
						continue;
				}break;

					// �ړ���
				case OBJECT_TYPE::MOVE_BLOCK:{
					CMoveBlock *pBlock = (CMoveBlock*)stageObj;
					colliInfo.posOld = pBlock->GetPosOld();
				}
				break;

					// 覐�
				case OBJECT_TYPE::METEOR:{
					CMeteor *pMeteor = (CMeteor *)stageObj;
					colliInfo.posOld = pMeteor->GetPosOld();
				}
				break;

					// ���[�U�[
				case OBJECT_TYPE::LASER:{
					CRoadTripLaser *pLaser = (CRoadTripLaser *)stageObj;

					OtherInfo = new CCollision::ColliInfo;
					
					OtherInfo->pos = pLaser->GetLaserPos();
					OtherInfo->posOld = OtherInfo->pos;
					OtherInfo->fWidth = pLaser->GetLaserSize().x * 0.5f;
					OtherInfo->fHeight = pLaser->GetLaserSize().y * 0.5f;

					// ���������������i�[
					OtherInfo->Rot = s_pColli->IsBoxCollider(Self, *OtherInfo, vec);
				}
				break;

					// �k�C
				case OBJECT_TYPE::EXTEND_DOG:{
					//CExtenddog *pDog = (CExtenddog *)stageObj;

					//OtherInfo = new CollInfo[OBJ_EXTENDDOG];

					//for (int nCnt = 0; nCnt < OBJ_EXTENDDOG; nCnt++){
					//	switch (nCnt){
					//	case 0:
					//		OtherInfo[nCnt].pos = pDog->GetHeadPos();
					//		OtherInfo[nCnt].posOld = pDog->GetHeadPosOid();
					//		break;
					//	case 1:
					//		OtherInfo[nCnt].pos = pDog->GetBodyPos();
					//		OtherInfo[nCnt].posOld = pDog->GetBodyPos();
					//		break;
					//	case 2:
					//		OtherInfo[nCnt].pos = pDog->GetHipPos();
					//		OtherInfo[nCnt].posOld = pDog->GetHipPos();
					//		break;
					//	}

					//	OtherInfo[nCnt].fWidth = pDog->GetWidth() * 0.5f;
					//	OtherInfo[nCnt].fHeight = pDog->GetHeight() * 0.5f;

					//	if (D3DXVec3Length(&(OtherInfo[nCnt].pos - Player.pos)) >
					//		D3DXVec2Length(&D3DXVECTOR2(OtherInfo[nCnt].fWidth + SIZE_WIDTH, OtherInfo[nCnt].fHeight + SIZE_HEIGHT))) continue;

					//	// �I�u�W�F�N�g�̍ŏ��E�ő�ʒu
					//	OtherInfo[nCnt].minPos = D3DXVECTOR3(OtherInfo[nCnt].pos.x - OtherInfo[nCnt].fWidth, OtherInfo[nCnt].pos.y - OtherInfo[nCnt].fHeight, 0.0f);
					//	OtherInfo[nCnt].maxPos = D3DXVECTOR3(OtherInfo[nCnt].pos.x + OtherInfo[nCnt].fWidth, OtherInfo[nCnt].pos.y + OtherInfo[nCnt].fHeight, 0.0f);

					//	// ���������������i�[
					//	OtherInfo[nCnt].ColliRot = s_pColli->IsBoxCollider(*pSelf, colliInfo, vec);

					//	//if (OtherInfo[nCnt].ColliRot != CCollision::ROT::NONE){
					//	//	bOtherColl = true;
					//	//}
					//}

					//CExtenddog::STATE state = pDog->GetState();
					//if ((OtherInfo[2].ColliRot != CCollision::ROT::UNDER) ||
					//	(OtherInfo[2].ColliRot != CCollision::ROT::OVER && state == CExtenddog::STATE::DOWN_LAND)) {
					//	Player.bExtendDog = false;
					//}
				}
				break;

					//�Y
				case OBJECT_TYPE::PILE:
				{
					CPile *pPile = (CPile *)stageObj;

					colliInfo.pos = pPile->GetPos();
					colliInfo.posOld = pPile->GetPosOld();
				}
				break;
				}

				// ���������������i�[
				colliInfo.Rot = s_pColli->IsBoxCollider(Self, colliInfo, vec);
				nColliRot[nCntPlayer] = (int)colliInfo.Rot;

				// �������Ă��Ȃ�
				if (colliInfo.Rot == CCollision::ROT::NONE)
				{
					//�ʂ̓����蔻����폜
					if (OtherInfo != NULL)
					{
						delete[] OtherInfo;
						OtherInfo = NULL;
					}

					//�Y�ɓ������Ă��Ȃ��Ȃ����Ă��Ȃ�
					if (type == OBJECT_TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				//���S����
				bool bDeath = false;

				// ��ނ��ƂɊ֐�����
				switch (type)
				{
				case OBJECT_TYPE::BLOCK:	 s_pColli->Block(&Self, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::FILLBLOCK: s_pColli->FillBlock(&Self, colliInfo.Rot, &Player.side, &bDeath); break;
				case OBJECT_TYPE::TRAMPOLINE:s_pColli->Trampoline(&Self, &colliInfo, (CTrampoline *)stageObj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::SPIKE:	 s_pColli->Spike(&Self, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::MOVE_BLOCK:s_pColli->MoveBlock(&Self, (CMoveBlock *)stageObj, &colliInfo, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::METEOR:	 s_pColli->Meteor(&Self, &colliInfo, &Player.side, &bDeath); break;
				case OBJECT_TYPE::LASER:	 s_pColli->Laser(&Self, (CRoadTripLaser *)stageObj,&colliInfo, NULL, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::EXTEND_DOG:s_pColli->Dog(&Self, (CExtenddog *)stageObj, &colliInfo, NULL, &Player.side, &bDeath); break;
				case OBJECT_TYPE::GOALGATE:	 s_pColli->GoalGate(&Self, &colliInfo, obj, &Player.side, &bDeath);	break;
				case OBJECT_TYPE::PARTS:	 s_pColli->Parts(&Self, (CParts *)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::ROCKET:	 s_pColli->Rocket(&Self, (CRocket *)stageObj, &Player.side, &bDeath); break;
				case OBJECT_TYPE::PILE:		 s_pColli->Pile(&Self, &colliInfo, (CPile *)stageObj, &Player.side, &bDeath); break;
				}

				//�ʂ̓����蔻����폜
				if (OtherInfo != NULL)
				{
					delete[] OtherInfo;
					OtherInfo = NULL;
				}

				// ���S����ON
				if (bDeath)
				{
					Death(Player, type, &nColliRot[nCntPlayer]);
					break;
				}

				//�����
				Player.pos = Self.pos;
				Player.move = Self.move;
			}

			//�����蔻��̎��㏈��
			CollisionAfter(stageObj, type, &nColliRot[0]);
		}
	}
}

//----------------------------
// �e�v���C���[�̓����蔻�肪�I�������̏���
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, int *pColliRot)
{
	// ��ނ��ƂɊ֐�����
	switch (type)
	{
		//�u���b�N�̃��A�N�V�����t���O
		case CStageObject::TYPE::BLOCK:
		{
			typedef CCollision::ROT ColRot;		 //�Փ˕����̕ʖ�
			Info *pInfo = &m_aInfo[0];			 //�v���C���[���̃|�C���^
			CBlock *pBlock = (CBlock *)pStageObj;//�u���b�N�փL���X�g

			for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++, pInfo++, pColliRot++)
			{
				pBlock->IsReaction_HitsRot(*pColliRot);	//����������������
				pBlock->IsReaction_Move(D3DXVec3Length(&pInfo->move) != 0.0f);
				
				//���n�������ǂ���
				if (((ColRot)*pColliRot == ColRot::OVER  && pInfo->side == WORLD_SIDE::FACE && pInfo->pos.y < pInfo->posOld.y) ||
					((ColRot)*pColliRot == ColRot::UNDER && pInfo->side == WORLD_SIDE::BEHIND && pInfo->pos.y > pInfo->posOld.y))
					pBlock->IsReaction_Land(true);
			}
			break;
		}

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