//========================================
// 
// ステージの処理
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
//****************************************
// 無名空間
//****************************************
namespace {
	//========== [[[ 関数宣言 ]]]
	void PutBackGround(void);

	//========== [[[ 変数宣言 ]]]
	int             planetIdx;
	int             stageIdx;
	int				Count;
	Pos3D			cloudpos[MAX_CLOUD];
	float			cloudmove[MAX_CLOUD];
	CPlayer*        player;
	CCoinUI*        coinUI;
	CRocketPartsUI* rocketparts;
	bool            isPause;
	short           wallModelIdxes[2];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ステージの関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 設定系関数
//========================================
void Stage::SetStageNumber(const int& setPlanetIdx, const int& setStageIdx) { planetIdx   = setPlanetIdx; stageIdx = setStageIdx; }
void Stage::SetPause      (const bool& setIsPause)                          { isPause     = setIsPause; }
void Stage::SetRocketPartsUI(CRocketPartsUI* parts)                           { rocketparts = parts; }

//========================================
// 取得系関数
//========================================
CPlayer* Stage::GetPlayer(void) { return player; }

//========================================
// 初期化処理
//========================================
void Stage::Init(void) {

	planetIdx = 0;
	stageIdx = 0;
	Count = 0;
	player = NULL;
	coinUI = NULL;
	rocketparts = NULL;
	isPause = false;
	for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
	{
		cloudpos[nCnt] = Pos3D(-400.0f + rand() % 200,200.0f,200.0f + rand() % 200 - 100);
		cloudmove[nCnt] = (rand() % 20 + 10) * 0.01f;
	}
	// ブロックの読み込み処理
	CBlock::Load();

	// 環境音プレイヤーの初期化処理
	StageSoundPlayer::Init();
}

//========================================
// 終了処理
//========================================
void Stage::Uninit(void) {

	// 環境音プレイヤーの終了処理
	StageSoundPlayer::Uninit();
}

//========================================
// 惑星番号を調べる
//========================================
bool Stage::CheckPlanetIdx(const int& planetIdx) {

	return Manager::StgEd()->GetPlanetIdx() == planetIdx;
}

//========================================
// ステージ番号確認処理
//========================================
bool Stage::CheckStageNumber(const int& planetIdx, const int& stageIdx) {

	if (Manager::StgEd()->GetPlanetIdx() == planetIdx) {
		return Manager::StgEd()->GetType()[planetIdx].nStageIdx == stageIdx;
	}

	return false;
}

//========================================
// ステージ開始処理（※やり直しを含む）
//========================================
void Stage::StartStage(void) {

	// ステージ生成
	Manager::StgEd()->StageLoad(planetIdx, stageIdx);

	// プレイヤーの生成
	if (player == NULL)
		player = CPlayer::Create();

	// コインUIの生成
	if (coinUI == NULL)
		coinUI = CCoinUI::Create();

	{// [[[ カメラ ]]]
		// カメラの視点/注視点を設定
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		Manager::GetSubCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 壁モデル読み込み
	wallModelIdxes[0] = RNLib::Model().Load("data\\MODEL\\Wall_Left.x");
	wallModelIdxes[1] = RNLib::Model().Load("data\\MODEL\\Wall_Right.x");

	// 環境音プレイヤーの開始処理
	StageSoundPlayer::Start();
}

//========================================
// ステージ更新処理
//========================================
void Stage::UpdateStage(void) {

	// 環境音プレイヤーの更新処理
	StageSoundPlayer::Update();

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {
			// [[[ 上下カメラ描画 ]]]
			const Pos2D windowCenterPos = RNLib::Window().GetCenterPos();
			const float windowWidth = RNLib::Window().GetWidth();
			const float windowHeight = RNLib::Window().GetHeight();
			const float windowHeightHalf = windowHeight * 0.5f;
			const float windowHeightHalf2 = windowHeightHalf * 0.5f;

			// 下
			RNLib::Polygon2D().Put(0, true)
				->SetPos(windowCenterPos + Pos2D(0.0f, windowHeightHalf2))
				->SetTexUV(Manager::GetSubCamera(), Pos2D(0.0f, 0.5f), Pos2D(1.0f, 0.5f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
				->SetSize(windowWidth, windowHeightHalf);
		}
	}

	// 背景設置処理
	PutBackGround();

	// コインUIの更新処理
	if (coinUI != NULL) {
		coinUI->Update();
	}

	// ロケットパーツの更新処理
	if (rocketparts != NULL) {
		rocketparts->Update();
	}

	// 非ポーズ時、
	if (!isPause) {

		// プレイヤーの更新処理
		if (player != NULL)
			player->Update();
	}
}

//========================================
// ステージ終了処理
//========================================
void Stage::EndStage(void) {

	// プレイヤーの解放
	if (player != NULL) {
		player->Uninit();
		delete player;
		player = NULL;
	}

	// コインUIの解放
	if (coinUI != NULL) {
		coinUI->Uninit();
		delete coinUI;
		coinUI = NULL;
	}

	// ステージオブジェクトと背景を解放
	Manager::StageObjectMgr()->ReleaseAll();
	Manager::BGMgr()->ReleaseAll();

	// ロケットパーツUIを解放
	if (rocketparts != NULL) {
		rocketparts->Uninit();
		delete rocketparts;
		rocketparts = NULL;
	}

	// 環境音プレイヤーの終了処理
	StageSoundPlayer::End();

	// スタティックメッシュの削除
	RNLib::MatMesh().Delete();
}

namespace {
	//========================================
	// 背景の配置処理
	//========================================
	void PutBackGround(void) {

		if (Stage::CheckPlanetIdx(0))
		{// [[[ 背景描画 ]]]
			
			// 上
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
				->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f + 32.0f, 200.0f), Pos3D(400.0f, 0.0f + 32.0f, 200.0f))
				->SetBillboard(true);
			
			// 雲
			for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
			{
				RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
					->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CLOUD])
					->SetVtxPos(Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z))
					->SetBillboard(true)
					->SetZTest(false)
					->SetCol(Color{ 255,255,255,100 });

				cloudpos[nCnt].x += cloudmove[nCnt];	// 移動量の増加

				if (cloudpos[nCnt].x >= 550.0f)
				{
					cloudpos[nCnt] = cloudpos[nCnt] = Pos3D(-500.0f + rand() % 100 - 100, 200.0f, 200.0f + rand() % 200 - 100);
				}
			}
		

			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true);

		}
		if (Stage::CheckPlanetIdx(1))
		{// [[[ 背景描画 ]]]

			// 上
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_OCEAN])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true);

			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CITY], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true);
		}

		// [[[ 壁モデル描画 ]]]
		RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
	}
}