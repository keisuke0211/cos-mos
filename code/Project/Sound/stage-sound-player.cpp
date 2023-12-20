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
// ���nSE
static const char* sc_blockLandingSE[(int)CBlock::LOOKS_TYPE::MAX] = {
	"data\\SOUND\\SE\\landing\\soil.wav",	//[0] �y�u���b�N
	"data\\SOUND\\SE\\landing\\grass.wav",	//[1] ���̐������y�u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	//[2] �Ñ�̐΃u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	//[3] �Ñ�̐΃u���b�NA
	"data\\SOUND\\SE\\landing\\ground.wav",	//[4] �Ñ�̐΃u���b�NB
	"data\\SOUND\\SE\\landing\\ground.wav",	//[5] �Ñ�̐΃u���b�NC
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[6] ���̏�
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[7] ���̌q��
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[8] ���̍��{
	"data\\SOUND\\SE\\landing\\grass.wav",	//[9] ���u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[10] �؂̊�
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[11] �؂̍���
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[12] �o�I�o�u�̖�(�w�i�p)
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[13] ��
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[14] �R�ς݂̃R�C��
	"data\\SOUND\\SE\\landing\\ground.wav",	//[15] ��̐Δ�
	"data\\SOUND\\SE\\landing\\ground.wav",	//[16] �Ñ�̐΃����K�u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[17] �y���Ñ�̐΃u���b�N(���ԗp)
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[18] ���V�̖�
	"data\\SOUND\\SE\\landing\\ground.wav",	//[19] �O�ڂ̃u���b�N
	"data\\SOUND\\SE\\landing\\grass.wav",	//[20] ���u���b�N(�؂̎�)
	"data\\SOUND\\SE\\landing\\ground.wav",	//[21] �k�C�̉ƃu���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	//[22] �A�X�t�@���g�u���b�N
	"data\\SOUND\\SE\\landing\\ground.wav",	//[23] �Ђъ��ꂽ�A�X�t�@���g�u���b�N
	"data\\SOUND\\SE\\landing\\iron.wav",	//[24] �S�i�q(�����蔻�薳���z��)
	"data\\SOUND\\SE\\landing\\iron.wav",	//[25] �S�i�q1
	"data\\SOUND\\SE\\landing\\iron.wav",	//[26] �S�i�q2
	"data\\SOUND\\SE\\landing\\iron.wav",	//[27] �S�i�q3
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[28] �D�u���b�N
	"data\\SOUND\\SE\\landing\\jamp.wav",	//[29] �����A�X�t�@���g�u���b�N(���ԗp)
	"data\\SOUND\\SE\\landing\\sand.wav",	//[30] ���u���b�N
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
// �ϐ���`
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

	RNLib::Text2D().PutDebugLog(String("�ŏI�t���[��%d", f));

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
// �J�n����
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
// �I������
//========================================
void StageSoundPlayer::End(void) {

	// �Đ��I�u�W�F�N�g�̔j��
	RNLib::Sound().GetPlay(swapBGMPlayID).Delete();
	RNLib::Sound().GetPlay(loopPlayID).Delete();
	RNLib::Sound().GetPlay(BGMPlayID).Delete();
}

//========================================
// �u���b�N�̒��nSE�Đ�����
//========================================
void StageSoundPlayer::PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType) {

	RNLib::Sound().Play(blockLandingSEIdxes[(int)looksType], _RNC_Sound::CATEGORY::SE, 1.0f, false);
}