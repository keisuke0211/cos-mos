//========================================
// 
// 松明の処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once
#ifndef _TORCH_H_
#define _TORCH_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// 松明の情報構造体
class CTorch : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CTorch();
	virtual        ~CTorch();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

private:

	//========== [[[ 変数宣言 ]]]
	float m_fBlink;		// 点滅
	int	  m_nBlinkAnim;	// 点滅アニメーション
	int ModelIdx;
};

#endif