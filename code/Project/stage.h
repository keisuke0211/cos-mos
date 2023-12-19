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
	bool     CheckPlanetIdx  (const int& planetIdx);
	bool     CheckStageNumber(const int& planetIdx, const int& stageIdx);
	void     StartStage      (void);
	void     UpdateStage     (void);
	void     EndStage        (void);
	void     SetStageNumber  (const int& planetIdx, const int& stageIdx);
	void     SetPause        (const bool& isPause);
	void     SetRocketPartsUI(CRocketPartsUI* parts);
	void     SetIsCutIn      (const bool& isCutIn);
	CPlayer* GetPlayer       (void);

	//�e�X�e�[�W���Ƃ̃f�[�^
	struct Data
	{
		int CoinNums; //�R�C���̖���
		bool *pGet;   //�e�X�e�[�W�̊e�R�C���̎擾��
	};

	float GetBestTime(CInt& planetIdx, CInt& stageIdx);
	void  RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime);
	Data  GetData(CInt& planetIdx, CInt& stageIdx);
	void  SetCoinInfo(CInt& planetIdx, CInt& stageIdx, const Data& data);
	void  SetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID, const bool& bGet);
};