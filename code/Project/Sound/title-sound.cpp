//========================================
// 
// �^�C�g��SE����
// Author:SYUUKI IIDA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "title-sound.h"
#include "../manager.h"

//****************************************
// �\���̒�`
//****************************************
struct AmbientSEInfo {
	char* path;			// �T�E���h�̃p�X
	UShort spanMin;		// �Ԋu�̉���
	UShort spanAdd;		// �Ԋu�̉��Z�l
	float volumeMin;	// ���ʂ̉���
	float volumeAdd;	// ���ʂ̉��Z�l
};

// World2
//#define AMBIENT_SE_2_MAX (2)
//#define AMBIENT_SE_2_LOOP_PATH "data\\SOUND\\SE\\AmbientSound\\bubble.wav"
//#define AMBIENT_SE_2_LOOP_VOLUME (0.5f)
#define BGM_PATH "data\\SOUND\\BGM\\SELECT.wav"
#define BGM_VOLUME (0.5f)
//static const AmbientSEInfo sc_ambientSE2Infos[AMBIENT_SE_2_MAX] = {
//	{ "data\\SOUND\\SE\\AmbientSound\\whale.wav", 300, 900, 0.5f, 0.5f },
//	{ "data\\SOUND\\SE\\AmbientSound\\dolphin.wav", 600, 600, 0.25f, 0.25f },
//};

//****************************************
// �ϐ���`
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

}
