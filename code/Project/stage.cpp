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

#define  MAX_COUNT		(2000)
#define  MAX_CLOUD		(5)
#define  MAX_BUBBLE		(7)
#define  MAX_BUBBLECNT	(640)
//****************************************
// �������
//****************************************
namespace {
	//========== [[[ �֐��錾 ]]]
	void PutBackGround(void);
	void ClearRecord(void);
	void AllocRecord(void);
	void LoadRecord(void);
	void SaveRecord(void);
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
	bool            isCutIn;
	short           wallModelIdxes[2];
	CCamera*        UICamera[2];
	CDoll3D*        UIDoll[2];

	//�X�e�[�W�N���A�^�C���̕ۑ��ꏊ
	struct Record
	{
		int MaxStage;     //�X�e�[�W��
		float *pBestTime; //�e�X�e�[�W�̃x�X�g�^�C��
	};
	Record *pRecord; //�f�����Ƃ̃��R�[�h
	int MaxPlanet;   //�ő�f����

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
void Stage::SetRocketPartsUI(CRocketPartsUI* parts)                           { rocketparts = parts; }
void Stage::SetIsCutIn      (const bool& setIsCutIn)                          { isCutIn     = setIsCutIn; }

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
	fishpos = Pos3D(100.0f,300.0f,0.0f);

	for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
	{
		cloudpos[nCnt] = Pos3D(-400.0f + rand() % 400,100.0f,0.0f + rand() % 200 - 100);
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
	ClearRecord();
}

//========================================
// �I������
//========================================
void Stage::Uninit(void)
{
	// �����v���C���[�̏I������
	StageSoundPlayer::Uninit();

	//���R�[�h�L�^
	SaveRecord();

	//�������J��
	ClearRecord();
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

	// �X�e�[�W����
	Manager::StgEd()->StageLoad(planetIdx, stageIdx);

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

	for (int cnt = 0; cnt < 2; cnt++) {
		{// [[[ UI�p�J�����̐��� ]]]
			UICamera[cnt] = new CCamera(Size2D(200.0f, RNLib::Window().GetHeight()));

			// �N���b�s���O���I��
			UICamera[cnt]->SetIsClipping(true);

			// �w�i�F�ݒ�
			UICamera[cnt]->SetBGCol(Color(0, 0, 0, 100));

			// ���C�g��ݒ�
			UICamera[cnt]->SetLightID(Manager::GetLightIdx(0));
		}

		{// [[[ UI�p�h�[���𐶐� ]]]
			UIDoll[cnt] = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load(cnt == 0 ? "data\\SETUP\\Player_Mouth.txt" : "data\\SETUP\\Player_Eye.txt"));

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
}

//========================================
// �X�e�[�W�X�V����
//========================================
void Stage::UpdateStage(void) {

	// �����v���C���[�̍X�V����
	StageSoundPlayer::Update();

	// �E�B���h�E�����擾
	const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
	const float windowWidth       = RNLib::Window().GetWidth();
	const float windowHeight      = RNLib::Window().GetHeight();
	const float windowHeightHalf  = windowHeight * 0.5f;
	const float windowHeightHalf2 = windowHeightHalf * 0.5f;

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {
			// ���J�����`��
			RNLib::Polygon2D().Put(0, true)
				->SetPos(windowCenterPos + Pos2D(0.0f, windowHeightHalf2))
				->SetTexUV(Manager::GetSubCamera(), Pos2D(0.0f, 0.5f), Pos2D(1.0f, 0.5f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
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
			if (--counter < 0)
				counter = 0;
		}
		const float rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, counter, 30);

		// UI�J�����`��
		RNLib::Polygon2D().Put(0, true)
			->SetPos(Pos2D(-100.0f, windowHeightHalf) + Pos2D(250.0f * rate, 0.0f))
			->SetTexUV(UICamera[0], Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
			->SetSize(200.0f, windowHeight)
			->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
		RNLib::Polygon2D().Put(0, true)
			->SetPos(Pos2D(windowWidth + 100.0f, windowHeightHalf) + Pos2D(-250.0f * rate, 0.0f))
			->SetTexUV(UICamera[1], Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
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

		RNLib::Polygon3D().Put(PRIORITY_OBJECT, RNLib::Matrix().MultiplyMtx(eyeMtx, UIDoll[1]->GetBoneState(0).GetWorldMtx()))
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

	// ��|�[�Y���A
	if (!isPause) {

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
	Manager::StageObjectMgr()->ReleaseAll();
	Manager::BGMgr()->ReleaseAll();

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

		if (Stage::CheckPlanetIdx(0))
		{// [[[ �w�i�`�� ]]]

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
				->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f, 200.0f), Pos3D(400.0f, 0.0f, 200.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// �_
			for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
			{
				RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
					->SetTex(CResources::TEXTURE_IDXES[cloudtex[nCnt]])
					->SetVtxPos(Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z))
					->SetBillboard(true)
					->SetZTest(false)
					->SetCol(Color{ 255,255,255,100 });

				cloudpos[nCnt].x += cloudmove[nCnt];	// �ړ��ʂ̑���

				if (cloudpos[nCnt].x >= 550.0f)
				{
					cloudpos[nCnt] = cloudpos[nCnt] = Pos3D(-500.0f + rand() % 200 - 200, 200.0f, 200.0f + rand() % 200 - 100);
				}
			}


			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// [[[ �ǃ��f���`�� ]]]
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		}
		if (Stage::CheckPlanetIdx(1))
		{// [[[ �w�i�`�� ]]]

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_OCEAN])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CITY], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FISH])
				->SetVtxPos(Pos3D(-100.0f + fishpos.x, 100.0f + fishpos.y, 700.0f), Pos3D(0.0f + fishpos.x, 100.0f + fishpos.y, 700.0f), Pos3D(-100.0f + fishpos.x, -100.0f + fishpos.y, 700.0f), Pos3D(0.0f + fishpos.x, -100.0f + fishpos.y, 700.0f))
				->SetBillboard(true);

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

//========================================
// ���R�[�h�̃������J��
// Author�FHIRASAWA SHION
//========================================
namespace
{
	void ClearRecord(void)
	{
		if (pRecord != NULL)
		{
			for (int nCntRecord = 0; nCntRecord < MaxPlanet; nCntRecord++)
			{
				if (pRecord[nCntRecord].pBestTime != NULL)
				{
					delete[] pRecord[nCntRecord].pBestTime;
					pRecord[nCntRecord].pBestTime = NULL;
				}
			}

			delete[] pRecord;
			pRecord = NULL;
		}
		MaxPlanet = 0;
	}
}

//========================================
// ���R�[�h�̃������m��
// Author�FHIRASAWA SHION
//========================================
namespace
{
	void AllocRecord(void)
	{
		//�X�e�[�W�G�f�B�^�[�擾
		CStageEditor *pEd = Manager::StgEd();

		//�f���̑����擾
		MaxPlanet = pEd->GetPlanetMax();

		//�f���̐������������m��
		pRecord = new Record[MaxPlanet];

		for (int nCntStage = 0; nCntStage < MaxPlanet; nCntStage++)
		{
			//�w�肳�ꂽ�f���̃X�e�[�W�����擾
			CInt MaxStage = pRecord[nCntStage].MaxStage = pEd->GetType()[nCntStage].nStageMax;

			//�X�e�[�W�����̃��R�[�h�ꏊ�m��
			pRecord[nCntStage].pBestTime = new float[MaxStage];
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
	LoadRecord();

	//�ǂݍ��߂���x�X�g�^�C����Ԃ�
	if (pRecord != NULL)
		return pRecord[planetIdx].pBestTime[stageIdx];

	//���s�Ȃ�傫��������Ԃ�
	return 10000.0f;
}

//========================================
// �^�C���X�V
// Author�FHIRASAWA SHION
//========================================
void Stage::RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime)
{
	LoadRecord();

	//�^�C���X�V
	if (ClearTime < pRecord[planetIdx].pBestTime[stageIdx])
		pRecord[planetIdx].pBestTime[stageIdx] = ClearTime;
}

//========================================
// ���R�[�h�t�@�C���ǂݍ���
// Author�FHIRASAWA SHION
//========================================
namespace 
{
	void LoadRecord(void)
	{
		if (pRecord != NULL) return;

		FILE *pFile = fopen("data\\GAMEDATA\\STAGE\\CLEAR_TIME.txt", "r");
		if (pFile == NULL) return;

		//�������m��
		AllocRecord();

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
				pRecord[planetID].pBestTime[StageID] = LoadFloat(NULL, CHR_PAUSE);//���R�[�h���
			}
		}

		fclose(pFile);
	}
}

//========================================
// ���R�[�h�t�@�C�������o��
// Author�FHIRASAWA SHION
//========================================
namespace 
{
	void SaveRecord(void)
	{
		if (pRecord == NULL) return;

		FILE *pFile = fopen("data\\GAMEDATA\\STAGE\\CLEAR_TIME.txt", "w");
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
		fprintf(pFile, "#���[���h - �X�e�[�W - �x�X�g�^�C��");
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//���[���h�������o��
			fprintf(pFile, WORLD_COMMENT, nCntPlanet + 1);

			for (int nCntStage = 0; nCntStage < pRecord[nCntPlanet].MaxStage; nCntStage++)
			{
				//���R�[�h�L�q
				fprintf(pFile, "	%s = %d - %d - %.2f\n", 
						CODE_RECORD, nCntPlanet, nCntStage, pRecord[nCntPlanet].pBestTime[nCntStage]);
			}
		}

		//�I��
		fprintf(pFile, "\n%s", END_RECORD);
		fclose(pFile);
	}
}