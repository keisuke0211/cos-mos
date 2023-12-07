//========================================
// 
// ステージプレイヤー処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Object/Block/block.h"

//****************************************
// 名前空間
//****************************************
namespace StageSoundPlayer {
	
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Start(void);
	void End(void);
	void PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType);
};