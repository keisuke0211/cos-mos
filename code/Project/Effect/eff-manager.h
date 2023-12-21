//========================================
// 
// エフェクトマネージャーの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "eff.h"
#include "particle.h"
#include "eff-meteor.h"
#include "eff-death.h"
#include "eff-model.h"
#define EFFECT_COUNT		(60)		// カウント
#define RANDOM_MAGNI		(12)		//ランダム倍率
#define INIT_EFFECT_SCALE	(D3DXVECTOR3(32.0f,32.0f,0.0f))

//****************************************
// クラス
//****************************************
class CEffMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CEffMgr();
	virtual~CEffMgr();
	virtual void Update(void);

	CEff *EffectCreate(int nTex, D3DXVECTOR3 pos , D3DXVECTOR3 scale, Color col, int nCount = EFFECT_COUNT, D3DXVECTOR3 spin = INITD3DXVECTOR3, D3DXVECTOR3 move = INITD3DXVECTOR3,bool billboard = true, D3DXVECTOR3 scalemag = D3DXVECTOR3(1.0f,1.0f,1.0f));
	CEffect_Model *ModelEffectCreate(int nIdx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, Color col, int nCount = EFFECT_COUNT, D3DXVECTOR3 move = D3DXVECTOR3(8.0f, 6.0f, 0.0f));
	CEffect_Meteor * EffectMeteorCreate(D3DXVECTOR3 pos);

	CParticle *ParticleCreate(int nTex,D3DXVECTOR3 pos,D3DXVECTOR3 scale, Color col, CParticle::TYPE type = CParticle::TYPE::TYPE_NORMAL, int nCount = EFFECT_COUNT, D3DXVECTOR3 rot = INITD3DXVECTOR3, D3DXVECTOR3 move = D3DXVECTOR3(10.0f, 10.0f, 0.0f), bool bZtest = false, bool bVec3D = false, _RNC_DrawState::ALPHA_BLEND_MODE alphamode = _RNC_DrawState::ALPHA_BLEND_MODE::ADD, int magni = RANDOM_MAGNI);
	
	//*****************************************
	//死亡パーティクル生成処理
	//最初の引数には、テクスチャ番号 or モデル番号を代入する
	//*****************************************
	CEffect_Death *DeathParticleCreate(Vector3D pos, Vector3D move, Vector3D rot, Vector3D spin, float size, Color color, CEffect_Death::TYPE type);
	void DeathInk(D3DXVECTOR3 pos, int Tex);

private:

	// *** 構造体 ***

	// *** 静的変数 ***

	// *** 変数 ***
};
