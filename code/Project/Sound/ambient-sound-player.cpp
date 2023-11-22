//========================================
// 
// �����v���C���[����
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "ambient-sound-player.h"

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

//****************************************
// �萔��`
//****************************************
#define AMBIENT_SE_MAX (1)
static const AmbientSEInfo sc_ambientSEInfos[AMBIENT_SE_MAX] = {
	{ "RNData\\Sound\\TestSE.wav", 300, 300, 0.0f, 0.0f },
};

//****************************************
// �ϐ���`
//****************************************
static short ambientSEIdxes[AMBIENT_SE_MAX];
static short ambientSECounter[AMBIENT_SE_MAX];

//========================================
// ����������
//========================================
void AmbientSoundPlayer::Init(void) {

	// �T�E���h�̓ǂݍ���&�J�E���^�[
	for (int cnt = 0; cnt < AMBIENT_SE_MAX; cnt++) {
		ambientSEIdxes[cnt] = RNLib::Sound().Load(sc_ambientSEInfos[cnt].path);
		ambientSECounter[cnt] = sc_ambientSEInfos[cnt].spanMin + rand() % sc_ambientSEInfos[cnt].spanAdd;
	}
}

//========================================
// �I������
//========================================
void AmbientSoundPlayer::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void AmbientSoundPlayer::Update(void) {

	for (int cnt = 0; cnt < AMBIENT_SE_MAX; cnt++) {
		if (--ambientSECounter[cnt] <= 0) {
			ambientSECounter[cnt] = sc_ambientSEInfos[cnt].spanMin + rand() % sc_ambientSEInfos[cnt].spanAdd;
			RNLib::Sound().Play(ambientSEIdxes[cnt], CSound::CATEGORY::SE, false);
		}
	}
}