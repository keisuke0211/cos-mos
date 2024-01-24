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
#include "../resource.h"
#include "../stage.h"

const Color CPlayer::P1_COLOR = Color{ 255, 155, 59, 255 };
const Color CPlayer::P2_COLOR = Color{ 65, 233, 210, 255 };

// �X���b�v�C���^�[�o��
CInt   CPlayer::SWAP_INTERVAL = 20;	// �X���b�v�C���^�[�o��
CFloat CPlayer::GUIDE_WIDTH   = 10.0f; // �K�C�h�̕�
CFloat CPlayer::GUIDE_HEIGHT  = 14.0f; // �K�C�h�̍���
CFloat CPlayer::MAX_GUIDE_SPEED = 0.2f;// �K�C�h�A�j���[�V�����̍ő呬�x

int                CPlayer::s_nSwapInterval = 0; // �c��X���b�v�C���^�[�o��
bool               CPlayer::s_bSwapAnim = false; //�X���b�v�A�j���[�V���������ǂ���
CPlayer::SWAP_ANIM CPlayer::s_AnimState = CPlayer::SWAP_ANIM::PROLOGUE;	//�A�j���[�V�����\��

int CPlayer::s_nGoalInterval = 0;//�S�[����̗]�C�J�E���^�[
int CPlayer::s_zoomUpCounter = 0;// �Y�[���A�b�v�J�E���^�[
int CPlayer::s_zoomUpFixedCounter = 0;// �Y�[���A�b�v�J�E���^�[

CFloat CPlayer::SIZE_WIDTH = 7.0f; // ����
CFloat CPlayer::SIZE_HEIGHT = 8.0f;// ����

CFloat CPlayer::MOVE_SPEED = 0.3f;     // �ړ���
CFloat CPlayer::MAX_MOVE_SPEED = 2.3f; // �ő�ړ���

CFloat CPlayer::JUMP_POWER = 5.0f;     // ��{�W�����v��
CFloat CPlayer::GRAVITY_POWER = -0.3f; // ��{�d�͉����x

CInt CPlayer::TRAMPOLINE_JUMP_COUNTER = 10;

const char *CPlayer::PARTICLE_TEX_PATH[(int)PARTI_TEX::MAX] = {
	"data\\TEXTURE\\Effect\\eff_Circle_005.png",// �X���b�v�}�[�N
	"data\\TEXTURE\\Effect\\eff_Star_000.png",  // �X���b�v�p�[�e�B�N��00
	"data\\TEXTURE\\Effect\\ink001.png",        // �X���b�v�p�[�e�B�N��01
	"data\\TEXTURE\\Effect\\ink002.png",        // �X���b�v�p�[�e�B�N��02
	"data\\TEXTURE\\Effect\\swap_guide.png",    // �X���b�v�K�C�h
	"data\\TEXTURE\\player.PNG",                // �v���C���[�A�C�R��
	"data\\TEXTURE\\Effect\\mark_Skull_000.png",// ���S�}�[�N
	"data\\TEXTURE\\Effect\\ink001.png",        // ���S�C���N
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // ���S�p�[�e�B�N��
	"data\\TEXTURE\\Effect\\eff_Hit_002.png",   // �S�[���E���P�b�g��Ԏ��̃G�t�F�N�g
	"data\\TEXTURE\\Effect\\eff_Smoke_000.png", // ���̃e�N�X�`��
};
int CPlayer::s_ParticleTex[(int)PARTI_TEX::MAX] = {};

CPlayer::SE     CPlayer::s_SE = {};	//�T�E���h�p�\����
CPlayer::Motion CPlayer::s_motion[2] = {};	//���[�V�����p�\����
bool            CPlayer::ms_bSwapEnd = false;
UShort          CPlayer::ms_guideCounter = 0;

bool  CPlayer::s_bAimPlayer = false;
int   CPlayer::s_nAimNo = 0;
Pos3D CPlayer::s_addPosV = INITPOS3D;
Pos3D CPlayer::s_addPosR = INITPOS3D;
float CPlayer::s_fCorrWidth = 0.0f;
float CPlayer::s_fCorrHeight = 0.0f;
float CPlayer::s_fAimWorkSpeed = 1.0f;
CPlayer::Info CPlayer::m_aInfo[CPlayer::NUM_PLAYER];	// �e�v���C���[�̏��

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
	s_SE.Swaping = 0;
	s_SE.SwapEnd = 0;
	for each(short &dog in s_SE.dog)
	{
		dog = 0;
	}

	int cntPlayer = 0;
	for each(Info &Player in m_aInfo)
	{
		Player.idx = cntPlayer;
		Player.expandCounter = 0;
		Player.isDeath = false;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.DeathType = 0;
		Player.doll = NULL;
		Player.pos = INITD3DXVECTOR3;          // �ʒu
		Player.posOld = INITD3DXVECTOR3;       // �O��ʒu
		Player.rot = INITD3DXVECTOR3;          // ����
		Player.move = INITD3DXVECTOR3;         // �ړ���
		Player.color = COLOR_WHITE;            // �F
		Player.nSwapAlpha = NORMAL_SWAP_ALPHA; // �X���b�v�}�[�N�̃��l
		Player.fSwapPosY = 0.0f;               // �X���b�v��̂x���W
		Player.fSwapMoveY = 0.0f;              // �X���b�v�ړ����̑��x
		Player.fGuideTexVPos = 0.0f;           // �K�C�h�̃e�N�X�`���u���W
		Player.fGuideTexVSize = 0.0f;          // �K�C�h�̃e�N�X�`���u�T�C�Y
		Player.fGuideMoveSpeed = 0.0f;         // �K�C�h�̃e�N�X�`���ړ��X�s�[�h
		Player.bGround = true;                 // �n�ʂɐڂ��Ă��邩
		Player.bGroundOld = true;              // �n�ʂɐڂ��Ă��邩(�ߋ�)
		Player.landingCounter = false;
		Player.bJump = false;                  // �W�����v
		Player.bRide = false;                  // ���P�b�g�ɏ���Ă��邩�ǂ���
		Player.bGoal = false;                  // �S�[���������ǂ���
		Player.pGoalGate = NULL;
		Player.fJumpPower = 0.0f;              // �W�����v��
		Player.fGravity = 0.0f;                // �d��
		Player.fMaxHeight = 0.0f;              // �ō��x���W
		Player.nTramJumpCounter = 0;           // �g�����|�����ɂ���Ē��ˏオ��ō����B�n�_
		Player.fTramTargetPosY = 0.0f;         // �g�����|�����p�̖ڕW�ʒu
		Player.bTramJump = false;              // �g�����|�����p�̓���W�����v
		Player.bExtendDog = false;             // �k�C�p�̐ڐG�t���O
		Player.bLandPile = false;              // �Y�ɏ���Ă��邩�ǂ���
		Player.side = WORLD_SIDE::FACE;        // �ǂ���̐��E�ɑ��݂��邩
		cntPlayer++;
	}
}

//=======================================
// �f�X�g���N�^
//=======================================
CPlayer::~CPlayer()
{
	delete m_aInfo[0].doll;
	delete m_aInfo[1].doll;
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
	ms_bSwapEnd = false;
	ms_guideCounter = 0;

	// �P�o�������
	if (m_aInfo[0].doll != NULL)
		delete m_aInfo[0].doll;
	m_aInfo[0].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Mouth.txt"), TRUE);
	m_aInfo[0].rot = Rot3D(0.0f, D3DX_PI, 0.0f);
	m_aInfo[0].color = P1_COLOR;

	// �Q�o�������
	if (m_aInfo[1].doll != NULL)
		delete m_aInfo[1].doll;
	m_aInfo[1].doll = new CDoll3D(PRIORITY_PLAYER, RNLib::SetUp3D().Load("data\\SETUP\\Player_Eye.txt"), TRUE);
	m_aInfo[1].rot = CStageObject::INVERSEVECTOR3;
	m_aInfo[1].color = P2_COLOR;

	// �L�[�R���t�B�O������
	InitKeyConfig();

	//�p�[�e�B�N���e�N�X�`���Ǎ�
	_RNC_Texture *pTexture = &RNLib::Texture();
	for (int nCntTex = 0; nCntTex < (int)PARTI_TEX::MAX; nCntTex++)
	{
		s_ParticleTex[nCntTex] = pTexture->Load(PARTICLE_TEX_PATH[nCntTex]);
	}

	s_SE.pSound = &RNLib::Sound();
	s_SE.jump = s_SE.pSound->Load("data\\SOUND\\SE\\jamp_01.wav");
	s_SE.dog[0] = s_SE.pSound->Load("data\\SOUND\\SE\\heron2.wav");		// ����
	s_SE.dog[1] = s_SE.pSound->Load("data\\SOUND\\SE\\shrink.wav");		// �k��
	s_SE.dog[2] = s_SE.pSound->Load("data\\SOUND\\SE\\extend.wav");		// �L�т�
	s_SE.dog[3] = s_SE.pSound->Load("data\\SOUND\\SE\\vibration.wav");	// �k����
	s_SE.Swaping = s_SE.pSound->Load("data\\SOUND\\SE\\swap_now.wav");
	s_SE.SwapEnd = s_SE.pSound->Load("data\\SOUND\\SE\\swap_end.wav");
	s_SE.expand = s_SE.pSound->Load("data\\SOUND\\SE\\death_expand.wav");
	s_SE.explosion = s_SE.pSound->Load("data\\SOUND\\SE\\death_explosion.wav");

	s_motion[0].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Default.txt");
	s_motion[0].walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Walk.txt");
	s_motion[0].jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Jump.txt");
	s_motion[0].fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Fall.txt");
	s_motion[0].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Landing.txt");
	s_motion[0].dance = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\Dance.txt");
	s_motion[0].getup = RNLib::Motion3D().Load("data\\MOTION\\Player_Mouth\\GetUp.txt");

	s_motion[1].neutral = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Default.txt");
	s_motion[1].walk = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Walk.txt");
	s_motion[1].jump = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Jump.txt");
	s_motion[1].fall = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Fall.txt");
	s_motion[1].landing = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Landing.txt");
	s_motion[1].dance = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\Dance.txt");
	s_motion[1].getup = RNLib::Motion3D().Load("data\\MOTION\\Player_Eye\\GetUp.txt");

	InitInfo();

	// �����l�ݒ�
	// �� ����Ȃ��ƃX�e�[�W����O�Ɉ�u���nSE���Ȃ�
	m_aInfo[0].bJump = m_aInfo[1].bJump = false;

	s_zoomUpCounter = 0;
	s_zoomUpFixedCounter = 0;
	if (Manager::StgEd()->GetPlanetIdx() == 0) {
		if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
			s_zoomUpCounter = ZOOM_UP_TIME;
			s_zoomUpFixedCounter = ZOOM_UP_FIXED_TIME;
		}
	}

	//�J�b�g�C��
	Stage::SetIsCutIn(false);

	//�J�E���g
	nCnt = 60;

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
		Player.JoyPad[(int)KEY_CONFIG::MOVE_LEFT]  = _RNC_Input::BUTTON::LEFT;  // ���ړ�
		Player.JoyPad[(int)KEY_CONFIG::MOVE_RIGHT] = _RNC_Input::BUTTON::RIGHT; // �E�ړ�
		Player.JoyPad[(int)KEY_CONFIG::JUMP]       = _RNC_Input::BUTTON::A;     // �W�����v
		Player.JoyPad[(int)KEY_CONFIG::SWAP]       = _RNC_Input::BUTTON::X;     // �X���b�v
		Player.JoyPad[(int)KEY_CONFIG::DECIDE]     = _RNC_Input::BUTTON::A;     // ����
		Player.JoyPad[(int)KEY_CONFIG::PAUSE]      = _RNC_Input::BUTTON::START; // �|�[�Y
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
		Player.isDeath = false;
		Player.deathCounter = 0;
		Player.deathCounter2 = 0;
		Player.swapWaitCounter = 0;
		Player.nEscapeGoalInterval = 0;
	}

	CGoalGate::ResetEtr();
}

//=====================================================================================================================
// �I������
//=====================================================================================================================
void CPlayer::Uninit(void)
{

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
		UpdateInfo();
		return;
	}

	// ���쏈��
	ActionControl();

	// ���҂Ƃ��ɃS�[�����ĂȂ�������
	if ((!m_aInfo[0].bGoal || !m_aInfo[1].bGoal) &&
		(!m_aInfo[0].bRide || !m_aInfo[1].bRide))
	{
		// �X���b�v
		Swap();
	}
	else if ((m_aInfo[0].bGoal && m_aInfo[1].bGoal) ||
		     (m_aInfo[0].bRide && m_aInfo[1].bRide))
	{
		//�S�[�����o
		GoalDirector();
	}

	// �����蔻��܂Ƃ�
	CollisionToStageObject();

	// �ߋ��̈ʒu��ݒ�
	m_aInfo[0].posOld = m_aInfo[0].pos;
	m_aInfo[1].posOld = m_aInfo[1].pos;

	// ���X�V
	UpdateInfo();
}

//----------------------------
// ���X�V����
//----------------------------
void CPlayer::UpdateInfo(void)
{
	// �K�C�h�e�L�X�g
	const bool isSwapGuide = SwapGuideText();

	int nCntPlayer = -1;
	for each (Info &Player in m_aInfo)
	{
		nCntPlayer++;

		UpdateDeath(Player, nCntPlayer);

		// �ʒu�ݒ�
		if (Player.deathCounter == 0 && Player.deathCounter2 == 0 && !Player.bGoal && !s_bSwapAnim && !Player.bRide) {
			Player.doll->SetPos(Player.pos - Pos3D(0.0f, (fabsf(Player.pos.y) / Player.pos.y) * SIZE_HEIGHT, 0.0f));
			Player.doll->SetRot(Player.rot);
			Player.doll->SetScale(Player.scale);
			Player.doll->SetIsShow(true);

			// ���ڂ̕`��
			if (nCntPlayer == 1) {
				Matrix eyeMtx = RNLib::Matrix().ConvPosRotToMtx(Pos3D(0.0f, 6.0f, 6.85f), Rot3D(0.0f, D3DX_PI, 0.0f));

				static int eyeCounter = 0;
				static int eyeCounter2 = 0;
				static int eyeTime = 0;

				if (eyeCounter2 > 0) {
					eyeCounter2--;
				}
				else if (++eyeCounter >= eyeTime) {
					eyeCounter = 0;
					eyeCounter2 = 4;
					eyeTime = 5 + (rand() % 90);
				}

				RNLib::Polygon3D().Put(PRIORITY_OBJECT, RNLib::Matrix().MultiplyMtx(Player.doll->GetBoneState(0).GetWorldMtx(), eyeMtx))
					->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::CHR_BLACK_EYE], (eyeCounter2 > 0), 2, 1)
					->SetSize(4.0f, 4.0f);
			}
		}
		else {
			Player.doll->SetIsShow(false);
		}

		// �X���b�v�A�j���[�V�������ł���ΐ܂�Ԃ�
		if (s_bSwapAnim) {
			continue;
		}

		if (isSwapGuide && !IsKeyConfigPress(nCntPlayer, Player.side, KEY_CONFIG::SWAP)) {
			if (-156.0f <= Player.pos.x && 156.0f >= Player.pos.x) {
				Pos3D putPos = Player.pos;
				putPos.y += (Player.pos.y / fabsf(Player.pos.y)) * 24.0f;
				CInt count = RNLib::Count().GetCount() % 40;
				float sizeRate = (float)(count - ((count % 20) * (count / 20)) * 2) / 20;
				RNLib::Text3D().Put(PRIORITY_UI, "X", _RNC_Text::ALIGNMENT::CENTER, 0, RNLib::Matrix().ConvPosToMtx(putPos))
					->SetSize(Size2D(16.0f * (0.5f + sizeRate * 0.25f), 16.0f * (0.5f + sizeRate * 0.25f)))
					->SetZTest(false)
					->SetBillboard(true);
			}
		}

		//���P�b�g���~��C���^�[�o������
		if (Player.nEscapeGoalInterval > 0)
			Player.nEscapeGoalInterval--;

		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
		if (Player.bRide || Player.bGoal)
			continue;

		//�X���b�v�K�C�h�\��
		if (!Player.isDeath)
			SwapGuide(Player);

		// �ō��x���W�X�V
		switch (Player.side) {
			case WORLD_SIDE::FACE:	 Player.fMaxHeight = Player.fMaxHeight < Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
			case WORLD_SIDE::BEHIND: Player.fMaxHeight = Player.fMaxHeight > Player.pos.y ? Player.pos.y : Player.fMaxHeight; break;
		}
	}
}

//----------------------------
// ���S�X�V����
//----------------------------
void CPlayer::UpdateDeath(Info& info, CInt& count) {

	// ���S�J�E���^�[&���o
	static Vector3D addVec = INITVECTOR3D;
	static Pos3D posVTemp = INITVECTOR3D;
	static Pos3D posRTemp = INITVECTOR3D;
	if (info.deathCounter > 0) {
		if (--info.deathCounter == 0) {
			info.deathCounter2 = DEATH_TIME2;
			addVec = INITVECTOR3D;
			posVTemp = Manager::GetMainCamera()->GetPosV();
			posRTemp = Manager::GetMainCamera()->GetPosR();

			// �~�X�̃e�L�X�g����
			CMiss::Create();

			// �S�[�X�g�̐���
			CGhost::Create(info.pos, count);
		}

		float rate = (float)info.deathCounter / DEATH_TIME;
		float rateOpp = 1.0f - rate;
		Manager::GetMainCamera()->SetMotionBlurColor(Color(255,(int)(255 * rateOpp), (int)(255 * rateOpp),255));
		Manager::GetMainCamera()->SetMotionBlurPower(0.25f + rate * 0.25f);
		Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.1f));
	}
	// ���S�J�E���^�[2&���o
	if (info.deathCounter2 > 0) {

		if (--info.deathCounter2 == 0) {
			//�v���C���[��񏉊����@�{�@����̃I�u�W�F�N�g�̏�����
			InitInfo();
			CPile::ResetTrunkHeightAll();

			//�I�u�W�F�N�g���擾
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {
				//�擾�����I�u�W�F�N�g���L���X�g
				CStageObject* pObj = (CStageObject*)obj;

				switch (pObj->GetType())
				{//�I�u�W�F�N�g��ϊ����č폜
				case CStageObject::TYPE::MISS:  { CMiss*  Miss  = (CMiss*)obj;  Miss->Delete(); }break;
				case CStageObject::TYPE::GHOST: { CGhost* Ghost = (CGhost*)obj; Ghost->Delete();} break;
				}
			}
		}

		bool isReturn = false;
		int counter = info.deathCounter2;
		if (counter > DEATH_TIME2 / 2)
			counter -= DEATH_TIME2 / 2;
		float rate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, counter, DEATH_TIME2 / 2);
		if (rate > 0.5f) {
			rate = 0.5f + (0.5f - rate);
			isReturn = true;
		}
		rate /= 0.5f;

		float rate2 = ((float)info.deathCounter2 / DEATH_TIME2);
		if (rate2 > 0.5f) {
			rate2 = 0.5f + (0.5f - rate2);
		}
		rate2 /= 0.5f;

		addVec += RNLib::Geometry().GetRandomVec();
		Manager::GetMainCamera()->SetMotionBlurPower(isReturn ? 0.25f + rate * 0.25f : rate * 0.5f);
		Manager::GetMainCamera()->SetMotionBlurScale(1.0f + (rate * 0.02f));
		Manager::GetMainCamera()->SetPosVAndPosR(
			((Manager::GetMainCamera()->GetPosV() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posVTemp * (1.0f - rate2)),
			((Manager::GetMainCamera()->GetPosR() + Vector3D(addVec.x * rate, addVec.y * rate, 0.0f)) * rate2) + (posRTemp * (1.0f - rate2)));
	}

	// �c��݃J�E���^�[�����o
	else if (info.expandCounter > 0) {
		if (--info.expandCounter == 0) {
			RNLib::Sound().Play(s_SE.explosion, _RNC_Sound::CATEGORY::SE, 1.0f, false);
			Manager::GetMainCamera()->SetVib(5.0f);
			RNLib::Input().SetVibration(2.0f, count);
			RNLib::Input().SetVibration(1.0f, !count);

			const CEffect_Death::TYPE type = (CEffect_Death::TYPE)info.DeathType;
			switch (type)
			{
				case CEffect_Death::TYPE::INK:
				{
					CEffect_Death* pEff = Manager::EffectMgr()->DeathParticleCreate(info.pos, INITVECTOR3D, INITVECTOR3D, INITVECTOR3D, CStageObject::SIZE_OF_1_SQUARE, info.color, type);
				}
					break;

				case CEffect_Death::TYPE::BALL:
				{
					CInt NUM_PARTICLE = 8;
					Pos3D rot = INITVECTOR3D;
					for (int ParCnt = 0; ParCnt < NUM_PARTICLE; ParCnt++)
					{
						rot.z = -D3DX_PI + D3DX_PI_DOUBLE * RNLib::Number().GetRandomFloat(1.0f);
						CEffect_Death* pEff = Manager::EffectMgr()->DeathParticleCreate(info.pos, INITVECTOR3D, rot, INITVECTOR3D, 0.0f, info.color, type);

						pEff->SetBallSize(CEffect_Death::BALL_SIZE_LV::SMALL);
					}
				}break;
			}
			info.deathCounter = DEATH_TIME;
		}
		info.scale.x =
		info.scale.y =
		info.scale.z = 1.0f + (1.0f - RNLib::Ease().Easing(EASE_TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 0.2f;
		info.pos.z = -(1.0f - RNLib::Ease().Easing(EASE_TYPE::IN_SINE, info.expandCounter, EXPAND_TIME)) * 8.0f;
	}
}

//----------------------------
// �X���b�v�K�C�h�e�L�X�g
//----------------------------
bool CPlayer::SwapGuideText(void)
{
	//�X���b�v�� or �ŏ��̘f���ł͂Ȃ�
	if (s_bSwapAnim || Manager::StgEd()->GetPlanetIdx() != 0) return false;

	if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath &&
		!m_aInfo[0].bGoal || !m_aInfo[1].bGoal) {

		if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
			if (s_zoomUpCounter == 0) {
				if (++ms_guideCounter > 30)
					ms_guideCounter = 30;
				CFloat rate = (float)ms_guideCounter / 30;
				if (ms_bSwapEnd) {

					if (ms_guideCounter == 1) {
						RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], _RNC_Sound::CATEGORY::SE, 1.0f, false);
					}
					if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
						RNLib::Text3D().Put(PRIORITY_UI, "OK!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
							->SetSize(Size2D(32.0f * rate, 32.0f * rate))
							->SetZTest(false)
							->SetBillboard(true);
					}
				}
				else {
					if (s_nSwapInterval == 0) {
						if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
							RNLib::Text3D().Put(PRIORITY_UI, "SWAP���Ă݂悤!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
								->SetSize(Size2D(24.0f * rate, 24.0f * rate))
								->SetZTest(false)
								->SetBillboard(true);
							return true;
						}
					}
				}
			}
		}
		else if (Manager::StgEd()->GetType()[0].nStageIdx == 3) {
			if (++ms_guideCounter > 30)
				ms_guideCounter = 30;

			//�J�E���^�[���i�ނقǃe�L�X�g���g�傳���邽�߂̃J�E���^�[����
			CFloat rate = (float)ms_guideCounter / 30;
			if (CParts::GetDispNum() == 0) {
				if (ms_guideCounter == 1) 
					RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], _RNC_Sound::CATEGORY::SE, 1.0f, false);

				if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
					RNLib::Text3D().Put(PRIORITY_UI, "�Ƃт̂�!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
						->SetSize(Size2D(32.0f * rate, 32.0f * rate))
						->SetZTest(false)
						->SetBillboard(true);
				}
			}
			else if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
				RNLib::Text3D().Put(PRIORITY_UI, "���P�b�g�̃p�[�c�����߂�!", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, INITMATRIX)
					->SetSize(Size2D(24.0f * rate, 24.0f * rate))
					->SetZTest(false)
					->SetBillboard(true);
			}
		}
	}

	return false;
}

//----------------------------
// ���쏈��
//----------------------------
void CPlayer::ActionControl(void)
{
	// �v���C���[�ԍ�
	int nIdxPlayer = -1;

	bool isZoomUp = false;
	if (s_zoomUpCounter > 0 &&
		Manager::StgEd()->GetPlanetIdx() == 0 &&
		Manager::StgEd()->GetType()[0].nStageIdx == 0) {
		isZoomUp = true;
	}

	// [[[ �J�������� ]]]
	if (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath) 
	{// �ǂ��������ł��Ȃ�
		Pos3D targetPosV = Manager::StgEd()->GetCameraPos();
		Pos3D targetPosR = (m_aInfo[0].pos + m_aInfo[1].pos) * 0.5f;
		targetPosR.x *= 0.25f;
		targetPosR.y = 0.0f;

		if (isZoomUp) 
		{// �Y�[���A�b�v��
			if (s_zoomUpFixedCounter > 0) {
				s_zoomUpFixedCounter--;
				if (s_zoomUpFixedCounter <= ZOOM_UP_FIXED_TIME - 60) {
					if (s_zoomUpFixedCounter == ZOOM_UP_FIXED_TIME - 60) {
						m_aInfo[0].doll->OverwriteMotion(s_motion[0].jump);
						m_aInfo[1].doll->OverwriteMotion(s_motion[1].jump);
						m_aInfo[0].move.y += 2.5f;
						m_aInfo[1].move.y -= 2.5f;
					}
				}
				else {
					m_aInfo[0].doll->OverwriteMotion(s_motion[0].getup);
					m_aInfo[1].doll->OverwriteMotion(s_motion[1].getup);
				}
			}
			else {
				s_zoomUpCounter--;
			}
			
			if (nCnt > 0) {

				nCnt--;
				if (nCnt == 0) {

					CInt NumEffect = 40;
					for (int Cnt = 0; Cnt < NumEffect; Cnt++) {
						CInt Num = rand() % 2;
						const Pos3D TexPos = Pos3D(m_aInfo[Num].pos.x + (float)(rand() % (int)12.0f - 12.0f * 0.5), m_aInfo[Num].pos.y - 8.0f * cosf(m_aInfo[Num].rot.z), m_aInfo[Num].pos.z);
						const Rot3D TexRot = Pos3D(m_aInfo[Num].rot.x, m_aInfo[Num].rot.y, m_aInfo[Num].rot.z + ((rand() % 1570) - 785) * 0.001f);
						CFloat ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.9f) + INIT_EFFECT_SCALE.x * 0.1f);
						Manager::EffectMgr()->ParticleCreate(GetParticleIdx(PARTI_TEX::SMOKE_EFFECT), TexPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 600, TexRot, D3DXVECTOR3(80.0f, 80.0f, 0.0f), false, false, _RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
						Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(TexPos.x, TexPos.y + 1.0f * cosf(TexRot.y), TexPos.z), TexRot, INITSCALE3D * 0.05f, COLOR_WHITE);
					}
				}
			}

			Pos3D basePosRMain = m_aInfo[0].pos + Pos3D(0.0f, -16.0f, 0.0f);
			Pos3D basePosRSub  = m_aInfo[1].pos + Pos3D(0.0f, 16.0f, 0.0f);
			Pos3D basePosVMain = basePosRMain + Pos3D(0.0f, 0.0f, -100.0f);
			Pos3D basePosVSub  = basePosRSub  + Pos3D(0.0f, 0.0f, -100.0f);
			float rate = 1.0f;
			if (s_zoomUpCounter < 60) {
				rate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, s_zoomUpCounter, 60);
			}
			CFloat rateOpp = 1.0f - rate;
			Manager::GetMainCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVMain * rate, targetPosR * rateOpp + basePosRMain * rate);
			Manager::GetSubCamera()->SetPosVAndPosR(targetPosV * rateOpp + basePosVSub * rate, targetPosR * rateOpp + basePosRSub * rate);

			//�v���ĊJ
			if (s_zoomUpCounter == 0)
			{
				CMode_Game::RestartTime();
			}
			return;
		}
		else{
			_RNC_Input *pInput = &RNLib::Input();
			if (RNSystem::GetMode() == RNSystem::MODE::DEBUG) {
				if (pInput->GetKeyTrigger(DIK_B))
				{//���_�ؑ�
					if (!s_bAimPlayer)
					{
						s_bAimPlayer = !s_bAimPlayer;
						s_nAimNo = 0;
					}
					else if (s_nAimNo == 0) {
						s_nAimNo = 1;
					}
					else
					{
						s_bAimPlayer = false;
						s_nAimNo = 0;
					}

					if (s_bAimPlayer) {
						s_addPosV.x = s_addPosR.x = m_aInfo[s_nAimNo].pos.x + s_fCorrWidth;
						s_addPosV.y = s_addPosR.y = m_aInfo[s_nAimNo].pos.y + s_fCorrHeight;
						s_addPosV.z = -100.0f;
					}
				}
			}

			if (s_bAimPlayer) {
				if (RNLib::Input().GetKeyPress(DIK_R)) {
					if (RNLib::Input().GetKeyPress(DIK_T)) { s_addPosR.z += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_G)) { s_addPosR.z -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_H)) { s_addPosR.x += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_F)) { s_addPosR.x -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_U)) { s_addPosR.y += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_J)) { s_addPosR.y -= 1.0f; }
				}
				else {
					if (RNLib::Input().GetKeyPress(DIK_T)) { s_addPosV.z += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_G)) { s_addPosV.z -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_H)) { s_addPosV.x += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_F)) { s_addPosV.x -= 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_U)) { s_addPosV.y += 1.0f; }
					if (RNLib::Input().GetKeyPress(DIK_J)) { s_addPosV.y -= 1.0f; }
				}

				Manager::GetMainCamera()->SetPosVAndPosR(s_addPosV, s_addPosR);

#if 0
				if (pInput->GetKeyTrigger(DIK_V)) s_fCorrWidth -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_N)) s_fCorrWidth += 0.1f;
				if (pInput->GetKeyTrigger(DIK_G)) s_fCorrHeight -= 0.1f;
				if (pInput->GetKeyTrigger(DIK_H)) s_fCorrHeight += 0.1f;
				
				//�{���̓����蔻��͈�
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,255,255,255 })
					->SetSize(SIZE_WIDTH * 2.0f, SIZE_HEIGHT * 2.0f);

				//�����̓����蔻��͈�
				RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_aInfo[0].pos, INITVECTOR3D)
					->SetCol(Color{ 255,100,100,255 })
					->SetSize(SIZE_WIDTH * 1.0f, SIZE_HEIGHT * 1.0f);

				RNLib::Text2D().PutDebugLog(String("���̒�����:%f  �c�̒�����:%f", s_fCorrWidth, s_fCorrHeight));
#endif
			}
			else {
				Manager::GetMainCamera()->SetPosVAndPosR(targetPosV, targetPosR);
			}
		}
	}
	else 
	{// �ǂ��炩������ł���
		return;
	}

	for each (Info & Player in m_aInfo)
	{
		// ���̃v���C���[�ԍ���
		nIdxPlayer++;

		// �������S�[�����Ă��Ȃ���Ώo��
		if (CRocket::GetCounter() < NUM_PLAYER && !m_aInfo[(nIdxPlayer + 1) % NUM_PLAYER].bGoal &&
			(Player.bRide || Player.bGoal) && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
		{
			//���P�b�g�E�o�̃C���^�[�o���ݒ�
			if (Player.bRide)
				Player.nEscapeGoalInterval = CRocket::ESCAPE_INTERVAL;

			//�S�[���h�A�E�o�̃C���^�[�o���ݒ�
			else if (Player.bGoal)
				Player.nEscapeGoalInterval = CGoalGate::ESCAPE_INTERVAL;

			CGoalGate::EntrySub();
			CRocket::RideOff();
			Player.bRide = false;
			Player.bGoal = false;

			if(Player.pGoalGate != NULL)
				Player.pGoalGate->LeaveDoor();
		}

		// ���P�b�g�ɏ���Ă���@or �S�[�����Ă��� or �Y�[���A�b�v or �^�C���I�[�o�[�̎��X�L�b�v
		if (Player.bRide || Player.bGoal || isZoomUp || Stage::GetIsTimeOver()) 
			continue;

		bool isMove = false;

		// �X���b�v����
		if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::SWAP)) {
			Player.nSwapAlpha = 255;
			if (++Player.swapWaitCounter > SWAP_WAIT_BALLOON_TIME)
				Player.swapWaitCounter = SWAP_WAIT_BALLOON_TIME;
			Player.rot.y = D3DX_PI;
		}
		//�X���b�v�����
		else {
			Player.nSwapAlpha = NORMAL_SWAP_ALPHA;
			if (--Player.swapWaitCounter < 0)
				Player.swapWaitCounter = 0;

			// �W�����v���́i�󒆂���Ȃ��j
			if (!Player.bJump && Player.bGround && IsKeyConfigTrigger(nIdxPlayer, Player.side, KEY_CONFIG::JUMP))
			{
				Player.bGround = false;            // �n�ʂ��痣�ꂽ
				Player.move.y = Player.fJumpPower; // �W�����v�ʑ��
				Player.bJump = true;               // �W�����v����
				PlaySE(SE_LABEL::JUMP);            // SE�Đ�
			}

			if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_RIGHT) ||
				RNLib::Input().GetStickAnglePress(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::RIGHT, nIdxPlayer))
			{// �E�Ɉړ�
				Player.move.x += MOVE_SPEED;
				Player.rot.y += RNLib::Geometry().FindAngleDifference(Player.rot.y, D3DX_PI * 0.7f) * 0.5f;
				isMove = true;

			}
			else if (IsKeyConfigPress(nIdxPlayer, Player.side, KEY_CONFIG::MOVE_LEFT) ||
				RNLib::Input().GetStickAnglePress(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::LEFT, nIdxPlayer))
			{// ���Ɉړ�
				Player.move.x -= MOVE_SPEED;
				Player.rot.y += RNLib::Geometry().FindAngleDifference(Player.rot.y, -D3DX_PI * 0.7f) * 0.5f;
				isMove = true;
			}
		}

		if (Player.swapWaitCounter > 0) {
			Player.doll->OverwriteMotion(s_motion[nIdxPlayer].dance);
		}
		else if (!Player.bGround) {
			if (Player.bGroundOld) {
				Player.doll->OverwriteMotion(s_motion[0].jump);
			}
			else if(Player.doll->GetMotionCounter() > 10){
				if ((Player.pos.y > 0.0f && Player.move.y < 0.0f) || (Player.pos.y < 0.0f && Player.move.y > 0.0f)) {
					Player.doll->OverwriteMotion(s_motion[nIdxPlayer].fall);
				}
			}
		}
		else if (Player.landingCounter == 0) {
			if (isMove) {
				Player.doll->OverwriteMotion(s_motion[nIdxPlayer].walk);
			}
			else {
				Player.doll->OverwriteMotion(s_motion[nIdxPlayer].neutral);
			}
		}
		else {
			Player.landingCounter--;
		}

		{// �����o���̕\��
			Pos3D putPos = Player.pos;
			putPos.y += RNLib::Number().GetPlusMinus(Player.pos.y) * 12.0f;
			_RNC_Polygon3D::CRegistInfo* polygon3D = RNLib::Polygon3D().Put(PRIORITY_UI, putPos, Rot3D(0.0f, 0.0f, -0.1f + (RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, RNLib::Number().GetTurnNum(RNLib::Count().GetCount(), 30), 30)) * 0.2f))
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_WAITBUBBLE], Player.pos.y < 0.0f, 2, 1)
				->SetCol(Color(255, 255, 255, 255 * ((float)Player.swapWaitCounter / SWAP_WAIT_BALLOON_TIME)))
				->SetZTest(false);
			
			Size2D size = Size2D(16.0f, 32.0f * ((float)Player.swapWaitCounter / SWAP_WAIT_BALLOON_TIME));
			if (Player.pos.y > 0.0f) {
				polygon3D->SetVtxPos(
					Pos3D(-size.x, size.y, 0.0f),
					Pos3D(size.x, size.y, 0.0f),
					Pos3D(-size.x, 0.0f, 0.0f),
					Pos3D(size.x, 0.0f, 0.0f));
			}
			else {
				polygon3D->SetVtxPos(
					Pos3D(-size.x, 0.0f, 0.0f),
					Pos3D(size.x, 0.0f, 0.0f),
					Pos3D(-size.x, -size.y, 0.0f),
					Pos3D(size.x, -size.y, 0.0f));
			}
		}
	}
}

//#################################################
// �X���b�v����
//#################################################
void CPlayer::Swap(void)
{
	// �ǂ��炩������ł�����I��
	if (m_aInfo[0].isDeath || m_aInfo[1].isDeath)
		return;

	//�C���^�[�o����
	if (s_nSwapInterval != 0)
	{
		s_nSwapInterval--;
		return;
	}

	// ���҂Ƃ��ɃX���b�v�{�^���������Ă���
	if ((IsKeyConfigPress(0, m_aInfo[0].side, KEY_CONFIG::SWAP) && m_aInfo[0].swapWaitCounter > 0) &&
		(IsKeyConfigPress(1, m_aInfo[1].side, KEY_CONFIG::SWAP) && m_aInfo[1].swapWaitCounter > 0))
	{
		//���P�b�g�ɏ���Ă��Ȃ��Ƃ��ɃT�E���h�Đ�
		if (CRocket::GetCounter() != NUM_PLAYER)
		{
			// �X���b�v�A�j���[�V�����ݒ�
			s_AnimState = SWAP_ANIM::PROLOGUE;
			s_nSwapInterval = SWAP_PROLOGUE_INTERVAL;
			s_bSwapAnim = true;
			PlaySE(SE_LABEL::SWAPING);
		}

		for each (Info &Player in m_aInfo)
		{
			// ���P�b�g�ɏ���Ă���X�L�b�v
			if (Player.bRide) continue;

			// �ʒu�E�d�͉����x�E�W�����v�ʁE���݂��鐢�E�𔽓]
			Player.fSwapPosY = Player.pos.y * -1.0f;
			Player.fSwapMoveY = (Player.fSwapPosY - Player.pos.y) / SWAP_MIDDLE_INTERVAL;
			Player.bTramJump = false;

			// �O��ʒu�X�V
			Player.posOld = Player.pos;
			Player.bGroundOld = Player.bGround;

			// �����o���J�E���^�[������
			Player.swapWaitCounter = 0;
		}
	}
}

//*************************************************
// �X���b�v�A�j���[�V����
//*************************************************
void CPlayer::SwapAnimation(void)
{
	//�C���^�[�o������
	s_nSwapInterval--;

	RNLib::Polygon2D().Put(PRIORITY_SWAP_CURTAIN, INITPOS2D, 0.0f)
		->SetSize(10000.0f, 10000.0f)
		->SetCol(Color{ 0,0,0,100 });

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++)
	{
		//�v���C���[���Q�Ɓi�ړ��������p�[�e�B�N���̈ʒu����O�ɏo��
		Info &Player = m_aInfo[nCntPlayer];

		switch (s_AnimState)
		{
			case CPlayer::SWAP_ANIM::PROLOGUE: SwapAnim_Prologue(Player, nCntPlayer); break; //�v�����[�O
			case CPlayer::SWAP_ANIM::MIDDLE:   SwapAnim_Middle(Player, nCntPlayer);	break;	//����
			case CPlayer::SWAP_ANIM::EPILOGUE: SwapAnim_Epilogue(Player, nCntPlayer); break;//�G�s���[�O
		}

		//�S�[�����Ă���G�t�F�N�g��\��
		if (Player.bGoal || Player.bRide) continue;

		Color setCol;
		if (nCntPlayer == 0){
			setCol = Color{ (215 + rand() % 40),(135 + rand() % 40),(39 + rand() % 40),255 };
		}
		else{
			setCol = Color{ (45 + rand() % 40),(130 + rand() % 125),(130 + rand() % 125),255 };
		}

		CInt nTex = rand() % 2 + 2;

		Manager::EffectMgr()->ParticleCreate(GetParticleIdx((PARTI_TEX)nTex), Player.pos + Pos3D(0.0f,0.0f,0.0f), Vector3D(16.0f, 16.0f, 0.0f), setCol, CParticle::TYPE::TYPE_NORMAL, 300,D3DXVECTOR3(0.0f, 0.0f, (float)(rand() % 629 - 314) / 100.0f),D3DXVECTOR3(8.0f,8.0f,0.0f),false,false,_RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
	}
}

//*************************************************
//�v�����[�O����
//*************************************************
void CPlayer::SwapAnim_Prologue(Info& Player, CInt nIdxPlayer)
{
	//���̃C���^�[�o����
	if (s_nSwapInterval > 0 || nIdxPlayer == 0) return;
	s_nSwapInterval = SWAP_MIDDLE_INTERVAL;
	s_AnimState = SWAP_ANIM::MIDDLE;
}

//*************************************************
//���ԏ���
//*************************************************
void CPlayer::SwapAnim_Middle(Info& Player, CInt nIdxPlayer)
{
	//�X���b�v��ֈړ�
	Player.posOld.y = Player.pos.y;
	Player.pos.y += Player.fSwapMoveY;

	//���̃C���^�[�o����
	if (s_nSwapInterval > 0) return;
	Player.posOld.y = Player.pos.y = Player.fSwapPosY;
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
void CPlayer::SwapAnim_Epilogue(Info& Player, CInt nIdxPlayer)
{
	//�Ō�̃v���C���[�̂Ƃ��ɃX���b�v�A�j���[�V�����I��
	if (s_nSwapInterval > 0 || nIdxPlayer == 0) return;
	s_bSwapAnim = false;
	s_nSwapInterval = SWAP_INTERVAL;
	ms_bSwapEnd = (-156.0f <= m_aInfo[0].pos.x && 156.0f >= m_aInfo[0].pos.x && -156.0f <= m_aInfo[1].pos.x && 156.0f >= m_aInfo[1].pos.x);
	PlaySE(SE_LABEL::SWAPEND);

	if (Manager::StgEd()->GetPlanetIdx() == 0)
		if (Manager::StgEd()->GetType()[0].nStageIdx == 0)
			ms_guideCounter = 0;
}

//*************************************************
// �K�C�h�\��
//*************************************************
void CPlayer::SwapGuide(Info& Player)
{
	// �X���b�v��̃}�[�N��`�悷��ʒu
	const Pos3D MarkPos = Pos3D(Player.pos.x, Player.pos.y * -1.0f, -8.0f);
	CFloat MarkSize = 16.0f;

	//�}�[�N�̃e�N�X�`���t�u���W
	CFloat TexULeft = Player.idx * 0.5f;	CFloat TexURight = TexULeft + 0.5f;
	CFloat TexVOver = Player.side == WORLD_SIDE::BEHIND ? 0.0f : 1.0f;
	CFloat TexVUnder = 1.0f - TexVOver;

	//�X���b�v��̃}�[�N�`��
	if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
		RNLib::Polygon3D().Put(PRIORITY_EFFECT, MarkPos, INITD3DXVECTOR3)
			->SetSize(MarkSize, MarkSize)
			->SetBillboard(true)
			->SetZTest(false)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_MARK))
			->SetCol(Color{ 255, 255, 255, (int)Player.nSwapAlpha })
			->SetLighting(false)
			->SetTex(GetParticleIdx(PARTI_TEX::CHARACTER),
				Pos2D(TexULeft, TexVOver), Pos2D(TexURight, TexVOver),
				Pos2D(TexULeft, TexVUnder), Pos2D(TexURight, TexVUnder));
	}

	//�X���b�v��܂ł̒��S���W
	const Pos3D Center = Pos3D(Player.pos.x, 0.0f, MarkPos.z);
	CFloat BottomPosV = Player.fGuideTexVPos + Player.fGuideTexVSize;

	//�K�C�h�T�C�Y��ݒ�
	CInt YDiff = -Player.pos.y * 200;
	CFloat fSize = (YDiff / (int)GUIDE_HEIGHT) / 100.0f;
	Player.fGuideTexVSize = fabsf(fSize);

	//�K�C�h�̃X�s�[�h��ݒ�
	Player.fGuideMoveSpeed = fSize / 100.0f;
	RNLib::Number().Clamp(&Player.fGuideMoveSpeed, -MAX_GUIDE_SPEED, MAX_GUIDE_SPEED);//OK!
	Player.fGuideTexVPos += Player.fGuideMoveSpeed;

	if (Player.fGuideTexVPos >= Player.fGuideTexVSize)
		Player.fGuideTexVPos = 0.0f;

	//�X���b�v�K�C�h�̕`��
	if (RNLib::DrawMgr().GetIsDrawPolygon2D(false)) {
		RNLib::Polygon3D().Put(PRIORITY_EFFECT, Center, INITD3DXVECTOR3)
			->SetSize(GUIDE_WIDTH, (fabsf(Player.pos.y) - SIZE_HEIGHT) * 2.0f)
			->SetBillboard(true)
			->SetZTest(false)
			->SetCol(Color{ Player.color.r,Player.color.g,Player.color.b, (UShort)Player.nSwapAlpha })
			->SetLighting(false)
			->SetTex(GetParticleIdx(PARTI_TEX::SWAP_GUIDE),
				Pos2D(0.0f, Player.fGuideTexVPos), Pos2D(1.0f, Player.fGuideTexVPos),
				Pos2D(0.0f, BottomPosV), Pos2D(1.0f, BottomPosV));
	}
}

//----------------------------
// ���S����
//----------------------------
void CPlayer::Death(Info& Player)
{
	if (Player.isDeath)
		return;

	Player.DeathType = (int)CEffect_Death::TYPE::INK;

	Player.isDeath = true;
	Player.expandCounter = EXPAND_TIME;
	Player.swapWaitCounter = 0;
	RNLib::Sound().Play(s_SE.expand, _RNC_Sound::CATEGORY::SE, 1.0f, false);
}

//----------------------------
// �ړ�����
//----------------------------
void CPlayer::Move(VECTOL vec, int cntPlayer)
{
	if (m_aInfo[cntPlayer].isDeath) {
		return;
	}

	// �v���C���[�̈ʒu�X�V
	Info& Player = m_aInfo[cntPlayer];

	// ���P�b�g�ɏ���Ă���@or �S�[�����Ă�����X�L�b�v
	if (Player.bRide || Player.bGoal) return;

	// �ړ��ʔ��f
	switch (vec)
	{
	case VECTOL::X:

		// ��������
		Player.move.x += (0.0f - Player.move.x) * 0.12f;

		// �]�̈ړ��ʂ��C��
		RNLib::Number().Clamp(&Player.move.x, -MAX_MOVE_SPEED, MAX_MOVE_SPEED);//OK!
		
		// �ʒu�X�V
		Player.pos.x += Player.move.x;

		RNLib::Number().Clamp(&Player.pos.x, -360.0f, 360.0f);
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
}

//========================================
// �����蔻�菈��
// Author:HIRASAWA SHION
// Arrange:RIKU NISHIMURA  OTONO KEISUKE
//========================================
void CPlayer::CollisionToStageObject(void)
{
	// ��U���v���C���[�Ƃ��ɃW�����v�s��
	m_aInfo[0].bGroundOld = m_aInfo[0].bGround;
	m_aInfo[1].bGroundOld = m_aInfo[1].bGround;
	m_aInfo[0].bGround = m_aInfo[1].bGround = false;

	for (int nCntVec = 0; nCntVec < (int)VECTOL::MAX; nCntVec++) {

		//���S����
		bool aDeath[2];
		aDeath[0] = false;
		aDeath[1] = false;

		// �Փ˃x�N�g���ɕϊ�
		const VECTOL vec = (VECTOL)nCntVec;

		// �v���C���[���ɏ������Ă���
		int nCntPlayer = NONEDATA;
		for each (Info & Player in m_aInfo) {
			nCntPlayer++;

			// �ړ��E�ʒu���䏈��
			Move(vec, nCntPlayer);

			// ���P�b�g�ɏ���Ă邩�S�[�����Ă�����܂�Ԃ�
			if (Player.bRide || Player.bGoal)
				continue;

			//�����̏��
			CCollision::SelfInfo Self;
			Self.pos = Player.pos;
			Self.posOld = Player.posOld;
			Self.move = Player.move;
			Self.fWidth = Self.fHeight = SIZE_HEIGHT;

			// �I�u�W�F�N�g1��1�����Ă���
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {

				// �擾�����I�u�W�F�N�g���L���X�g
				CStageObject* pObj = (CStageObject*)obj;

				// ��ގ擾
				const OBJECT_TYPE type = pObj->GetType();

				// �I�u�W�F�N�g�̓����蔻�����ݒ�
				CCollision::ColliInfo colliInfo;
				colliInfo.pos = pObj->GetPos();
				colliInfo.posOld = colliInfo.pos;
				colliInfo.fWidth = pObj->GetWidth() * 0.5f;
				colliInfo.fHeight = pObj->GetHeight() * 0.5f;
				colliInfo.vec = nCntVec;

				// �v���C���[�̋߂��ɃI�u�W�F�N�g�����邩����
				// ������I�u�W�F�N�g������
				if (type != OBJECT_TYPE::TRAMPOLINE && type != OBJECT_TYPE::LASER &&
					type != OBJECT_TYPE::EXTEND_DOG && type != OBJECT_TYPE::PILE && 
					type != OBJECT_TYPE::ROCKET && CCollision::IsInRange(Self, colliInfo, true, true, false, false, s_nSwapInterval))
					continue;

				//�Ǝ��̓����蔻��ݒ�
				//�ꍇ�ɂ���Ă͂����Ŕ���I��
				if (!UniqueColliOpption(pObj, type, Player, &colliInfo.pos, &colliInfo.posOld, &colliInfo.fWidth, &colliInfo.fHeight)) continue;

				// ���������������i�[
				colliInfo.Rot = CCollision::IsBoxToBoxCollider(Self, colliInfo, vec);
				CInt nColliRot = (int)colliInfo.Rot;

				// �������Ă��Ȃ�
				if (colliInfo.Rot == CCollision::ROT::NONE)
				{
					//�Y�Ȃ����Ă��Ȃ�
					if (type == OBJECT_TYPE::PILE)
						Player.bLandPile = false;
					continue;
				}

				// ���S���Ă��邩�ۑ�
				const bool deathOld = aDeath[nCntPlayer];

				// ��ނ��ƂɊ֐�����
				switch (type)
				{
				case OBJECT_TYPE::BLOCK:     CCollision::Block(&Self, &colliInfo, Player, (CBlock*)pObj, &Player.side, &aDeath[nCntPlayer]);break;
				case OBJECT_TYPE::TRAMPOLINE:CCollision::Trampoline(&Self, &colliInfo, (CTrampoline*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::FILLBLOCK: CCollision::FillBlock(&Self, colliInfo.Rot, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::LAVA_BLOCK:aDeath[nCntPlayer] = true;	break;
				case OBJECT_TYPE::SPIKE:     CCollision::Spike(&Self, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::MOVE_BLOCK:CCollision::MoveBlock(&Self, (CMoveBlock*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::METEOR:    CCollision::Meteor(&Self, &colliInfo, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::LASER:     CCollision::Laser(&Self, (CRoadTripLaser*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::EXTEND_DOG:CCollision::Dog(&Self, (CExtenddog*)pObj, &colliInfo, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::GOALGATE:  CCollision::GoalGate(&Self, &colliInfo, obj, &Player.side, &aDeath[nCntPlayer]);	break;
				case OBJECT_TYPE::PARTS:     CCollision::Parts(&Self, (CParts*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::ROCKET:    CCollision::Rocket(&Self, (CRocket*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				case OBJECT_TYPE::PILE:      CCollision::Pile(&Self, &colliInfo, (CPile*)pObj, &Player.side, &aDeath[nCntPlayer]); break;
				}		

				//�����
				if (vec == VECTOL::X) {
					Player.pos.x = Self.pos.x;
					Player.move.x = Self.move.x;
				}
				else {
					Player.pos.y = Self.pos.y;
					Player.move.y = Self.move.y;
				}

				//�����蔻��̎��㏈��
				CollisionAfter(pObj, type, &nColliRot, Player);
			}
		}

		// ���S����ON
		if ((aDeath[0] || aDeath[1]) && (!m_aInfo[0].isDeath && !m_aInfo[1].isDeath)) {
			if (aDeath[0])
				Death(m_aInfo[0]);

			if (aDeath[1])
				Death(m_aInfo[1]);
		}
	}
}

//----------------------------
//�Ǝ��̃I�u�W�F�N�g�̓����蔻��ݒ�
//----------------------------
bool CPlayer::UniqueColliOpption(CStageObject *pObj, const OBJECT_TYPE type, Info &Player, Pos3D *pPos, Pos3D *pPosOld, float *pWidth, float *pHeight)
{
	switch (type) {
		// �u���b�N
		case OBJECT_TYPE::BLOCK: {
			CBlock* pBlock = (CBlock*)pObj;

			if (!pBlock->GetCollision())
				return false;

		}break;

			// �S�[���Q�[�g
		case OBJECT_TYPE::GOALGATE: {
			CGoalGate* pGoalGate = (CGoalGate*)pObj;
			if (pGoalGate->GetStartGate())
				//�����蔻��I��
				return false;
		}break;

			// �ړ���
		case OBJECT_TYPE::MOVE_BLOCK: {
			if (pPosOld != NULL)
			{
				CMoveBlock* pBlock = (CMoveBlock*)pObj;
				*pPosOld = pBlock->GetPosOld();
			}
		}break;

			// 覐�
		case OBJECT_TYPE::METEOR: {
			if (pPosOld != NULL)
			{
				CMeteor* pBlock = (CMeteor*)pObj;
				*pPosOld = pBlock->GetPosOld();
			}
		}break;

			// ���[�U�[
		case OBJECT_TYPE::LASER: {
			CRoadTripLaser* pLaser = (CRoadTripLaser*)pObj;

			if (pPos    != NULL)*pPos    = pLaser->GetLaserPos();
			if (pPosOld != NULL)*pPosOld = pLaser->GetPosOld();
			if (pWidth  != NULL)*pWidth  = pLaser->GetLaserSize().x * 0.5f;
			if (pHeight != NULL)*pHeight = pLaser->GetLaserSize().y * 0.5f;
		}break;

			// �k�C
		case OBJECT_TYPE::EXTEND_DOG: {
			CExtenddog *pDog = (CExtenddog *)pObj;
			if (pPos    != NULL)*pPos    = pDog->GetBodyPos();
			if (pPosOld != NULL)*pPosOld = pDog->GetBodyPosOld();
			if (pHeight != NULL)*pHeight = pDog->GetColliHeight();
		}break;

			//�Y
		case OBJECT_TYPE::PILE:
		{
			if (pPos != NULL && pPosOld != NULL && pHeight != NULL)
			{
				CPile* pPile = (CPile*)pObj;
				pPile->GetCollisionInfo(*pPos, *pPosOld, *pHeight);

				if (D3DXVec3Length(&(*pPos - Player.pos)) >
					D3DXVec2Length(&D3DXVECTOR2(*pWidth + SIZE_WIDTH, *pHeight + SIZE_HEIGHT)))
					//�����蔻��I��
					return false;
			}
		}break;
	}

	//�����蔻�葱�s
	return true;
}

//----------------------------
// �e�v���C���[�̓����蔻�肪�I�������̏���
//----------------------------
void CPlayer::CollisionAfter(CStageObject *pStageObj, const CStageObject::TYPE type, CInt *pColliRot, Info& info)
{
	// ��ނ��ƂɊ֐�����
	switch (type)
	{
		//�u���b�N�̃��A�N�V�����t���O
		case OBJECT_TYPE::BLOCK:
		{
			typedef CCollision::ROT ColRot;		 //�Փ˕����̕ʖ�
			Info *pInfo = &m_aInfo[0];			 //�v���C���[���̃|�C���^
			CBlock *pBlock = (CBlock *)pStageObj;//�u���b�N�փL���X�g

			for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++, pInfo++)
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
		case OBJECT_TYPE::EXTEND_DOG:
		{
			//�k�C�ɕϊ�
			CExtenddog *pDog = (CExtenddog *)pStageObj;

			//���K�̕����Ɠ�����������������
			if (pDog->GetHipRot() == *pColliRot) {
				pDog->SetState(CExtenddog::STATE::RETURN);
				
				if (!info.bGroundOld) {
					RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::DOG_00], _RNC_Sound::CATEGORY::SE, 1.0f, false);
				}
			}
			break;
		}

		//�Y�ɏ���Ă���v���C���[
		case OBJECT_TYPE::PILE:
		{
			//�Y�̔�����擾
			CPile *pPile = (CPile *)pStageObj;
			Pos3D Pos, null;float height;
			pPile->GetCollisionInfo(Pos, null, height);

			for each (Info &Player in m_aInfo)
			{
				if(!Player.bLandPile) continue;

				switch (Player.side)
				{
					case WORLD_SIDE::FACE:
					{
						CFloat LandPos = Pos.y + height + SIZE_HEIGHT;
						if (Player.pos.y < LandPos) Player.pos.y = LandPos;
					}break;

					case WORLD_SIDE::BEHIND:
					{
						CFloat LandPos = Pos.y - height - SIZE_HEIGHT;
						if (Player.pos.y > LandPos) Player.pos.y = LandPos;
					}break;
				}

				Player.move.y = 0.0f;

				//���̍Y�Ŕ��肵�Ȃ��悤������
				Player.bLandPile = false;
			}
			break;
		}

		//�S�[���Q�[�g
		case OBJECT_TYPE::GOALGATE:
		{
			if (m_aInfo[0].bGoal && m_aInfo[1].bGoal)
			{
				//�v���I��
				CMode_Game::SetMeasureTime(false);
				s_nGoalInterval = -GOAL_INTERVAL;
			}
		}

		//���P�b�g
		case OBJECT_TYPE::ROCKET:
		{
			if (m_aInfo[0].bRide && m_aInfo[1].bRide)
			{
				//�v���I��
				CMode_Game::SetMeasureTime(false);
				s_nGoalInterval = -GOAL_INTERVAL;
			}
		}
	}
}

//----------------------------
// �v���C���[���ݒ�
// �w�肳�ꂽ�ԍ��̃v���C���[����ݒ肵�܂��B
//----------------------------
void CPlayer::SetInfo(const Info info, CInt nNum)
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
	CFloat diff = -fMaxHeight - pInfo->pos.y;
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
bool CPlayer::IsKeyConfigTrigger(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetTrigger(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// �ǂ��炩�̃v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigTrigger(KEY_CONFIG KeyConfig)
{
	return 
		RNLib::Input().GetTrigger(m_aInfo[0].Keyborad[(int)m_aInfo[0].side][(int)KeyConfig], m_aInfo[0].JoyPad[(int)KeyConfig], 0) ||
		RNLib::Input().GetTrigger(m_aInfo[1].Keyborad[(int)m_aInfo[1].side][(int)KeyConfig], m_aInfo[1].JoyPad[(int)KeyConfig], 1);
}

//----------------------------
// �v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigPress(CInt nIdx, const WORLD_SIDE side, KEY_CONFIG KeyConfig)
{
	return RNLib::Input().GetPress(m_aInfo[nIdx].Keyborad[(int)side][(int)KeyConfig], m_aInfo[nIdx].JoyPad[(int)KeyConfig], nIdx);
}

//----------------------------
// �ǂ��炩�̃v���C���[���w�肳�ꂽ�L�[�R���t�B�O���g���Ă��邩
//----------------------------
bool CPlayer::IsKeyConfigPress(KEY_CONFIG KeyConfig)
{
	return
		RNLib::Input().GetPress(m_aInfo[0].Keyborad[(int)m_aInfo[0].side][(int)KeyConfig], m_aInfo[0].JoyPad[(int)KeyConfig], 0) ||
		RNLib::Input().GetPress(m_aInfo[1].Keyborad[(int)m_aInfo[1].side][(int)KeyConfig], m_aInfo[1].JoyPad[(int)KeyConfig], 1);
}

//----------------------------
//�v���C���[�pSE�Đ�
//----------------------------
void CPlayer::PlaySE(SE_LABEL label)
{
	switch (label)
	{
		case CPlayer::SE_LABEL::JUMP:   s_SE.pSound->Play(s_SE.jump,    _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_00: s_SE.pSound->Play(s_SE.dog[0],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_01: s_SE.pSound->Play(s_SE.dog[1],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_02: s_SE.pSound->Play(s_SE.dog[2],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::DOG_03: s_SE.pSound->Play(s_SE.dog[3],  _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPING:s_SE.pSound->Play(s_SE.Swaping, _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
		case CPlayer::SE_LABEL::SWAPEND:s_SE.pSound->Play(s_SE.SwapEnd, _RNC_Sound::CATEGORY::SE, 1.0f, false); break;
	}
}

//----------------------------
//�S�[����̉��o
//----------------------------
void CPlayer::GoalDirector(void)
{
	//���ԉ��Z
	s_nGoalInterval++;

	//�N���A�^�C���擾
	static float BestTime;
	CFloat ClearTime = CMode_Game::GetPlayTime();
	CStageEditor *pEd = Manager::StgEd();
	CInt planet = pEd->GetPlanetIdx();
	CInt stage = pEd->GetType()[planet].nStageIdx;

	if (IsKeyConfigTrigger(KEY_CONFIG::DECIDE))
	{
		//���̉��o���Ԃɐݒ�
		if (s_nGoalInterval < POP_CLEARTIME)
		{
			s_nGoalInterval = POP_CLEARTIME;
			BestTime = Stage::GetBestTime(planet, stage);
		}
		else if (s_nGoalInterval < GOAL_INTERVAL)
			s_nGoalInterval = GOAL_INTERVAL;

		//���̃X�e�[�W��
		else
		{
			CCoin::AddNumAll();
			pEd->SwapStage(stage + 1);
			Stage::RegistTime(planet, stage, ClearTime);
			Stage::SetIsCutIn(false);
		}
	}
	
	if (s_nGoalInterval < 0) return;

	Stage::SetIsCutIn(true);
	Stage::SetIsGoal(true);

	const Pos2D Center = RNLib::Window().GetCenterPos();
	const Pos2D Size = RNLib::Window().GetSize();

	//��ʂ��Â�����
	RNLib::Polygon2D().Put(PRIORITY_UI, Center, 0.0f)
		->SetSize(Size.x, Size.y)
		->SetCol(Color{ 0,0,0,150 });

	//�x�X�g�^�C���擾
	if(s_nGoalInterval < POP_CLEARTIME) 
		BestTime = Stage::GetBestTime(planet, stage);

	//�N���A�^�C���\��
	if (s_nGoalInterval >= POP_CLEARTIME)
	{
		if(ClearTime < BestTime)
			RNLib::Text2D().Put(PRIORITY_UI, String("New Record!!"), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(100.0f, 130.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f))
			->SetCol(Color{ 255,255,0,255 });

		RNLib::Text2D().Put(PRIORITY_UI, String("�x�X�g�^�C��:%.1f�b", BestTime), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(100.0f, 160.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f));

		const Pos2D PopPos = Center + Pos2D(0.0f, 200.0f);
		RNLib::Text2D().Put(PRIORITY_UI, String("�N���A�^�C��:%.1f�b", ClearTime), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Center + Pos2D(0.0f, 200.0f), 0.0f)
			->SetSize(Size2D(50.0f, 50.0f));
	}

	// ���̃X�e�[�W��
	if (s_nGoalInterval >= GOAL_INTERVAL)
	{
		//�S�[���C���^�[�o������̒��ߎ���
		CInt DiffInterval = s_nGoalInterval - GOAL_INTERVAL;

		//�C���^�[�o���O���͕\��
		if (DiffInterval < GOAL_INTERVAL / 2)
			RNLib::Text2D().Put(PRIORITY_UI, "NextStage: A �{�^�� or Enter", _RNC_Text::ALIGNMENT::CENTER, NONEDATA, Pos2D(Center.x + 100.0f, 600.0f), 0.0f)
			->SetSize(Size2D(20.0f, 20.0f));

		if (s_nGoalInterval >= GOAL_INTERVAL * 2)
			s_nGoalInterval = GOAL_INTERVAL;
	}

	RNLib::Text2D().PutDebugLog(String("�S�[���C���^�[�o��:%d", s_nGoalInterval));
}