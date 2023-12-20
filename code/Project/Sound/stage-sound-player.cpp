//========================================
// 
// ステージプレイヤー処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "stage-sound-player.h"
#include "../manager.h"

//****************************************
// 構造体定義
//****************************************
struct AmbientSEInfo {
	char* path;			// サウンドのパス
	UShort spanMin;		// 間隔の下限
	UShort spanAdd;		// 間隔の加算値
	float volumeMin;	// 音量の下限
	float volumeAdd;	// 音量の加算値
};

//****************************************
// 定数定義
//****************************************
// BGM/環境音
#define SWAP_BGM_PATH      "data\\SOUND\\BGM\\SWAP.wav"
// World1
#define AMBIENT_SE_1_MAX (2)
#define AMBIENT_SE_1_LOOP_PATH "NONEDATA"
#define AMBIENT_SE_1_LOOP_VOLUME (0.5f)
#define BGM_1_PATH "data\\SOUND\\BGM\\STAGE1.wav"
#define BGM_1_VOLUME (0.5f)
static const AmbientSEInfo sc_ambientSE1Infos[AMBIENT_SE_1_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\bard.wav", 200, 500, 0.5f, 0.5f },
	{ "data\\SOUND\\SE\\AmbientSound\\shake.wav", 100, 300, 0.5f, 0.5f },
};
// World2
#define AMBIENT_SE_2_MAX (2)
#define AMBIENT_SE_2_LOOP_PATH "data\\SOUND\\SE\\AmbientSound\\bubble.wav"
#define AMBIENT_SE_2_LOOP_VOLUME (0.5f)
#define BGM_2_PATH "data\\SOUND\\BGM\\STAGE2.wav"
#define BGM_2_VOLUME (0.5f)
static const AmbientSEInfo sc_ambientSE2Infos[AMBIENT_SE_2_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\whale.wav", 300, 900, 0.5f, 0.5f },
	{ "data\\SOUND\\SE\\AmbientSound\\dolphin.wav", 600, 600, 0.25f, 0.25f },
};
// 着地SE
static const char* sc_blockLandingSE[(int)CBlock::LOOKS_TYPE::MAX] = {
	"data\\SOUND\\SE\\landing\\soil.wav",	//[0] 土ブロック
	"data\\SOUND\\SE\\landing\\grass.wav",	//[1] 草の生えた土ブロック
	"data\\SOUND\\SE\\landing\\ground.wav",	//[2] 古代の石ブロック
	"data\\SOUND\\SE\\landing\\ground.wav",	//[3] 古代の石ブロックA
	"data\\SOUND\\SE\\landing\\ground.wav",	//[4] 古代の石ブロックB
	"data\\SOUND\\SE\\landing\\ground.wav",	//[5] 古代の石ブロックC
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[6] 柱の上
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[7] 柱の繋ぎ
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[8] 柱の根本
	"data\\SOUND\\SE\\landing\\grass.wav",	//[9] 草ブロック
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[10] 木の幹
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[11] 木の根元
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[12] バオバブの木(背景用)
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[13] 宝箱
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[14] 山積みのコイン
	"data\\SOUND\\SE\\landing\\ground.wav",	//[15] 謎の石碑
	"data\\SOUND\\SE\\landing\\ground.wav",	//[16] 古代の石レンガブロック
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[17] 土＆古代の石ブロック(中間用)
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[18] ヤシの木
	"data\\SOUND\\SE\\landing\\ground.wav",	//[19] 三つ目のブロック
	"data\\SOUND\\SE\\landing\\grass.wav",	//[20] 草ブロック(木の実)
	"data\\SOUND\\SE\\landing\\ground.wav",	//[21] ヌイの家ブロック
	"data\\SOUND\\SE\\landing\\ground.wav",	//[22] アスファルトブロック
	"data\\SOUND\\SE\\landing\\ground.wav",	//[23] ひび割れたアスファルトブロック
	"data\\SOUND\\SE\\landing\\iron.wav",	//[24] 鉄格子(当たり判定無し想定)
	"data\\SOUND\\SE\\landing\\iron.wav",	//[25] 鉄格子1
	"data\\SOUND\\SE\\landing\\iron.wav",	//[26] 鉄格子2
	"data\\SOUND\\SE\\landing\\iron.wav",	//[27] 鉄格子3
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[28] 泥ブロック
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[29] 砂＆アスファルトブロック(中間用)
	"data\\SOUND\\SE\\landing\\sand.wav",	//[30] 砂ブロック
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
	"NONEDATA",
};

//****************************************
// 変数定義
//****************************************
static short ambientSE1Idxes[AMBIENT_SE_1_MAX];
static short ambientSE1Counter[AMBIENT_SE_1_MAX];
static short ambientSE2Idxes[AMBIENT_SE_2_MAX];
static short ambientSE2Counter[AMBIENT_SE_2_MAX];
static short loopPlayID;
static short BGMPlayID;
static float BGMVolume;
static float swapBGMVolume;
static short swapBGMPlayID;
static short blockLandingSEIdxes[(int)CBlock::LOOKS_TYPE::MAX];

//========================================
// 初期化処理
//========================================
void StageSoundPlayer::Init(void) {

	// サウンドの読み込み&カウンター
	for (int cnt = 0; cnt < AMBIENT_SE_1_MAX; cnt++) {
		ambientSE1Idxes[cnt] = RNLib::Sound().Load(sc_ambientSE1Infos[cnt].path);
		ambientSE1Counter[cnt] = sc_ambientSE1Infos[cnt].spanMin + rand() % sc_ambientSE1Infos[cnt].spanAdd;
	}
	for (int cnt = 0; cnt < AMBIENT_SE_2_MAX; cnt++) {
		ambientSE2Idxes[cnt] = RNLib::Sound().Load(sc_ambientSE2Infos[cnt].path);
		ambientSE2Counter[cnt] = sc_ambientSE2Infos[cnt].spanMin + rand() % sc_ambientSE2Infos[cnt].spanAdd;
	}
	for (int cnt = 0; cnt < (int)CBlock::LOOKS_TYPE::MAX; cnt++) {
		blockLandingSEIdxes[cnt] = RNLib::Sound().Load(sc_blockLandingSE[cnt]);
	}
}

//========================================
// 終了処理
//========================================
void StageSoundPlayer::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void StageSoundPlayer::Update(void) {

	if (CPlayer::GetIsSwapWait()) {
		swapBGMVolume += 0.02f;
		if (swapBGMVolume > 1.0f)
			swapBGMVolume = 1.0f;

		if (   RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 28  - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 55  - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 82  - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 109 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 137 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 163 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 190 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 218 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 246 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 273 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 299 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 327 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 355 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 381 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 409 - 4
			|| RNLib::Sound().GetPlay(swapBGMPlayID).GetCount() == 435 - 4
			) {
			if (CPlayer::GetInfo(0)->swapWaitCounter == 5) {
				RNLib::Input().SetVibration(0.5f, 1);
				RNLib::Input().SetVibration(0.3f, 0);
			}
			else if (CPlayer::GetInfo(1)->swapWaitCounter == 5) {
				RNLib::Input().SetVibration(0.5f, 0);
				RNLib::Input().SetVibration(0.3f, 1);
			}
		}
	}
	else {
		swapBGMVolume -= 0.02f;
		if (swapBGMVolume < 0.0f)
			swapBGMVolume = 0.0f;
	}
	RNLib::Sound().GetPlay(swapBGMPlayID).SetVolume(swapBGMVolume);

	static int f = 0;
	if (RNLib::Input().GetKeyTrigger(DIK_L)) {
		f = RNLib::Sound().GetPlay(swapBGMPlayID).GetCount();
	}

	RNLib::Text2D().PutDebugLog(String("最終フレーム%d", f));

	{
		float setVolume = 0.2f + ((1.0f - swapBGMVolume));
		if (setVolume > 1.0f)
			setVolume = 1.0f;
		RNLib::Sound().GetPlay(BGMPlayID).SetVolume(setVolume * BGMVolume);
	}

	const int planet = Manager::StgEd()->GetPlanetIdx();

	if (planet == 0) {
		for (int cnt = 0; cnt < AMBIENT_SE_1_MAX; cnt++) {
			if (--ambientSE1Counter[cnt] <= 0) {
				ambientSE1Counter[cnt] = sc_ambientSE1Infos[cnt].spanMin + rand() % sc_ambientSE1Infos[cnt].spanAdd;
				RNLib::Sound().Play(ambientSE1Idxes[cnt], _RNC_Sound::CATEGORY::SE, sc_ambientSE1Infos[cnt].volumeMin + RNLib::Number().GetRandomFloat(1.0f) * sc_ambientSE1Infos[cnt].volumeAdd, false);
			}
		}
	}
	else if (planet == 1) {
		for (int cnt = 0; cnt < AMBIENT_SE_2_MAX; cnt++) {
			if (--ambientSE2Counter[cnt] <= 0) {
				ambientSE2Counter[cnt] = sc_ambientSE2Infos[cnt].spanMin + rand() % sc_ambientSE2Infos[cnt].spanAdd;
				RNLib::Sound().Play(ambientSE2Idxes[cnt], _RNC_Sound::CATEGORY::SE, sc_ambientSE2Infos[cnt].volumeMin + RNLib::Number().GetRandomFloat(1.0f) * sc_ambientSE2Infos[cnt].volumeAdd, false);
			}
		}
	}
}

//========================================
// 開始処理
//========================================
void StageSoundPlayer::Start(void) {

	swapBGMVolume = 0.0f;
	swapBGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(SWAP_BGM_PATH), _RNC_Sound::CATEGORY::BGM, 0.0f, true);

	const int planet = Manager::StgEd()->GetPlanetIdx();
	if (planet == 0) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_1_LOOP_PATH), _RNC_Sound::CATEGORY::SE, AMBIENT_SE_1_LOOP_VOLUME, true);
		BGMVolume = BGM_1_VOLUME;
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_1_PATH), _RNC_Sound::CATEGORY::BGM, BGM_1_VOLUME, true);
	}
	else if (planet == 1) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_2_LOOP_PATH), _RNC_Sound::CATEGORY::SE, AMBIENT_SE_2_LOOP_VOLUME, true);
		BGMVolume = BGM_2_VOLUME;
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_2_PATH), _RNC_Sound::CATEGORY::BGM, BGM_2_VOLUME, true);
	}
	else {
		loopPlayID = NONEDATA;
		BGMPlayID = NONEDATA;
	}
}

//========================================
// 終了処理
//========================================
void StageSoundPlayer::End(void) {

	// 再生オブジェクトの破棄
	RNLib::Sound().GetPlay(swapBGMPlayID).Delete();
	RNLib::Sound().GetPlay(loopPlayID).Delete();
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}

//========================================
// ブロックの着地SE再生処理
//========================================
void StageSoundPlayer::PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType) {

	RNLib::Sound().Play(blockLandingSEIdxes[(int)looksType], _RNC_Sound::CATEGORY::SE, 1.0f, false);
}