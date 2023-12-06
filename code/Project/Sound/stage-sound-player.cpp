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
#define AMBIENT_SE_1_MAX (2)
#define AMBIENT_SE_1_LOOP_PATH "NONEDATA"
#define AMBIENT_SE_1_LOOP_VOLUME (0.5f)
#define BGM_1_PATH "data\\SOUND\\BGM\\STAGE1.wav"
#define BGM_1_VOLUME (0.5f)
static const AmbientSEInfo sc_ambientSE1Infos[AMBIENT_SE_1_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\bard.wav", 200, 500, 0.5f, 0.5f },
	{ "data\\SOUND\\SE\\AmbientSound\\shake.wav", 100, 300, 0.5f, 0.5f },
};
#define AMBIENT_SE_2_MAX (2)
#define AMBIENT_SE_2_LOOP_PATH "data\\SOUND\\SE\\AmbientSound\\bubble.wav"
#define AMBIENT_SE_2_LOOP_VOLUME (0.5f)
#define BGM_2_PATH "data\\SOUND\\BGM\\STAGE2.wav"
#define BGM_2_VOLUME (0.5f)
static const AmbientSEInfo sc_ambientSE2Infos[AMBIENT_SE_2_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\whale.wav", 300, 900, 0.5f, 0.5f },
	{ "data\\SOUND\\SE\\AmbientSound\\dolphin.wav", 600, 600, 0.25f, 0.25f },
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

	const int planet = Manager::StgEd()->GetPlanetIdx();

	if (planet == 0) {
		for (int cnt = 0; cnt < AMBIENT_SE_1_MAX; cnt++) {
			if (--ambientSE1Counter[cnt] <= 0) {
				ambientSE1Counter[cnt] = sc_ambientSE1Infos[cnt].spanMin + rand() % sc_ambientSE1Infos[cnt].spanAdd;
				RNLib::Sound().Play(ambientSE1Idxes[cnt], CSound::CATEGORY::SE, sc_ambientSE1Infos[cnt].volumeMin + fRand() * sc_ambientSE1Infos[cnt].volumeAdd, false);
			}
		}
	}
	else if (planet == 1) {
		for (int cnt = 0; cnt < AMBIENT_SE_2_MAX; cnt++) {
			if (--ambientSE2Counter[cnt] <= 0) {
				ambientSE2Counter[cnt] = sc_ambientSE2Infos[cnt].spanMin + rand() % sc_ambientSE2Infos[cnt].spanAdd;
				RNLib::Sound().Play(ambientSE2Idxes[cnt], CSound::CATEGORY::SE, sc_ambientSE2Infos[cnt].volumeMin + fRand() * sc_ambientSE2Infos[cnt].volumeAdd, false);
			}
		}
	}
}

//========================================
// 開始処理
//========================================
void StageSoundPlayer::Start(void) {

	const int planet = Manager::StgEd()->GetPlanetIdx();
	if (planet == 0) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_1_LOOP_PATH), CSound::CATEGORY::SE, AMBIENT_SE_1_LOOP_VOLUME, true);
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_1_PATH), CSound::CATEGORY::SE, BGM_1_VOLUME, true);
	}
	else if (planet == 1) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_2_LOOP_PATH), CSound::CATEGORY::SE, AMBIENT_SE_2_LOOP_VOLUME, true);
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_2_PATH), CSound::CATEGORY::SE, BGM_2_VOLUME, true);
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
	RNLib::Sound().GetPlay(loopPlayID).Delete();
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}