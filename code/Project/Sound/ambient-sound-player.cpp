//========================================
// 
// 環境音プレイヤー処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "ambient-sound-player.h"

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
#define AMBIENT_SE_MAX (1)
static const AmbientSEInfo sc_ambientSEInfos[AMBIENT_SE_MAX] = {
	{ "RNData\\Sound\\TestSE.wav", 300, 300, 0.0f, 0.0f },
};

//****************************************
// 変数定義
//****************************************
static short ambientSEIdxes[AMBIENT_SE_MAX];
static short ambientSECounter[AMBIENT_SE_MAX];

//========================================
// 初期化処理
//========================================
void AmbientSoundPlayer::Init(void) {

	// サウンドの読み込み&カウンター
	for (int cnt = 0; cnt < AMBIENT_SE_MAX; cnt++) {
		ambientSEIdxes[cnt] = RNLib::Sound().Load(sc_ambientSEInfos[cnt].path);
		ambientSECounter[cnt] = sc_ambientSEInfos[cnt].spanMin + rand() % sc_ambientSEInfos[cnt].spanAdd;
	}
}

//========================================
// 終了処理
//========================================
void AmbientSoundPlayer::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void AmbientSoundPlayer::Update(void) {

	for (int cnt = 0; cnt < AMBIENT_SE_MAX; cnt++) {
		if (--ambientSECounter[cnt] <= 0) {
			ambientSECounter[cnt] = sc_ambientSEInfos[cnt].spanMin + rand() % sc_ambientSEInfos[cnt].spanAdd;
			RNLib::Sound().Play(ambientSEIdxes[cnt], CSound::CATEGORY::SE, false);
		}
	}
}