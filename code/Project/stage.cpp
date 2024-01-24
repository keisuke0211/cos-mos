//========================================
// 
// �X�e�[�W�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "stage.h"
#include "Object/Block/block.h"
#include "manager.h"
#include "UI/coinUI.h"
#include "Sound/stage-sound-player.h"
#include "UI/partsUI.h"
#include "resource.h"
#include "BackGround/fishes.h"

#define  MAX_COUNT		 (2000)
#define  MAX_CLOUD		 (5)
#define  MAX_BUBBLE		 (7)
#define  MAX_BUBBLECNT	 (640)

//****************************************
// �������
//****************************************
namespace {
	//========== [[[ �֐��錾 ]]]
	void PutBackGround(void);
	void ClearWorldData(void);
	void AllocWorldData(void);
	void LoadWorldData(void);
	void SaveWorldData(void);
	void RefreshData(void); //���[���h/�X�e�[�W����h��ւ�
	int LoadInt(char *pString, const char *pPunc) { return atoi(strtok(pString, pPunc)); }
	float LoadFloat(char *pString, const char *pPunc) { return (float)atof(strtok(pString, pPunc)); }

	//========== [[[ �ϐ��錾 ]]]
	int             planetIdx;
	int             stageIdx;
	Pos3D			cloudpos[MAX_CLOUD];
	Pos3D			bubblepos[MAX_BUBBLE];
	Pos3D			fishpos;
	float			cloudmove[MAX_CLOUD];
	Pos3D			bubblemove[MAX_BUBBLE];
	int				cloudtex[MAX_CLOUD];
	int				bubbleCnt;
	CPlayer*        player;
	CCoinUI*        coinUI;
	CRocketPartsUI* rocketparts;
	bool            isPause;
	bool            isReset;
	bool            isCutIn;
	bool            isGoal;
	short           wallModelIdxes[2];
	CCamera*        UICamera[2];
	CDoll3D*        UIDoll[2];
	int             limitTimeCounter;

	//�e�f�����Ƃ̃f�[�^
	struct WorldData
	{
		int MaxStage;      //�X�e�[�W��
		float *pBestTime;  //�e�X�e�[�W�̃x�X�g�^�C��
		Stage::Data *pStgRec;//�e�X�e�[�W���Ƃ̃f�[�^
		Stage::Data *pStart; //�X�e�[�W�J�n���̏��i��蒼�����̃R�C������󋵃��Z�b�g�ȂǂɎd�l
	};
	WorldData *pWldData; //�f�����Ƃ̃��R�[�h
	int MaxPlanet;      //�ő�f����

	const char *STAGE_DATA = "data\\GAMEDATA\\STAGE\\STAGE_DATA.txt";

					 //�����Y��
	const char COMMENT = '#';       //�R�����g����
	const char CHR_END = '\0';      //�I�[����
	const char CHR_TAB = '\t';      //�^�u����
	const char *CHR_PAUSE = " -=\n"; //�ǂݎ��Ȃ���������
	const char *SET_RECORD = "SET_RECORD";
	const char *END_RECORD = "END_RECORD";
	const char *CODE_RECORD = "RECORD";
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �X�e�[�W�̊֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ݒ�n�֐�
//========================================
void Stage::SetStageNumber  (const int& setPlanetIdx, const int& setStageIdx) { planetIdx   = setPlanetIdx; stageIdx = setStageIdx; }
void Stage::SetPause        (const bool& setIsPause)                          { isPause     = setIsPause; }
bool Stage::GetPause        (void)                                            { return isPause; }
void Stage::SetRocketPartsUI(CRocketPartsUI* parts)                           { rocketparts = parts; }
void Stage::SetIsCutIn      (const bool& setIsCutIn)                          { isCutIn     = setIsCutIn; }
void Stage::SetIsReset      (const bool& setIsReset)                          { isReset     = setIsReset; }
void Stage::SetIsGoal       (const bool& setIsGoal)                           { isGoal      = setIsGoal; }
bool Stage::GetIsGoal       (void)                                            { return isGoal; }
bool Stage::GetIsTimeOver   (void)                                            { return (limitTimeCounter <= 0); }
bool Stage::GetIsShowUI     (void)                                            { return !isPause && !isCutIn && CPlayer::GetZoomUpCounter() <= 0 && !isReset; }

//========================================
// �擾�n�֐�
//========================================
CPlayer* Stage::GetPlayer(void) { return player; }

//========================================
// ����������
//========================================
void Stage::Init(void) {

	planetIdx = 0;
	stageIdx = 0;
	bubbleCnt = 0;
	player = NULL;
	coinUI = NULL;
	rocketparts = NULL;
	isPause = false;
	isReset = false;
	isGoal = false;
	fishpos = Pos3D(100.0f,300.0f,0.0f);

	for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
	{
		cloudpos[nCnt] = Pos3D(-400.0f + rand() % 400,100.0f,50.0f + rand() % 50);
		cloudmove[nCnt] = (rand() % 20 + 10) * 0.01f;
		cloudtex[nCnt] = (int)CResources::TEXTURE::BG_CLOUD_A + rand() % 3;
	}
	Pos3D pos = Pos3D(100.0f + (rand() % 500 - 250), 200.0f, 200.0f);
	for (int nCnt = 0; nCnt < MAX_BUBBLE; nCnt++)
	{
		bubblepos[nCnt] = pos;
		bubblemove[nCnt] = Pos3D((rand() % 40 + 20), 100.0f, 0.0f);
	}
	// �u���b�N�̓ǂݍ��ݏ���
	CBlock::Load();

	// �����v���C���[�̏���������
	StageSoundPlayer::Init();

	MaxPlanet = 0;
	ClearWorldData();
	LoadWorldData();
}

//========================================
// �I������
//========================================
void Stage::Uninit(void)
{
	// �����v���C���[�̏I������
	StageSoundPlayer::Uninit();

	//���R�[�h�L�^
	SaveWorldData();

	//�������J��
	ClearWorldData();
}

//========================================
// �f���ԍ��𒲂ׂ�
//========================================
bool Stage::CheckPlanetIdx(const int& planetIdx) {

	return Manager::StgEd()->GetPlanetIdx() == planetIdx;
}

//========================================
// �X�e�[�W�ԍ��m�F����
//========================================
bool Stage::CheckStageNumber(const int& planetIdx, const int& stageIdx) {

	if (Manager::StgEd()->GetPlanetIdx() == planetIdx) {
		return Manager::StgEd()->GetType()[planetIdx].nStageIdx == stageIdx;
	}

	return false;
}

//========================================
// �X�e�[�W�J�n�����i����蒼�����܂ށj
//========================================
void Stage::StartStage(void) {

	// �S�[���t���O��FALSE��
	isGoal = false;

	// ���Z�b�g�������t���O
	isReset;

	// �X�e�[�W����
	Manager::StgEd()->StageLoad(planetIdx, stageIdx);

	// �������Ԃ�ݒ�
	limitTimeCounter = Manager::StgEd()->GetStageLimittime(planetIdx, stageIdx) * 60;

	// �v���C���[�̐���
	if (player == NULL)
		player = CPlayer::Create();

	// �R�C��UI�̐���
	if (coinUI == NULL)
		coinUI = CCoinUI::Create(D3DXVECTOR3(100.0f, 25.0f, 0.0f) * 2.0f,Scale2D(40.0f,40.0f),true);

	{// [[[ �J���� ]]]
		// �J�����̎��_/�����_��ݒ�
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		Manager::GetSubCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// �ǃ��f���ǂݍ���
	wallModelIdxes[0] = RNLib::Model().Load("data\\MODEL\\Wall_Left.x");
	wallModelIdxes[1] = RNLib::Model().Load("data\\MODEL\\Wall_Right.x");

	// �����v���C���[�̊J�n����
	StageSoundPlayer::Start();

	if (CheckPlanetIdx(1)) {

		// ���X�V����
		Fishes::Start();
	}

	for (int cnt = 0; cnt < 2; cnt++) {
		{// [[[ UI�p�J�����̐��� ]]]
			UICamera[cnt] = new CCamera(Size2D(200.0f, RNLib::Window().GetHeight()));

			// �N���b�s���O���I��
			UICamera[cnt]->SetIsClipping(true);

			// �w�i�F�ݒ�
			UICamera[cnt]->SetOverwriteBGCol(Color(0, 0, 0, 100));

			// ���C�g��ݒ�
			UICamera[cnt]->SetLightID(Manager::GetLightIdx(0));
		}

		{// [[[ UI�p�h�[���𐶐� ]]]
			UIDoll[cnt] = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load(cnt == 0 ? "data\\SETUP\\Player_Mouth.txt" : "data\\SETUP\\Player_Eye.txt"), TRUE);

			// ���[�V�����ݒ�
			UIDoll[cnt]->SetMotion(RNLib::Motion3D().Load(cnt == 0 ? "data\\MOTION\\Player_Mouth\\Happy.txt" : "data\\MOTION\\Player_Eye\\Walk.txt"));

			// �ʒu/�����ݒ�
			UIDoll[cnt]->SetPos(Pos3D(0.0f, -8.0f, 100.0f));
			UIDoll[cnt]->SetRot(Rot3D(0.0f, D3DX_PI * (cnt == 0 ? 0.9f : 1.1f), 0.0f));

			// �N���b�s���O�ݒ�
			UIDoll[cnt]->SetClippingCamera(UICamera[cnt]->GetID());
		}
	}

	// �J�����̃��C�g
	Manager::GetMainCamera()->SetLightID(Manager::GetLightIdx(Manager::StgEd()->GetPlanetIdx() + 1));
	Manager::GetSubCamera()->SetLightID(Manager::GetLightIdx(Manager::StgEd()->GetPlanetIdx() + 1));

	// ���Z�b�g�t���O��FALSE�ɂ���
	isReset = false;
}

//========================================
// �X�e�[�W�X�V����
//========================================
void Stage::UpdateStage(void) {

	// �����v���C���[�̍X�V����
	StageSoundPlayer::Update();

	static bool isLimit = true;
	if (RNLib::Input().GetKeyTrigger(DIK_F8) && RNSystem::GetMode() == RNSystem::MODE::DEBUG)
		isLimit = !isLimit;

	// �������Ԃ̃J�E���g
	if(GetIsShowUI() && isLimit) {
		if (--limitTimeCounter <= 0) {

			// �^�C���I�[�o�[�̏���
			if (limitTimeCounter == 0) {
				RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::TIME_OVER], _RNC_Sound::CATEGORY::SE, 0.2f, false);
			}

			if (limitTimeCounter == -300) {
				Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::NUI);
			}
		}
		else if (limitTimeCounter % 60 == 0 && limitTimeCounter / 60 > 0 && limitTimeCounter / 60 <= 10) {
			RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::TIME_COUNT], _RNC_Sound::CATEGORY::SE, 0.2f, false);
		}
	}

	// ���ԕ\��
	if (GetIsShowUI()) {
		RNLib::Polygon2D().Put(PRIORITY_UI, Pos2D(140.0f, 160.0f), 0.0f)
			->SetSize(200.0f, 60.0f)
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_FRAME]);
		{
			float rate = (limitTimeCounter % 60) / 60.0f;
			float scale = 1.0f + rate * 0.2f;

			_RNC_Text2D::CRegistInfo* registInfo = RNLib::Text2D().Put(PRIORITY_UI, String("%d", limitTimeCounter / 60 < 0 ? 0: limitTimeCounter / 60), _RNC_Text::ALIGNMENT::CENTER, 0, Pos2D(140.0f, 160.0f), 0.0f)
				->SetSize(limitTimeCounter > 0 ? Size2D(28.0f * scale, 28.0f * scale) : Size2D(28.0f, 28.0f));

			if (limitTimeCounter / 60 <= 10) {
				registInfo->SetCol(Color(255, 255 * (1.0f - rate), 255 * (1.0f - rate), 255));
			}
		}
	}

	// �E�B���h�E�����擾
	const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
	const float windowWidth       = RNLib::Window().GetWidth();
	const float windowHeight      = RNLib::Window().GetHeight();
	const float windowHeightHalf  = windowHeight * 0.5f;
	const float windowHeightHalf2 = windowHeightHalf * 0.5f;

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {

			// ���J�����`��
			RNLib::Polygon2D().Put(0, windowCenterPos + Pos2D(0.0f, windowHeightHalf2), 0.0f, true)
				->SetTex(Manager::GetSubCamera(), Pos2D(0.0f, 0.5f), Pos2D(1.0f, 0.5f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
				->SetSize(windowWidth, windowHeightHalf)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
		}
	}

	{
		static int counter = 0;
		if (isCutIn) {
			if (++counter > 30)
				counter = 30;
		}
		else {
			counter = 0;
		}
		const float rate = RNLib::Ease().Easing(EASE_TYPE::OUT_SINE, counter, 30);

		// UI�J�����`��
		RNLib::Polygon2D().Put(0, Pos2D(-100.0f, windowHeightHalf) + Pos2D(250.0f * rate, 0.0f), 0.0f, true)
			->SetTex(UICamera[0], Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
			->SetSize(200.0f, windowHeight)
			->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
		RNLib::Polygon2D().Put(0, Pos2D(windowWidth + 100.0f, windowHeightHalf) + Pos2D(-250.0f * rate, 0.0f), 0.0f, true)
			->SetTex(UICamera[1], Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
			->SetSize(200.0f, windowHeight)
			->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

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

		RNLib::Polygon3D().Put(PRIORITY_OBJECT, RNLib::Matrix().MultiplyMtx(UIDoll[1]->GetBoneState(0).GetWorldMtx(), eyeMtx))
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::CHR_BLACK_EYE], (eyeCounter2 > 0), 2, 1)
			->SetSize(4.0f, 4.0f)
			->SetClippingCamera(*UICamera[1]);
	}

	// �w�i�ݒu����
	PutBackGround();

	// �R�C��UI�̍X�V����
	if (coinUI != NULL) {
		coinUI->Update();
	}

	// ���P�b�g�p�[�c�̍X�V����
	if (rocketparts != NULL) {
		rocketparts->Update();
	}

	// ��|�[�Y & �񃊃Z�b�g���A
	if (!isPause && !isReset) {

		// �v���C���[�̍X�V����
		if (player != NULL)
			player->Update();
	}
}

//========================================
// �X�e�[�W�I������
//========================================
void Stage::EndStage(void) {

	// �v���C���[�̉��
	if (player != NULL) {
		player->Uninit();
		delete player;
		player = NULL;
	}

	// �R�C��UI�̉��
	if (coinUI != NULL) {
		coinUI->Uninit();
		delete coinUI;
		coinUI = NULL;
	}

	// �X�e�[�W�I�u�W�F�N�g�Ɣw�i�����
	Manager::StageObjectMgr()->DeleteAll();
	Manager::BGMgr()->DeleteAll();

	// �G�t�F�N�g3D�̉��
	Manager::EffectMgr()->DeleteAll();

	// ���P�b�g�p�[�cUI�����
	if (rocketparts != NULL) {
		rocketparts->Uninit();
		delete rocketparts;
		rocketparts = NULL;
	}

	// �����v���C���[�̏I������
	StageSoundPlayer::End();

	// �X�^�e�B�b�N���b�V���̍폜
	RNLib::StaticMesh().Delete(false);

	if (CheckPlanetIdx(1)) {

		// ���I������
		Fishes::End();
	}

	// UI�p�J�����̔j��
	for (int cnt = 0; cnt < 2; cnt++) {
		if (UICamera[cnt] != NULL) {
			delete UICamera[cnt];
			UICamera[cnt] = NULL;
		}
	}

	// UI�p�h�[����j��
	for (int cnt = 0; cnt < 2; cnt++) {
		delete UIDoll[cnt];
	}
}

namespace {
	//========================================
	// �w�i�̔z�u����
	//========================================
	void PutBackGround(void) {

		RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
			->SetVtxPos(Pos3D(-400.0f, 0.0f, 1000.0f), Pos3D(400.0f, 0.0f, 1000.0f), Pos3D(-400.0f, 0.0f, 0.0f), Pos3D(400.0f, 0.0f, 0.0f))
			->SetVtxCol(Color(0, 0, 0, 0), Color(0, 0, 0, 0), Color(0, 0, 0, 255), Color(0, 0, 0, 255))
			->SetCullingMode(_RNC_DrawState::CULLING_MODE::BOTH_SIDES);

		if (Stage::CheckPlanetIdx(0))
		{// [[[ �w�i�`�� ]]]

			if (Stage::CheckStageNumber(0, 0)) {
				RNLib::Polygon3D().Put(PRIORITY_UI, Pos3D(-184.0f, -100.0f, -20.0f), INITROT3D)
					->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_CONTROL_0])
					->SetSize(Size2D(150.0f, 75.0f))
					->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
				RNLib::Polygon3D().Put(PRIORITY_UI, Pos3D(184.0f, 100.0f, -20.0f), INITROT3D)
					->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::UI_CONTROL_1])
					->SetSize(Size2D(150.0f, 75.0f))
					->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
			}

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
				->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f, 200.0f), Pos3D(400.0f, 0.0f, 200.0f));

			// �_
			for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
			{
				RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
					->SetTex(CResources::TEXTURE_IDXES[cloudtex[nCnt]])
					->SetVtxPos(Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z))
					->SetZTest(false)
					->SetCol(Color{ 255,255,255,100 });

				cloudpos[nCnt].x += cloudmove[nCnt];	// �ړ��ʂ̑���

				if (cloudpos[nCnt].x >= 550.0f)
				{
					cloudpos[nCnt] = cloudpos[nCnt] = Pos3D(-500.0f + rand() % 200 - 200, 200.0f, 200.0f + rand() % 200 - 100);
					cloudpos[nCnt].z = 50 + rand() % 50;
				}
			}


			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// [[[ �ǃ��f���`�� ]]]
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		}
		if (Stage::CheckPlanetIdx(1))
		{// [[[ �w�i�`�� ]]]

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND_DEPTH, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_OCEAN])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR)
				->SetZTest(false);

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND_DEPTH, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CITY], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR)
				->SetZTest(false);

			// ���X�V����
			Fishes::Update();

			bubbleCnt++;


			if (bubbleCnt % 10 == 0)
			{
				Pos3D pos = Pos3D(100.0f + (rand() % 700 - 350), (rand() % 200 + 100), 200.0f);
				float scale = rand() % 5 + 5;



				D3DXVECTOR3 move = D3DXVECTOR3(rand() % 800 - 400, rand() % 100 + 50, 0);

				Manager::EffectMgr()->ParticleCreate(RNLib::Texture().Load("data\\TEXTURE\\BackGround\\bubble.png"),
					pos,
					Scale3D(scale, scale, 0.0f),
					Color(255, 255, 255, 255),
					CParticle::TYPE::TYPE_FLOATUP,
					MAX_BUBBLECNT,
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					move);

			}
		}
	}
}

namespace
{
	//========================================
	// ���R�[�h�̃������J��
	// Author�FHIRASAWA SHION
	//========================================
	void ClearWorldData(void)
	{
		if (pWldData != NULL)
		{
			for (int nCntWorld = 0; nCntWorld < MaxPlanet; nCntWorld++)
			{
				//�x�X�g�^�C���j��
				if (pWldData[nCntWorld].pBestTime != NULL)
				{
					delete[] pWldData[nCntWorld].pBestTime;
					pWldData[nCntWorld].pBestTime = NULL;
				}

				if (pWldData[nCntWorld].pStgRec != NULL)
				{
					//�e�X�e�[�W���Ƃ̃f�[�^�j��
					for (int nCntStage = 0; nCntStage < pWldData[nCntWorld].MaxStage; nCntStage++)
					{
						//�R�C���̉���󋵔j��
						if (pWldData[nCntWorld].pStgRec[nCntStage].pGet != NULL)
						{
							delete[] pWldData[nCntWorld].pStgRec[nCntStage].pGet;
							pWldData[nCntWorld].pStgRec[nCntStage].pGet = NULL;
						}
					}

					//�S�X�e�[�W�f�[�^�폜
					delete[] pWldData[nCntWorld].pStgRec;
					pWldData[nCntWorld].pStgRec = NULL;
				}

				//�ۑ����Ă��������J��
				if (pWldData[nCntWorld].pStart != NULL)
				{
					//�e�X�e�[�W���Ƃ̃f�[�^�j��
					for (int nCntStage = 0; nCntStage < pWldData[nCntWorld].MaxStage; nCntStage++)
					{
						//�R�C���̉���󋵔j��
						if (pWldData[nCntWorld].pStart[nCntStage].pGet != NULL)
						{
							delete[] pWldData[nCntWorld].pStart[nCntStage].pGet;
							pWldData[nCntWorld].pStart[nCntStage].pGet = NULL;
						}
					}
				
					//�S�X�e�[�W�f�[�^�폜
					delete[] pWldData[nCntWorld].pStart;
					pWldData[nCntWorld].pStart = NULL;
				}
			}

			//�S���[���h�f�[�^�폜
			delete[] pWldData;
			pWldData = NULL;
		}
		MaxPlanet = 0;
	}

	//========================================
	// ���R�[�h�̃������m��
	// Author�FHIRASAWA SHION
	//========================================
	void AllocWorldData(void)
	{
		//�X�e�[�W�G�f�B�^�[�擾
		CStageEditor *pEd = Manager::StgEd();

		//�f���̑����擾
		MaxPlanet = pEd->GetPlanetMax();

		//�f���̐������������m��
		pWldData = new WorldData[MaxPlanet];

		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//�w�肳�ꂽ�f���̃X�e�[�W�����擾
			CInt MaxStage = pWldData[nCntPlanet].MaxStage = pEd->GetType()[nCntPlanet].nStageMax;

			//�X�e�[�W�����̃��R�[�h�ꏊ�m��
			pWldData[nCntPlanet].pBestTime = new float[MaxStage];
			pWldData[nCntPlanet].pStgRec = new Stage::Data[MaxStage];
			pWldData[nCntPlanet].pStart = new Stage::Data[MaxStage];

			//�e�X�e�[�W�����N���A
			for (int nCntStage = 0; nCntStage < MaxStage; nCntStage++)
			{
				if (pWldData[nCntPlanet].pBestTime != NULL)
					pWldData[nCntPlanet].pBestTime[nCntStage] = 0.0f;

				if (pWldData[nCntPlanet].pStgRec != NULL)
				{
					pWldData[nCntPlanet].pStgRec[nCntStage].CoinNums = 0;
					pWldData[nCntPlanet].pStgRec[nCntStage].pGet = NULL;
				}

				if (pWldData[nCntPlanet].pStart != NULL)
				{
					pWldData[nCntPlanet].pStart[nCntStage].CoinNums = 0;
					pWldData[nCntPlanet].pStart[nCntStage].pGet = NULL;
				}
			}
		}
	}

	//========================================
	// ���R�[�h�t�@�C���ǂݍ���
	// Author�FHIRASAWA SHION
	//========================================
	void LoadWorldData(void)
	{
		if (pWldData != NULL) return;

		FILE *pFile = fopen(STAGE_DATA, "r");
		if (pFile == NULL) return;

		//�������m��
		AllocWorldData();

		char Text[TXT_MAX] = {}; // ��s���̕���
		while (true)
		{
			//1�s�ǂݎ��
			fgets(&Text[0], TXT_MAX, pFile);

			//�ǂݍ��񂾕�����̒��ɃR�����g�����邩�`�F�b�N
			char *pCharPos = strchr(&Text[0], COMMENT);

			//�R�����g�A�E�g�p�̕������������炻�̕����ȍ~���폜
			if (pCharPos != nullptr)*pCharPos = '\0';

			//�^�u����
			while (Text[0] == '\t')
			{
				char aCodeBackup[TXT_MAX];
				strcpy(&aCodeBackup[0], &Text[0]);//�ǂݍ��񂾂P�s��ۑ�����
				strcpy(&Text[0], &aCodeBackup[1]);//���̃^�u�������O����������̕����ŏ㏑������
			}

			//�ǂݎ��I��
			if (Text[0] == EOF || feof(pFile)) break;

			//���[�h
			else if (strncmp(&Text[0], CODE_RECORD, sizeof CODE_RECORD - 1) == 0)
			{
				char *pSprit = strtok(&Text[0], CHR_PAUSE); // ��؂蕶���܂ł�����
				CInt planetID = LoadInt(NULL, CHR_PAUSE);   // �f���ԍ��擾
				CInt StageID = LoadInt(NULL, CHR_PAUSE);    // �X�e�[�W�ԍ��擾

				WorldData& rWld = pWldData[planetID]; //�����̂ŏȗ�

				rWld.pBestTime[StageID] = LoadFloat(NULL, CHR_PAUSE);//���R�[�h���

				CInt NumCoin = rWld.pStgRec[StageID].CoinNums = LoadInt(NULL, CHR_PAUSE);//�R�C�����擾

				if (NumCoin != 0)
				{
					//����󋵏ꏊ�m��
					rWld.pStgRec[StageID].pGet = new bool[NumCoin];

					for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
					{
						//�R�C������󋵎擾
						rWld.pStgRec[StageID].pGet[nCntCoin] = LoadInt(NULL, CHR_PAUSE) == 0 ? false : true;
					}
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);

		//�������ۑ�
		RefreshData();
	}

	//========================================
	// ���R�[�h�t�@�C�������o��
	// Author�FHIRASAWA SHION
	//========================================
	void SaveWorldData(void)
	{
#ifdef _DEBUG
		return;
#endif

		if (pWldData == NULL) return;

		FILE *pFile = fopen(STAGE_DATA, "w");
		if (pFile == NULL) return;

		const char *WORLD_COMMENT = "\n#=====[ %d�� ]\n";

		//�t�@�C���w�b�_�R�����g�����o��
		fprintf(pFile, "#=======================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#�e�X�e�[�W�̃N���A�^�C��\n");
		fprintf(pFile, "#Author:HIRASAWA SHION\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#=======================================\n");
		fprintf(pFile, "SET_RECORD\n\n");

		//���R�[�h�̐�����
		fprintf(pFile, "#���̃t�H�[�}�b�g��Ώ���!!\n");
		fprintf(pFile, "#���[���h - �X�e�[�W - �x�X�g�^�C�� = �R�C���� - n���ڂ̃R�C���̉����(0=�����  1=����ς�)\n");
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//���[���h�������o��
			fprintf(pFile, WORLD_COMMENT, nCntPlanet + 1);

			for (int nCntStage = 0; nCntStage < pWldData[nCntPlanet].MaxStage; nCntStage++)
			{
				//���R�[�h�L�q
				fprintf(pFile, "	%s = %d - %d - %.2f = ",
						CODE_RECORD, nCntPlanet, nCntStage, pWldData[nCntPlanet].pBestTime[nCntStage]);

				if (pWldData[nCntPlanet].pStgRec == NULL)
					fprintf(pFile, "0");
				else
				{
					//�R�C�����Q��
					CInt& NumCoins = pWldData[nCntPlanet].pStgRec[nCntStage].CoinNums;

					//�R�C���������o��
					fprintf(pFile, "%d", NumCoins);

					//����󋵏����o��
					for (int nCntCoin = 0; nCntCoin < NumCoins; nCntCoin++)
					{
						fprintf(pFile, " - %d", pWldData[nCntPlanet].pStgRec[nCntStage].pGet[nCntCoin] ? 1 : 0);
					}
				}

				fprintf(pFile, "\n");
			}
		}

		//�I��
		fprintf(pFile, "\n%s", END_RECORD);
		fclose(pFile);
	}

	//========================================
	//���[���h/�X�e�[�W����h��ւ�
	// Author�FHIRASAWA SHION
	//========================================
	void RefreshData(void)
	{
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//�����̂ŏȗ�
			WorldData& rWld = pWldData[nCntPlanet];

			for (int nCntStage = 0; nCntStage < rWld.MaxStage; nCntStage++)
			{
				//�R�C�����擾
				CInt& NumCoin = rWld.pStgRec[nCntStage].CoinNums;

				//�O�Ȃ�����Ԃ�
				if (NumCoin == 0) continue;

				//�R�C�����Ɖ���󋵕ۑ��ꏊ�m��
				rWld.pStart[nCntStage].CoinNums = NumCoin;
				rWld.pStart[nCntStage].pGet = new bool[NumCoin];

				for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
				{
					//����󋵑��
					rWld.pStart[nCntStage].pGet[nCntCoin] = rWld.pStgRec[nCntStage].pGet[nCntCoin];
				}
			}
		}
	}
}

//========================================
// �w�肳�ꂽ�X�e�[�W�̃x�X�g�^�C����Ԃ�
// Author�FHIRASAWA SHION
//========================================
float Stage::GetBestTime(CInt& planetIdx, CInt& stageIdx)
{
	//���R�[�h�Ǎ�
	LoadWorldData();

	//�ǂݍ��߂���x�X�g�^�C����Ԃ�
	if (pWldData != NULL)
		return pWldData[planetIdx].pBestTime[stageIdx];

	//���s�Ȃ�傫��������Ԃ�
	return 10000.0f;
}

//========================================
// �^�C���X�V
// Author�FHIRASAWA SHION
//========================================
void Stage::RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime)
{
	LoadWorldData();

	//�^�C���X�V
	if (ClearTime < pWldData[planetIdx].pBestTime[stageIdx])
		pWldData[planetIdx].pBestTime[stageIdx] = ClearTime;
}

//========================================
// �X�e�[�W�����擾
// Author�FHIRASAWA SHION
//========================================
Stage::Data Stage::GetData(CInt& planetIdx, CInt& stageIdx)
{
	LoadWorldData();

	//�w�肳�ꂽ�X�e�[�W�̏���Ԃ�
	return pWldData[planetIdx].pStgRec[stageIdx];
}

//========================================
// �R�C������󋵂��擾
// Author�FHIRASAWA SHION
//========================================
bool Stage::GetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID)
{
	LoadWorldData();

	if (pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL)
	{
		RNLib::Window().Message_ERROR(String("���̃X�e�[�W�ɂ̓R�C���͔z�u����Ă��܂���B"));
		return false;
	}
	else if (coinID >= pWldData[planetIdx].pStgRec[stageIdx].CoinNums) {
		RNLib::Window().Message_ERROR(String("�R�C���̖��������ۂ̔z�u���Ƃ���Ă��܂��B\nID:%d ����:%d", coinID, pWldData[planetIdx].pStgRec[stageIdx].CoinNums));
		return false;
	}

	//����󋵂�Ԃ�
	return pWldData[planetIdx].pStgRec[stageIdx].pGet[coinID];
}

//========================================
// �O���t�@�C����肷�łɉ�����Ă���R�C���̑�����Ԃ�
// Author�FHIRASAWA SHION
//========================================
int Stage::GetCoinAll(void)
{
	LoadWorldData();

	int nNumAll = 0;

	for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
	{
		//�����̂ŏȗ�
		WorldData& rWld = pWldData[nCntPlanet];

		for (int nCntStage = 0; nCntStage < rWld.MaxStage; nCntStage++)
		{
			//�R�C�����擾
			CInt& NumCoin = rWld.pStgRec[nCntStage].CoinNums;

			//�O�Ȃ�����Ԃ�
			if (NumCoin == 0) continue;

			for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
			{
				//����󋵑��
				if (rWld.pStgRec[nCntStage].pGet[nCntCoin])nNumAll++;
			}
		}
	}

	//���ɉ�����Ă��鑍����Ԃ�
	return nNumAll;
}

//========================================
// �R�C������󋵂�ݒ�
// Author�FHIRASAWA SHION
//========================================
void  Stage::SetCoinInfo(CInt& planetIdx, CInt& stageIdx, const Data& data)
{
	LoadWorldData();

	//�R�C����������Ă�����ݒ肵�Ȃ�
	if (pWldData[planetIdx].pStgRec[stageIdx].CoinNums != data.CoinNums ||
		pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL) return;

	for (int nCntData = 0; nCntData < data.CoinNums; nCntData++)
	{
		//����󋵑��
		pWldData[planetIdx].pStgRec[stageIdx].pGet[nCntData] = data.pGet[nCntData];
	}
}

//========================================
// �R�C������󋵂�ݒ�
// Author�FHIRASAWA SHION
//========================================
void Stage::SetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID, const bool& bGet)
{
	LoadWorldData();

	//�R�C����������Ă�����ݒ肵�Ȃ�
	if (pWldData[planetIdx].pStgRec[stageIdx].CoinNums <= coinID ||
		pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL) return;

	//����󋵑��
	pWldData[planetIdx].pStgRec[stageIdx].pGet[coinID] = bGet;
}