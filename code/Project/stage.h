//========================================
// 
// �X�e�[�W�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "Character/player.h"
#include "UI/partsUI.h"

//****************************************
// ���O���
//****************************************
namespace Stage {
	//========== [[[ �֐��錾 ]]]
	void     Init            (void);
	void     Uninit          (void);
	void     SetStageNumber  (const int& planetIdx, const int& stageIdx);
	CPlayer* GetPlayer       (void);
	void     SetPause        (const bool& isPause);
	void     SetRocketPartsUI(CRocketPartsUI* parts);
	bool     CheckPlanetIdx  (const int& planetIdx);
	bool     CheckStageNumber(const int& planetIdx, const int& stageIdx);
	void     StartStage      (void);
	void     UpdateStage     (void);
	void     EndStage        (void);
};