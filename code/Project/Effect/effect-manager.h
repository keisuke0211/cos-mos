//========================================
// 
// エフェクトマネージャーの処理
// Author:RYUKI FUJIWARA
//
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "effect.h"
#include "particle.h"

#define INIT_EFFECT_SCALE (D3DXVECTOR3(32.0f,32.0f,0.0f))

//****************************************
// クラス
//****************************************
class CEffectMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CEffectMgr();
	virtual~CEffectMgr();
	virtual void Update(void);

	CEffect *EffectCreate(int nTex, D3DXVECTOR3 pos , D3DXVECTOR3 scale, Color col);
	CParticle *ParticleCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale, Color col);

private:

	// *** 構造体 ***

	// *** 静的変数 ***

	// *** 変数 ***
};
