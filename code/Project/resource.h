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
		BG_OCEAN,
		BG_CITY,
		BG_CLOUD,
		UI_WAITBUBBLE,
		CHR_BLACK_EYE,
		MAX,
	};
	static short TEXTURE_IDXES[(int)TEXTURE::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];

	// サウンド
	// CResources::SOUND[(int)CResources::SOUND]
	// RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::], CSound::CATEGORY::, 1.0f, );
	enum class SOUND {
		OK,				//「OK」音声
		SELECT,			// 決定音

		//プレイヤー関連
		JUMP,			// ジャンプ
		LANDING,		// 着地
		SWAPING,		// 移動中
		SWAPEND,		// スワップ終わり
		EXPAND,			// 膨らむ
		EXPLPSOON,		// 破裂
		DOG_00,			// 押す
		DOG_01,			// 縮む
		DOG_02,			// 伸びる
		DOG_03,			// 震える

		GET_PARTS,		//ロケットパーツ取得
		GET_COIN,		//コイン取得
		MAX,
	};
	static short SOUND_IDXES[(int)SOUND::MAX];
	static const char* SOUND_PATHS[(int)SOUND::MAX];

	//========== [[[ 読み込み ]]]
	static void Load(void) {
		for (int cnt = 0; cnt < (int)TEXTURE::MAX; TEXTURE_IDXES[cnt] = RNLib::Texture().Load(TEXTURE_PATHS[cnt]), cnt++);
		for (int cnt = 0; cnt < (int)SOUND  ::MAX; SOUND_IDXES  [cnt] = RNLib::Sound  ().Load(SOUND_PATHS  [cnt]), cnt++);
	}
};