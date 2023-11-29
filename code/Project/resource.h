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
		MAX,
	};
	static short TEXTURE_IDXES[(int)TEXTURE::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];

	//========== [[[ �ǂݍ��� ]]]
	static void Load(void) {
		for (int cnt = 0; cnt < (int)TEXTURE::MAX; TEXTURE_IDXES[cnt] = RNLib::Texture().Load(TEXTURE_PATHS[cnt]), cnt++);
	}
};