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
		BG_WILDERNESS,
		BG_FOREST,
		BG_CAVE,
		UI_WAITBUBBLE,
		MAX,
	};
	static short TEXTURE_IDXES[(int)TEXTURE::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];

	// �T�E���h
	// CResources::SOUND[(int)CResources::SOUND]
	// RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::], CSound::CATEGORY::, 1.0f, );
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
		MAX,
	};
	static short SOUND_IDXES[(int)SOUND::MAX];
	static const char* SOUND_PATHS[(int)SOUND::MAX];

	//========== [[[ �ǂݍ��� ]]]
	static void Load(void) {
		for (int cnt = 0; cnt < (int)TEXTURE::MAX; TEXTURE_IDXES[cnt] = RNLib::Texture().Load(TEXTURE_PATHS[cnt]), cnt++);
		for (int cnt = 0; cnt < (int)SOUND::MAX; SOUND_IDXES[cnt] = RNLib::Sound().Load(SOUND_PATHS[cnt]), cnt++);
	}
};