//========================================
// 
// �^�C�g��SE����
// Author:SYUUKI IIDA
// 
//========================================
#pragma once

#include "../Object/Block/block.h"

//****************************************
// ���O���
//****************************************
namespace titleSound {

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Start(void);
	void End(void);
	void PlayLandingSEIdx(const CBlock::LOOKS_TYPE& looksType);
};