//================================================================================================
//
//隕石生成器の処理[meteor_generator.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _METEOR_GENERATOR_H_
#define _METEOR_GENERATOR_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// 前方宣言
//****************************************
class CMeteor;
//****************************************
// クラス定義
//****************************************
// メテオの情報構造体
class CMeteorGenerator : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CMeteorGenerator();
	virtual        ~CMeteorGenerator();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// 移動量の設定
	void SetInterval(int nSummonInteval) { m_nSummoninterval = nSummonInteval; }	// 間隔の設定
private:
	static CMeteor *pMeteor;
	D3DXVECTOR3 m_move;
	int m_nSummoninterval;
	int m_nCntSummon;
};

#endif