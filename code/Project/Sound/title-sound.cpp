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

// �^�C�g��
#define BGM_PATH "data\\SOUND\\BGM\\SELECT.wav"
#define BGM_VOLUME (0.5f)

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
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}
