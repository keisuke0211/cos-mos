//========================================
// 
// 隕石の処理
// Author:KOMURO HIROMU  Hirasawa Shion
// 
//========================================
#pragma once
#ifndef _METEOR_H_
#define _METEOR_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// メテオの情報構造体
class CMeteor : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CMeteor();
	virtual        ~CMeteor();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void	SetMove(D3DXVECTOR3 move) { m_move = move; }

private:
	static const D3DXVECTOR3 METEOR_ADDROT;    // 隕石の回転の移動量
	static const D3DXVECTOR3 METEOR_MINPOS;    // 最小の位置
	static const D3DXVECTOR3 METEOR_MAXPOS;    // 最大の位置
	static const int         METEOR_ANIME_MAG; // 点滅アニメーションの倍率
	static const int         METEOR_BLINK_MAX; // 点滅アニメーションの最大数
	static const float       METEOR_BLINK_MIN; // 点滅の最小数
	static const float       METEOR_BLINK_ADJ; // 点滅アニメーションの調整

	void BlinkAnimation(void);	//点滅アニメーション処理

	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_move;	// 移動量
	float m_fBlink;		// 点滅
	int	  m_nBlinkAnim;	// 点滅アニメーション

	int ModelIdx;
};

#endif