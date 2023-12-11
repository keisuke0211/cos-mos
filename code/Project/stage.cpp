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

//****************************************
// �������
//****************************************
namespace {
	//========== [[[ �֐��錾 ]]]
	void PutBackGround(void);

	//========== [[[ �ϐ��錾 ]]]
	int             planetIdx;
	int             stageIdx;
	int				Count;
	CPlayer*        player;
	CCoinUI*        coinUI;
	CRocketPartsUI* rocketparts;
	bool            isPause;
	short           wallModelIdxes[2];
}

#define  MAX_COUNT		(1800)
//================================================================================
//----------|---------------------------------------------------------------------
//==========| �X�e�[�W�̊֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ݒ�n�֐�
//========================================
void Stage::SetStageNumber(const int& setPlanetIdx, const int& setStageIdx) { planetIdx   = setPlanetIdx; stageIdx = setStageIdx; }
void Stage::SetPause      (const bool& setIsPause)                          { isPause     = setIsPause; }
void Stage::SetRocketPartsUI(CRocketPartsUI* parts)                           { rocketparts = parts; }

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
	Count = 0;
	player = NULL;
	coinUI = NULL;
	rocketparts = NULL;
	isPause = false;

	// �u���b�N�̓ǂݍ��ݏ���
	CBlock::Load();

	// �����v���C���[�̏���������
	StageSoundPlayer::Init();
}

//========================================
// �I������
//========================================
void Stage::Uninit(void) {

	// �����v���C���[�̏I������
	StageSoundPlayer::Uninit();
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
		coinUI = CCoinUI::Create();

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
}

//========================================
// �X�e�[�W�X�V����
//========================================
void Stage::UpdateStage(void) {

	// �����v���C���[�̍X�V����
	StageSoundPlayer::Update();

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {
			// [[[ �㉺�J�����`�� ]]]
			const Pos2D windowCenterPos = RNLib::Window().GetCenterPos();
			const float windowWidth = RNLib::Window().GetWidth();
			const float windowHeight = RNLib::Window().GetHeight();
			const float windowHeightHalf = windowHeight * 0.5f;
			const float windowHeightHalf2 = windowHeightHalf * 0.5f;

			// ��
			RNLib::Polygon2D().Put(0, true)
				->SetPos(windowCenterPos + Pos2D(0.0f, windowHeightHalf2))
				->SetTexUV(Manager::GetSubCamera(), Pos2D(0.0f, 0.5f), Pos2D(1.0f, 0.5f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
				->SetSize(windowWidth, windowHeightHalf);
		}
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

	// �}�e���A�����b�V���̍폜
	RNLib::MatMesh().Delete();
}

namespace {
	//========================================
	// �w�i�̔z�u����
	//========================================
	void PutBackGround(void) {

		if (Stage::CheckPlanetIdx(0))
		{// [[[ �w�i�`�� ]]]
			Count++;

			// �����v�Z 
			CFloat fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, Count, MAX_COUNT);

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
				->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f + 32.0f, 200.0f), Pos3D(400.0f, 0.0f + 32.0f, 200.0f))
				->SetBillboard(true);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CLOUD])
				->SetVtxPos(Pos3D(-1000.0f + (2000.0f * fCountRate), 200.0f + 32.0f, 200.0f), Pos3D(-600.0f + (2000.0f * fCountRate), 200.0f + 32.0f, 200.0f), Pos3D(-1000.0f + (2000.0f * fCountRate), 100.0f + 32.0f, 200.0f), Pos3D(-600.0f + (2000.0f * fCountRate), 100.0f + 32.0f, 200.0f))
				->SetBillboard(true)
				->SetZTest(false)
				->SetCol(Color{ 255,255,255,100 });
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CLOUD])
				->SetVtxPos(Pos3D(-1200.0f + (2200.0f * fCountRate), 200.0f + 32.0f, 300.0f), Pos3D(-800.0f + (2200.0f * fCountRate), 200.0f + 32.0f, 300.0f), Pos3D(-1200.0f + (2200.0f * fCountRate), 100.0f + 32.0f, 300.0f), Pos3D(-800.0f + (2200.0f * fCountRate), 100.0f + 32.0f, 300.0f))
				->SetBillboard(true)
				->SetZTest(false)
				->SetCol(Color{ 255,255,255,100 });
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CLOUD])
				->SetVtxPos(Pos3D(-1000.0f + (2500.0f * fCountRate), 200.0f + 32.0f, 400.0f), Pos3D(-600.0f + (2500.0f * fCountRate), 200.0f + 32.0f, 400.0f), Pos3D(-1000.0f + (2500.0f * fCountRate), 100.0f + 32.0f, 400.0f), Pos3D(-600.0f + (2500.0f * fCountRate), 100.0f + 32.0f, 400.0f))
				->SetBillboard(true)
				->SetZTest(false)
				->SetCol(Color{ 255,255,255,100 });

			// ��
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true);

			if (Count > MAX_COUNT)
			{
				Count = 0;
			}
		}

		// [[[ �ǃ��f���`�� ]]]
		RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
	}
}