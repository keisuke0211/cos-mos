//========================================
// 
// �X�e�[�W�v���C���[����
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "stage-sound-player.h"
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
// BGM/����
#define AMBIENT_SE_1_MAX (2)
#define AMBIENT_SE_1_LOOP_PATH "NONEDATA"
#define AMBIENT_SE_1_LOOP_VOLUME (0.5f)
#define BGM_1_PATH "data\\SOUND\\BGM\\STAGE1.wav"
#define BGM_1_VOLUME (1.0f)
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
// ���nSE
static const char* sc_blockLandingSE[(int)CBlock::LOOKS_TYPE::MAX] = {
	"data\\SOUND\\SE\\landing\\soil.wav",	// �y�u���b�N
	"data\\SOUND\\SE\\landing\\grass.wav",	// ���̐������y�u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ñ�̐΃u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ñ�̐΃u���b�NA
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ñ�̐΃u���b�NB
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ñ�̐΃u���b�NC
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���̏�
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���̌q��
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���̍��{
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �؂̊�
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �؂̍���
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �o�I�o�u�̖�(�w�i�p)
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ��
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �R�ς݂̃R�C��
	"data\\SOUND\\SE\\landing\\ground.wav",	// ��̐Δ�
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ñ�̐΃����K�u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �y���Ñ�̐΃u���b�N(���ԗp)
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���V�̖�
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �O�ڂ̃u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	// ���u���b�N(�؂̎�)
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �k�C�̉ƃu���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	// �A�X�t�@���g�u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	// �Ђъ��ꂽ�A�X�t�@���g�u���b�N
	"data\\SOUND\\SE\\landing\\iron.wav",	// �S�i�q(�����蔻�薳���z��)
	"data\\SOUND\\SE\\landing\\iron.wav",	// �S�i�q1
	"data\\SOUND\\SE\\landing\\iron.wav",	// �S�i�q2
	"data\\SOUND\\SE\\landing\\iron.wav",	// �S�i�q3
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �D�u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	// �����A�X�t�@���g�u���b�N(���ԗp)
	"data\\SOUND\\SE\\landing\\sand.wav",	// ���u���b�N
};

//****************************************
// �ϐ���`
//****************************************
static short ambientSE1Idxes[AMBIENT_SE_1_MAX];
static short ambientSE1Counter[AMBIENT_SE_1_MAX];
static short ambientSE2Idxes[AMBIENT_SE_2_MAX];
static short ambientSE2Counter[AMBIENT_SE_2_MAX];
static short loopPlayID;
static short BGMPlayID;
static short blockLandingSEIdxes[(int)CBlock::LOOKS_TYPE::MAX];

//========================================
// ����������
//========================================
void StageSoundPlayer::Init(void) {

	// �T�E���h�̓ǂݍ���&�J�E���^�[
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
// �I������
//========================================
void StageSoundPlayer::Uninit(void) {

}

//========================================
// �X�V����
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
// �J�n����
//========================================
void StageSoundPlayer::Start(void) {

	const int planet = Manager::StgEd()->GetPlanetIdx();
	if (planet == 0) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_1_LOOP_PATH), CSound::CATEGORY::SE, AMBIENT_SE_1_LOOP_VOLUME, true);
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_1_PATH), CSound::CATEGORY::BGM, BGM_1_VOLUME, true);
	}
	else if (planet == 1) {
		loopPlayID = RNLib::Sound().Play(RNLib::Sound().Load(AMBIENT_SE_2_LOOP_PATH), CSound::CATEGORY::SE, AMBIENT_SE_2_LOOP_VOLUME, true);
		BGMPlayID = RNLib::Sound().Play(RNLib::Sound().Load(BGM_2_PATH), CSound::CATEGORY::BGM, BGM_2_VOLUME, true);
	}
	else {
		loopPlayID = NONEDATA;
		BGMPlayID = NONEDATA;
	}
}

//========================================
// �I������
//========================================
void StageSoundPlayer::End(void) {

	// �Đ��I�u�W�F�N�g�̔j��
	RNLib::Sound().GetPlay(loopPlayID).Delete();
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}

//========================================
// �u���b�N�̒��nSE�Đ�����
//========================================
void StageSoundPlayer::PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType) {

	RNLib::Sound().Play(blockLandingSEIdxes[(int)looksType], CSound::CATEGORY::SE, 1.0f, false);
}