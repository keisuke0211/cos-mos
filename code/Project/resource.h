//========================================
// 
// ���\�[�X�̊Ǘ�����
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../_RNLib/RNlib.h"

//****************************************
// �N���X��`
//****************************************
class CResources {
public:
	// �e�N�X�`��
	// CResources::TEXTURE_IDXES[(int)CResources::TEXTURE]
	enum class TEXTURE {
		BG_SPACE00,
		BG_SPACE01,
		BG_SPACE02,
		BG_WILDERNESS,
		BG_FOREST,
		BG_CAVE,
		BG_OCEAN,
		BG_CITY,
		BG_CLOUD_A,
		BG_CLOUD_B,
		BG_CLOUD_C,
		UI_WAITBUBBLE,
		CHR_BLACK_EYE,
		BG_FISH,
		BG_BUBBLE,
		UI_FRAME,
		UI_CONTROL_0,
		UI_CONTROL_1,
		EFF_CIRCLE,
		MAX,
	};
	static short TEXTURE_IDXES[(int)TEXTURE::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];

	// �T�E���h
	// CResources::SOUND[(int)CResources::SOUND]
	// RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::], _RNC_Sound::CATEGORY::, 1.0f,false );
	enum class SOUND {
		OK,				//�uOK�v����
		SELECT,			// ���艹

		//�v���C���[�֘A
		JUMP,			// �W�����v
		LANDING,		// ���n
		SWAPING,		// �ړ���
		SWAPEND,		// �X���b�v�I���
		EXPAND,			// �c���
		EXPLPSOON,		// �j��
		DOG_00,			// ����
		DOG_01,			// �k��
		DOG_02,			// �L�т�
		DOG_03,			// �k����

		GET_PARTS,		//���P�b�g�p�[�c�擾
		GET_COIN,		//�R�C���擾�@�i���j
		GATE_CLOSE,		//���[�v�Q�[�g������Ƃ��̉�
		TRAMPOLINE_BOUND,
		PILE_DIG,
		ROCKET_RIDE,
		ROCKET_DEPARTURE,
		ROCKET_FIRE,
		EXPLOSION,
		ROCKET_MOVE,

		TIME_COUNT,		//�^�C���J�E���g
		TIME_OVER,		//�^�C���I�[�o�[
		MAX,
	};
	static short SOUND_IDXES[(int)SOUND::MAX];
	static const char* SOUND_PATHS[(int)SOUND::MAX];

	//========== [[[ �ǂݍ��� ]]]
	static void Load(void) {
		for (int cnt = 0; cnt < (int)TEXTURE::MAX; TEXTURE_IDXES[cnt] = RNLib::Texture().Load(TEXTURE_PATHS[cnt]), cnt++);
		for (int cnt = 0; cnt < (int)SOUND  ::MAX; SOUND_IDXES  [cnt] = RNLib::Sound  ().Load(SOUND_PATHS  [cnt]), cnt++);
	}
};