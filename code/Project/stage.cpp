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
#define  MAX_BUBBLE		(7)
#define  MAX_BUBBLECNT	(640)
//****************************************
// 無名空間
//****************************************
namespace {
	//========== [[[ 関数宣言 ]]]
	void PutBackGround(void);
	void ClearRecord(void);
	void AllocRecord(void);
	void LoadRecord(void);
	void SaveRecord(void);
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
	bool            isCutIn;
	short           wallModelIdxes[2];
	CCamera*        UICamera[2];
	CDoll3D*        UIDoll[2];

	//ステージクリアタイムの保存場所
	struct Record
	{
		int MaxStage;     //ステージ数
		float *pBestTime; //各ステージのベストタイム
	};
	Record *pRecord; //惑星ごとのレコード
	int MaxPlanet;   //最大惑星数

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
void Stage::SetRocketPartsUI(CRocketPartsUI* parts)                           { rocketparts = parts; }
void Stage::SetIsCutIn      (const bool& setIsCutIn)                          { isCutIn     = setIsCutIn; }

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
	// ブロックの読み込み処理
	CBlock::Load();

	// 環境音プレイヤーの初期化処理
	StageSoundPlayer::Init();

	MaxPlanet = 0;
	ClearRecord();
}

//========================================
// 終了処理
//========================================
void Stage::Uninit(void)
{
	// 環境音プレイヤーの終了処理
	StageSoundPlayer::Uninit();

	//レコード記録
	SaveRecord();

	//メモリ開放
	ClearRecord();
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

	for (int cnt = 0; cnt < 2; cnt++) {
		{// [[[ UI用カメラの生成 ]]]
			UICamera[cnt] = new CCamera(Size2D(200.0f, RNLib::Window().GetHeight()));

			// クリッピングをオン
			UICamera[cnt]->SetIsClipping(true);

			// 背景色設定
			UICamera[cnt]->SetBGCol(Color(0, 0, 0, 100));

			// ライトを設定
			UICamera[cnt]->SetLightID(Manager::GetLightIdx(0));
		}

		{// [[[ UI用ドールを生成 ]]]
			UIDoll[cnt] = new CDoll3D(PRIORITY_OBJECT, RNLib::SetUp3D().Load(cnt == 0 ? "data\\SETUP\\Player_Mouth.txt" : "data\\SETUP\\Player_Eye.txt"));

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
}

//========================================
// ステージ更新処理
//========================================
void Stage::UpdateStage(void) {

	// 環境音プレイヤーの更新処理
	StageSoundPlayer::Update();

	// ウィンドウ情報を取得
	const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
	const float windowWidth       = RNLib::Window().GetWidth();
	const float windowHeight      = RNLib::Window().GetHeight();
	const float windowHeightHalf  = windowHeight * 0.5f;
	const float windowHeightHalf2 = windowHeightHalf * 0.5f;

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (CheckStageNumber(0, 0)) {
			// 下カメラ描画
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

		// UIカメラ描画
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
	RNLib::StaticMesh().Delete(false);

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

		if (Stage::CheckPlanetIdx(0))
		{// [[[ 背景描画 ]]]

			// 上
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

			// 雲
			for (int nCnt = 0; nCnt < MAX_CLOUD; nCnt++)
			{
				RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
					->SetTex(CResources::TEXTURE_IDXES[cloudtex[nCnt]])
					->SetVtxPos(Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z), Pos3D(cloudpos[nCnt].x + 200.0f, cloudpos[nCnt].y - 100.0f + 32.0f, cloudpos[nCnt].z))
					->SetBillboard(true)
					->SetZTest(false)
					->SetCol(Color{ 255,255,255,100 });

				cloudpos[nCnt].x += cloudmove[nCnt];	// 移動量の増加

				if (cloudpos[nCnt].x >= 550.0f)
				{
					cloudpos[nCnt] = cloudpos[nCnt] = Pos3D(-500.0f + rand() % 200 - 200, 200.0f, 200.0f + rand() % 200 - 100);
				}
			}


			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// [[[ 壁モデル描画 ]]]
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
			RNLib::Model().Put(PRIORITY_BACKGROUND, wallModelIdxes[1], Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
		}
		if (Stage::CheckPlanetIdx(1))
		{// [[[ 背景描画 ]]]

			// 上
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_OCEAN])
				->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// 下
			RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
				->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CITY], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
				->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
				->SetBillboard(true)
				->SetInterpolationMode(_RNC_DrawState::INTERPOLATION_MODE::LINEAR);

			// 魚
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
// レコードのメモリ開放
// Author：HIRASAWA SHION
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
// レコードのメモリ確保
// Author：HIRASAWA SHION
//========================================
namespace
{
	void AllocRecord(void)
	{
		//ステージエディター取得
		CStageEditor *pEd = Manager::StgEd();

		//惑星の総数取得
		MaxPlanet = pEd->GetPlanetMax();

		//惑星の数だけメモリ確保
		pRecord = new Record[MaxPlanet];

		for (int nCntStage = 0; nCntStage < MaxPlanet; nCntStage++)
		{
			//指定された惑星のステージ数を取得
			CInt MaxStage = pRecord[nCntStage].MaxStage = pEd->GetType()[nCntStage].nStageMax;

			//ステージ数分のレコード場所確保
			pRecord[nCntStage].pBestTime = new float[MaxStage];
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
	LoadRecord();

	//読み込めたらベストタイムを返す
	if (pRecord != NULL)
		return pRecord[planetIdx].pBestTime[stageIdx];

	//失敗なら大きい数字を返す
	return 10000.0f;
}

//========================================
// タイム更新
// Author：HIRASAWA SHION
//========================================
void Stage::RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime)
{
	LoadRecord();

	//タイム更新
	if (ClearTime < pRecord[planetIdx].pBestTime[stageIdx])
		pRecord[planetIdx].pBestTime[stageIdx] = ClearTime;
}

//========================================
// レコードファイル読み込み
// Author：HIRASAWA SHION
//========================================
namespace 
{
	void LoadRecord(void)
	{
		if (pRecord != NULL) return;

		FILE *pFile = fopen("data\\GAMEDATA\\STAGE\\CLEAR_TIME.txt", "r");
		if (pFile == NULL) return;

		//メモリ確保
		AllocRecord();

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
				pRecord[planetID].pBestTime[StageID] = LoadFloat(NULL, CHR_PAUSE);//レコード代入
			}
		}

		fclose(pFile);
	}
}

//========================================
// レコードファイル書き出し
// Author：HIRASAWA SHION
//========================================
namespace 
{
	void SaveRecord(void)
	{
		if (pRecord == NULL) return;

		FILE *pFile = fopen("data\\GAMEDATA\\STAGE\\CLEAR_TIME.txt", "w");
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
		fprintf(pFile, "#ワールド - ステージ - ベストタイム");
		for (int nCntPlanet = 0; nCntPlanet < MaxPlanet; nCntPlanet++)
		{
			//ワールド名書き出し
			fprintf(pFile, WORLD_COMMENT, nCntPlanet + 1);

			for (int nCntStage = 0; nCntStage < pRecord[nCntPlanet].MaxStage; nCntStage++)
			{
				//レコード記述
				fprintf(pFile, "	%s = %d - %d - %.2f\n", 
						CODE_RECORD, nCntPlanet, nCntStage, pRecord[nCntPlanet].pBestTime[nCntStage]);
			}
		}

		//終了
		fprintf(pFile, "\n%s", END_RECORD);
		fclose(pFile);
	}
}