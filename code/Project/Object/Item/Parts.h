//========================================
// 
// パーツの処理
// Author:KOMURO HIROMU　Hirasawa Shion
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// パーツの情報構造体
class CParts : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CParts();
	virtual        ~CParts();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	//---------------------------
	//描画 ON/OFF 状態を取得
	//---------------------------
	bool	GetDisp(void) { return m_bDisp; }

	//---------------------------
	//取得したら描画 ON/OFF を切り替え
	//---------------------------
	void	DispSwitch(bool bGet);

	//---------------------------
	//パーツの全体数を取得
	//---------------------------
	static int GetNumAll(void) { return s_nNumAll; }

private:
	//========== [[[ 変数宣言 ]]]
	static int s_nNumAll;					// パーツがいくつ存在するか
	static const D3DXVECTOR3 PARTS_ADDROT;	// パーツの回転の移動量

	int m_nEffctAnimCnt;	// エフェクトアニメカウント 
	int ModelIdx;
	bool m_bDisp;		//　取得したかどうか
}; 
