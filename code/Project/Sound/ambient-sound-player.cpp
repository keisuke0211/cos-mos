//========================================
// 
// �����v���C���[����
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "ambient-sound-player.h"
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

//****************************************
// �萔��`
//****************************************
#define AMBIENT_SE_1_MAX (2)
static const AmbientSEInfo sc_ambientSE1Infos[AMBIENT_SE_1_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\bard.wav", 200, 500, 0.5f, 0.5f },
	{ "data\\SOUND\\SE\\AmbientSound\\shake.wav", 100, 300, 0.5f, 0.5f },
};
#define AMBIENT_SE_2_MAX (1)
static const AmbientSEInfo sc_ambientSE2Infos[AMBIENT_SE_2_MAX] = {
	{ "data\\SOUND\\SE\\AmbientSound\\shake.wav", 100, 300, 0.5f, 0.5f },
};

//****************************************
// �ϐ���`
//****************************************
static short ambientSE1Idxes[AMBIENT_SE_1_MAX];
static short ambientSE1Counter[AMBIENT_SE_1_MAX];
static short ambientSE2Idxes[AMBIENT_SE_2_MAX];
static short ambientSE2Counter[AMBIENT_SE_2_MAX];

//========================================
// ����������
//========================================
void AmbientSoundPlayer::Init(void) {

	// �T�E���h�̓ǂݍ���&�J�E���^�[
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
// �I������
//========================================
void AmbientSoundPlayer::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void AmbientSoundPlayer::Update(void) {

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