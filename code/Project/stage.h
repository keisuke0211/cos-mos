//========================================
// 
// ステージの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "Character/player.h"
#include "UI/partsUI.h"

//****************************************
// 名前空間
//****************************************
namespace Stage {
	//========== [[[ 関数宣言 ]]]
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

	//各ステージごとのデータ
	struct Data
	{
		int CoinNums; //コインの枚数
		bool *pGet;   //各ステージの各コインの取得状況
	};

	float GetBestTime(CInt& planetIdx, CInt& stageIdx);
	void  RegistTime(CInt& planetIdx, CInt& stageIdx, CFloat& ClearTime);
	Data  GetData(CInt& planetIdx, CInt& stageIdx);
	void  SetCoinInfo(CInt& planetIdx, CInt& stageIdx, const Data& data);
	void  SetCoinInfo(CInt& planetIdx, CInt& stageIdx, CInt& coinID, const bool& bGet);
};