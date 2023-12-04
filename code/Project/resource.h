//========================================
// 
// リソースの管理処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
class CResources {
public:
	// テクスチャ
	// CResources::TEXTURE_IDXES[(int)CResources::TEXTURE]
	enum class TEXTURE {
		BG_WILDERNESS,
		BG_FOREST,
		BG_CAVE,
		MAX,
	};
	static short TEXTURE_IDXES[(int)TEXTURE::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];

	// サウンド
	// CResources::SOUND[(int)CResources::SOUND]
	// RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::OK], CSound::CATEGORY::SE, false);
	enum class SOUND {
		OK,
		SELECT,
		MAX,
	};
	static short SOUND_IDXES[(int)SOUND::MAX];
	static const char* SOUND_PATHS[(int)SOUND::MAX];

	//========== [[[ 読み込み ]]]
	static void Load(void) {
		for (int cnt = 0; cnt < (int)TEXTURE::MAX; TEXTURE_IDXES[cnt] = RNLib::Texture().Load(TEXTURE_PATHS[cnt]), cnt++);
		for (int cnt = 0; cnt < (int)SOUND::MAX; SOUND_IDXES[cnt] = RNLib::Sound().Load(SOUND_PATHS[cnt]), cnt++);
	}
};