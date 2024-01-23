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
#include "BackGround/fishes.h"

#define  MAX_COUNT		 (2000)
#define  MAX_CLOUD		 (5)
#define  MAX_BUBBLE		 (7)
#define  MAX_BUBBLECNT	 (640)

//****************************************
// 無名空間
//****************************************
namespace {
	//========== [[[ 関数宣言 ]]]
	void PutBackGround(void);
	void ClearWorldData(void);
	void AllocWorldData(void);
	void LoadWorldData(void);
	void SaveWorldData(void);
	void RefreshData(void); //ワールド/ステージ情報を塗り替え
	int LoadInt(char *pString, const char *pPunc) { return atoi(strtok(pString, pPunc)); }
	float LoadFloat(char *pString, const char *pPunc) { return (float)atof(strtok(pString, pPunc)); }

	//========== [[[ 変数宣言 ]]]
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

	//各惑星ごとのデータ
	struct WorldData
	{
		int MaxStage;      //ステージ数
		float *pBestTime;  //各ステージのベストタイム
		Stage::Data *pStgRec;//各ステージごとのデータ
		Stage::Data *pStart; //ステージ開始時の情報（やり直し時のコイン回収状況リセットなどに仕様
	};
	WorldData *pWldData; //惑星ごとのレコード
	int MaxPlanet;      //最大惑星数

	const char *STAGE_DATA = "data\\GAMEDATA\\STAGE\\STAGE_DATA.txt";

					 //文字添削
	const char COMMENT = '#';       //コメント文字
	const char CHR_END = '\0';      //終端文字
	const char CHR_TAB = '\t';      //タブ文字
	const char *CHR_PAUSE = " -=\n"; //読み取らない文字たち
	const char *SET_RECORD = "SET_RECORD";
	const char *END_RECORD = "END_RECORD";
	const char *CODE_RECORD = "RECORD";
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ステージの関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 設定系関数
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
// 取得系関数
//========================================
CPlayer* Stage::GetPlayer(void) { return player; }

//========================================
// 初期化処理
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
	// ブロックの読み込み処理
	CBlock::Load();

	// 環境音プレイヤーの初期化処理
	StageSoundPlayer::Init();

	MaxPlanet = 0;
	ClearWorldData();
	LoadWorldData();
}

//========================================
// 終了処理
//========================================
void Stage::Uninit(void)
{
	// 環境音プレイヤーの終了処理
	StageSoundPlayer::Uninit();

	//レコード記録
	SaveWorldData();

	//メモリ開放
	ClearWorldData();
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

	// ゴールフラグをFALSEに
	isGoal = false;

	// リセットしたかフラグ
	isReset;

	// ステージ生成
	Manager::StgEd()->StageLoad(planetIdx, stageIdx);

	// 制限時間を設定
	limitTimeCounter = Manager::StgEd()->GetStageLimittime(planetIdx, stageIdx) * 60;

	// プレイヤーの生成
	if (player == NULL)
		player = CPlayer::Create();

	// コインUIの生成
	if (coinUI == NULL)
		coinUI = CCoinUI::Create(D3DXVECTOR3(100.0f, 25.0f, 0.0f) * 2.0f,Scale2D(40.0f,40.0f),true);

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

	if (CheckPlanetIdx(1)) {

		// 魚更新処理
		Fishes::Start();
	}

	for (int cnt = 0; cnt < 2; cnt++) {
		{// [[[ UI用カメラの生成 ]]]
			UICamera[cnt] = new CCamera(Size2D(200.0f, RNLib::Window().GetHeight()));

			// クリッピングをオン
			UICamera[cnt]->SetIsClipping(true);

			// 背景色設定
			UICamera[cnt]->SetOverwriteBGCol(Color(0, 0, 0, 100));

			// ライトを設定
			UICamera[cnt]->SetLightID(Manager::GetLightIdx(0));
		}

		{// [[[ UI用ドールを生成 ]]]
			UIDoll[cnt] = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load(cnt == 0 ? "data\\SETUP\\Player_Mouth.txt" : "data\\SETUP\\Player_Eye.txt"), TRUE);

			// モーション設定
			UIDoll[cnt]->SetMotion(RNLib::Motion3D().Load(cnt == 0 ? "data\\MOTION\\Player_Mouth\\Happy.txt" : "data\\MOTION\\Player_Eye\\Walk.txt"));

			// 位置/向き設定
			UIDoll[cnt]->SetPos(Pos3D(0.0f, -8.0f, 100.0f));
			UIDoll[cnt]->SetRot(Rot3D(0.0f, D3DX_PI * (cnt == 0 ? 0.9f : 1.1f), 0.0f));

			// クリッピング設定
			UIDoll[cnt]->SetClippingCamera(UICamera[cnt]->GetID());
		}
	}

	// カメラのライト
	Manager::GetMainCamera()->SetLightID(Manager::GetLightIdx(Manager::StgEd()->GetPlanetIdx() + 1));
	Manager::GetSubCamera()->SetLightID(Manager::GetLightIdx(Manager::StgEd()->GetPlanetIdx() + 1));

	// リセットフラグをFALSEにする
	isReset = false;
}

//========================================
// ステージ更新処理
//========================================
void Stage::UpdateStage(void) {

	// 環境音プレイヤーの更新処理
	StageSoundPlayer::Update();

	static bool isLimit = true;
	if (RNLib::Input().GetKeyTrigger(DIK_F8) && RNSystem::GetMode() == RNSystem::MODE::DEBUG)
		isLimit = !isLimit;

	// 制限時間のカウント
	if(GetIsShowUI() && isLimit) {
		if (--limitTimeCounter <= 0) {

			// タイムオーバーの処理
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

	// 時間表示
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

	// ウィンドウ情報を取得
	const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
	const float windowWidth       = RNLib::Window().GetWidth();
	const float windowHeight      = RNLib::Window().GetHeight();
	const float windowHeightHalf  = windowHeight * 0.5f;
	const float windowHeightHalf2 = windowHeightHalf * 0.5f;

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {

			// 下カメラ描画
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

		// UIカメラ描画
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

	// 非ポーズ & 非リセット時、
	if (!isPause && !isReset) {

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
	Manager::StageObjectMgr()->DeleteAll();
	Manager::BGMgr()->DeleteAll();

	// エフェクト3Dの解放
	Manager::EffectMgr()->DeleteAll();

	// ロケットパーツUIを解放
	if (rocketparts != NULL) {
		rocketparts->Uninit();
		delete rocketparts;
		rocketparts = NULL;
	}

	// 環境音プレイヤーの終了処理
	StageSoundPlayer::End();

	// スタティックメッシュの削除
	RNLib::StaticMesh().Delete(false);

	if (CheckPlanetIdx(1)) {

		// 魚終了処理
		Fishes::End();
	}

	// UI用カメラの破棄
	for (int cnt = 0; cnt < 2; cnt++) {
		if (UICamera[cnt] != NULL) {
			delete UICamera[cnt];
			UICamera[cnt] = NULL;
		}
	}

	// UI用ドールを破棄
	for (int cnt = 0; cnt < 2; cnt++) {
		delete UIDoll[cnt];
	}
}

namespace {
	//========================================
	// 背景の配置処理
	//========================================
	void PutBackGround(void) {

		RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
			->SetVtxPos(Pos3D(-400.0f, 0.0f, 1000.0f), Pos3D(400.0f, 0.0f, 1000.0f), Pos3D(-400.0f, 0.0f, 0.0f), Pos3D(400.0f, 0.0f, 0.0f))
			->SetVtxCol(Color(0, 0, 0, 0), Color(0, 0, 0, 0), Color(0, 0, 0, 255), Color(0, 0, 0, 255))
			->SetCullingMode(_RNC_DrawState::CULLING_MODE::BOTH_SIDES);

		if (Stage::CheckPlanetIdx(0))
		{// [[[ 背景描画 ]]]

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

			// 上
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
				->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f, 200.0f), Pos3D(400.0f, 0.0f, 200.0f));

			// 雲
			for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
			{
				RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
					->SetTex(CResources::TEXTURE_IDXES[cloudtex[nCnt]])
					->SetVtxPos(Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z))
					->SetZTest(false)
					->SetCol(Color{ 255,255,255,100 });

				cloudpos[nCnt].x += cloudmove[nCnt];	// 移動量の増加

				if (cloudpos[nCnt].x >= 550.0f)
				{
					cloudpos[nCnt] = cloudpos[nCnt] = Pos3D(-500.0f + rand() % 200 - 200, 200.0f, 200.0f + rand() % 200 - 100);
					cloudpos[nCnt].z = 50 + rand() % 50;
				}
			}


			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// [[[ 壁モデル描画 ]]]
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		}
		if (Stage::CheckPlanetIdx(1))
		{// [[[ 背景描画 ]]]

			// 上
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND_DEPTH, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_OCEAN])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR)
				->SetZTest(false);

			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND_DEPTH, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CITY], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR)
				->SetZTest(false);

			// 魚更新処理
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
	// レコードのメモリ開放
	// Author：HIRASAWA SHION
	//========================================
	void ClearWorldData(void)
	{
		if (pWldData != NULL)
		{
			for (int nCntWorld = 0; nCntWorld < MaxPlanet; nCntWorld++)
			{
				//ベストタイム破棄
				if (pWldData[nCntWorld].pBestTime != NULL)
				{
					delete[] pWldData[nCntWorld].pBestTime;
					pWldData[nCntWorld].pBestTime = NULL;
				}

				if (pWldData[nCntWorld].pStgRec != NULL)
				{
					//各ステージごとのデータ破棄
					for (int nCntStage = 0; nCntStage < pWldData[nCntWorld].MaxStage; nCntStage++)
					{
						//コインの回収状況破棄
						if (pWldData[nCntWorld].pStgRec[nCntStage].pGet != NULL)
						{
							delete[] pWldData[nCntWorld].pStgRec[nCntStage].pGet;
							pWldData[nCntWorld].pStgRec[nCntStage].pGet = NULL;
						}
					}

					//全ステージデータ削除
					delete[] pWldData[nCntWorld].pStgRec;
					pWldData[nCntWorld].pStgRec = NULL;
				}

				//保存していた情報も開放
				if (pWldData[nCntWorld].pStart != NULL)
				{
					//各ステージごとのデータ破棄
					for (int nCntStage = 0; nCntStage < pWldData[nCntWorld].MaxStage; nCntStage++)
					{
						//コインの回収状況破棄
						if (pWldData[nCntWorld].pStart[nCntStage].pGet != NULL)
						{
							delete[] pWldData[nCntWorld].pStart[nCntStage].pGet;
							pWldData[nCntWorld].pStart[nCntStage].pGet = NULL;
						}
					}
				
					//全ステージデータ削除
					delete[] pWldData[nCntWorld].pStart;
					pWldData[nCntWorld].pStart = NULL;
				}
			}

			//全ワールドデータ削除
			delete[] pWldData;
			pWldData = NULL;
		}
		MaxPlanet = 0;
	}

	//========================================
	// レコードのメモリ確保
	// Author：HIRASAWA SHION
	//========================================
	void AllocWorldData(void)
	{
		//ステージエディター取得
		CStageEditor *pEd = Manager::StgEd();

		//惑星の総数取得
		MaxPlanet = pEd->GetPlanetMax();

		//惑星の数だけメモリ確保
		pWldData = new WorldData[MaxPlanet];

		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//指定された惑星のステージ数を取得
			CInt MaxStage = pWldData[nCntPlanet].MaxStage = pEd->GetType()[nCntPlanet].nStageMax;

			//ステージ数分のレコード場所確保
			pWldData[nCntPlanet].pBestTime = new float[MaxStage];
			pWldData[nCntPlanet].pStgRec = new Stage::Data[MaxStage];
			pWldData[nCntPlanet].pStart = new Stage::Data[MaxStage];

			//各ステージ情報をクリア
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
	// レコードファイル読み込み
	// Author：HIRASAWA SHION
	//========================================
	void LoadWorldData(void)
	{
		if (pWldData != NULL) return;

		FILE *pFile = fopen(STAGE_DATA, "r");
		if (pFile == NULL) return;

		//メモリ確保
		AllocWorldData();

		char Text[TXT_MAX] = {}; // 一行分の文字
		while (true)
		{
			//1行読み取り
			fgets(&Text[0], TXT_MAX, pFile);

			//読み込んだ文字列の中にコメントがあるかチェック
			char *pCharPos = strchr(&Text[0], COMMENT);

			//コメントアウト用の文字があったらその文字以降を削除
			if (pCharPos != nullptr)*pCharPos = '\0';

			//タブ消去
			while (Text[0] == '\t')
			{
				char aCodeBackup[TXT_MAX];
				strcpy(&aCodeBackup[0], &Text[0]);//読み込んだ１行を保存する
				strcpy(&Text[0], &aCodeBackup[1]);//頭のタブ文字を外した次からの文字で上書きする
			}

			//読み取り終了
			if (Text[0] == EOF || feof(pFile)) break;

			//ロード
			else if (strncmp(&Text[0], CODE_RECORD, sizeof CODE_RECORD - 1) == 0)
			{
				char *pSprit = strtok(&Text[0], CHR_PAUSE); // 区切り文字までを消す
				CInt planetID = LoadInt(NULL, CHR_PAUSE);   // 惑星番号取得
				CInt StageID = LoadInt(NULL, CHR_PAUSE);    // ステージ番号取得

				WorldData& rWld = pWldData[planetID]; //長いので省略

				rWld.pBestTime[StageID] = LoadFloat(NULL, CHR_PAUSE);//レコード代入

				CInt NumCoin = rWld.pStgRec[StageID].CoinNums = LoadInt(NULL, CHR_PAUSE);//コイン数取得

				if (NumCoin != 0)
				{
					//回収状況場所確保
					rWld.pStgRec[StageID].pGet = new bool[NumCoin];

					for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
					{
						//コイン回収状況取得
						rWld.pStgRec[StageID].pGet[nCntCoin] = LoadInt(NULL, CHR_PAUSE) == 0 ? false : true;
					}
				}
			}
		}

		//ファイルを閉じる
		fclose(pFile);

		//初期情報保存
		RefreshData();
	}

	//========================================
	// レコードファイル書き出し
	// Author：HIRASAWA SHION
	//========================================
	void SaveWorldData(void)
	{
#ifdef _DEBUG
		return;
#endif

		if (pWldData == NULL) return;

		FILE *pFile = fopen(STAGE_DATA, "w");
		if (pFile == NULL) return;

		const char *WORLD_COMMENT = "\n#=====[ %d面 ]\n";

		//ファイルヘッダコメント書き出し
		fprintf(pFile, "#=======================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#各ステージのクリアタイム\n");
		fprintf(pFile, "#Author:HIRASAWA SHION\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#=======================================\n");
		fprintf(pFile, "SET_RECORD\n\n");

		//レコードの説明文
		fprintf(pFile, "#下のフォーマット絶対順守!!\n");
		fprintf(pFile, "#ワールド - ステージ - ベストタイム = コイン数 - n枚目のコインの回収状況(0=未回収  1=回収済み)\n");
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//ワールド名書き出し
			fprintf(pFile, WORLD_COMMENT, nCntPlanet + 1);

			for (int nCntStage = 0; nCntStage < pWldData[nCntPlanet].MaxStage; nCntStage++)
			{
				//レコード記述
				fprintf(pFile, "	%s = %d - %d - %.2f = ",
						CODE_RECORD, nCntPlanet, nCntStage, pWldData[nCntPlanet].pBestTime[nCntStage]);

				if (pWldData[nCntPlanet].pStgRec == NULL)
					fprintf(pFile, "0");
				else
				{
					//コイン数参照
					CInt& NumCoins = pWldData[nCntPlanet].pStgRec[nCntStage].CoinNums;

					//コイン数書き出し
					fprintf(pFile, "%d", NumCoins);

					//回収状況書き出し
					for (int nCntCoin = 0; nCntCoin < NumCoins; nCntCoin++)
					{
						fprintf(pFile, " - %d", pWldData[nCntPlanet].pStgRec[nCntStage].pGet[nCntCoin] ? 1 : 0);
					}
				}

				fprintf(pFile, "\n");
			}
		}

		//終了
		fprintf(pFile, "\n%s", END_RECORD);
		fclose(pFile);
	}

	//========================================
	//ワールド/ステージ情報を塗り替え
	// Author：HIRASAWA SHION
	//========================================
	void RefreshData(void)
	{
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//長いので省略
			WorldData& rWld = pWldData[nCntPlanet];

			for (int nCntStage = 0; nCntStage < rWld.MaxStage; nCntStage++)
			{
				//コイン数取得
				CInt& NumCoin = rWld.pStgRec[nCntStage].CoinNums;

				//０なら引き返す
				if (NumCoin == 0) continue;

				//コイン数と回収状況保存場所確保
				rWld.pStart[nCntStage].CoinNums = NumCoin;
				rWld.pStart[nCntStage].pGet = new bool[NumCoin];

				for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
				{
					//回収状況代入
					rWld.pStart[nCntStage].pGet[nCntCoin] = rWld.pStgRec[nCntStage].pGet[nCntCoin];
				}
			}
		}
	}
}

//========================================
// 指定されたステージのベストタイムを返す
// Author：HIRASAWA SHION
//========================================
float Stage::GetBestTime(CInt& planetIdx, CInt& stageIdx)
{
	//レコード読込
	LoadWorldData();

	//読み込めたらベストタイムを返す
	if (pWldData != NULL)
		return pWldData[planetIdx].pBestTime[stageIdx];

	//失敗なら大きい数字を返す
	return 10000.0f;
}

//========================================
// タイム更新
// Author：HIRASAWA SHION
//========================================
void Stage::RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime)
{
	LoadWorldData();

	//タイム更新
	if (ClearTime < pWldData[planetIdx].pBestTime[stageIdx])
		pWldData[planetIdx].pBestTime[stageIdx] = ClearTime;
}

//========================================
// ステージ情報を取得
// Author：HIRASAWA SHION
//========================================
Stage::Data Stage::GetData(CInt& planetIdx, CInt& stageIdx)
{
	LoadWorldData();

	//指定されたステージの情報を返す
	return pWldData[planetIdx].pStgRec[stageIdx];
}

//========================================
// コイン回収状況を取得
// Author：HIRASAWA SHION
//========================================
bool Stage::GetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID)
{
	LoadWorldData();

	if (pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL)
	{
		RNLib::Window().Message_ERROR(String("このステージにはコインは配置されていません。"));
		return false;
	}
	else if (coinID >= pWldData[planetIdx].pStgRec[stageIdx].CoinNums) {
		RNLib::Window().Message_ERROR(String("コインの枚数が実際の配置数とずれています。\nID:%d 枚数:%d", coinID, pWldData[planetIdx].pStgRec[stageIdx].CoinNums));
		return false;
	}

	//回収状況を返す
	return pWldData[planetIdx].pStgRec[stageIdx].pGet[coinID];
}

//========================================
// 外部ファイルよりすでに回収しているコインの総数を返す
// Author：HIRASAWA SHION
//========================================
int Stage::GetCoinAll(void)
{
	LoadWorldData();

	int nNumAll = 0;

	for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
	{
		//長いので省略
		WorldData& rWld = pWldData[nCntPlanet];

		for (int nCntStage = 0; nCntStage < rWld.MaxStage; nCntStage++)
		{
			//コイン数取得
			CInt& NumCoin = rWld.pStgRec[nCntStage].CoinNums;

			//０なら引き返す
			if (NumCoin == 0) continue;

			for (int nCntCoin = 0; nCntCoin < NumCoin; nCntCoin++)
			{
				//回収状況代入
				if (rWld.pStgRec[nCntStage].pGet[nCntCoin])nNumAll++;
			}
		}
	}

	//既に回収している総数を返す
	return nNumAll;
}

//========================================
// コイン回収状況を設定
// Author：HIRASAWA SHION
//========================================
void  Stage::SetCoinInfo(CInt& planetIdx, CInt& stageIdx, const Data& data)
{
	LoadWorldData();

	//コイン数が違っていたら設定しない
	if (pWldData[planetIdx].pStgRec[stageIdx].CoinNums != data.CoinNums ||
		pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL) return;

	for (int nCntData = 0; nCntData < data.CoinNums; nCntData++)
	{
		//回収状況代入
		pWldData[planetIdx].pStgRec[stageIdx].pGet[nCntData] = data.pGet[nCntData];
	}
}

//========================================
// コイン回収状況を設定
// Author：HIRASAWA SHION
//========================================
void Stage::SetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID, const bool& bGet)
{
	LoadWorldData();

	//コイン数が違っていたら設定しない
	if (pWldData[planetIdx].pStgRec[stageIdx].CoinNums <= coinID ||
		pWldData[planetIdx].pStgRec[stageIdx].pGet == NULL) return;

	//回収状況代入
	pWldData[planetIdx].pStgRec[stageIdx].pGet[coinID] = bGet;
}