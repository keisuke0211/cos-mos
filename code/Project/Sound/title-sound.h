//========================================
// 
// タイトルSE処理
// Author:SYUUKI IIDA
// 
//========================================
#pragma once

#include "../Object/Block/block.h"

//****************************************
// 名前空間
//****************************************
namespace titleSound {

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Start(void);
	void End(void);
	void PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType);
};