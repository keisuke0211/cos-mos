//========================================
// 
// タイトルSE処理
// Author:SYUUKI IIDA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "title-sound.h"
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

// タイトル
#define BGM_PATH "data\\SOUND\\BGM\\SELECT.wav"
#define BGM_VOLUME (0.5f)

//****************************************
// 変数定義
//****************************************
static short BGMPlayID;

void titleSound::Init(void)
{

}

void titleSound::Uninit(void)
{

}

void titleSound::Update(void)
{

}

void titleSound::Start(void)
{
	BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_PATH), _RNC_Sound::CATEGORY::BGM, BGM_VOLUME, true);
}

void titleSound::End(void)
{
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}
