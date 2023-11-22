//========================================
// 
// エフェクトマネージャーの処理
// Author:RYUKI FUJIWARA
// 
// 
//========================================
#include "effect-manager.h"

//========================================
// 静的変数
//========================================

//========================================
// コンストラクタ
//========================================
CEffectMgr::CEffectMgr(void)
{
}

//========================================
// デストラクタ
//========================================
CEffectMgr::~CEffectMgr()
{
}

//========================================
// 更新
//========================================
void CEffectMgr::Update(void)
{

}

//========================================
// エフェクト
//========================================
CEffect *CEffectMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount, D3DXVECTOR3 spin, D3DXVECTOR3 move,bool bbilborad, D3DXVECTOR3 scalemag)
{
	CEffect *pObj = NULL;

	pObj = new CEffect;

	// 初期化処理
	pObj->Init(nTex, nCount, spin);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);
	pObj->SetMove(move);
	pObj->SetScalemag(scalemag);
	pObj->SetBillboard(bbilborad);

	return pObj;
}
//========================================
// 隕石エフェクト
//========================================
CEffect_Meteor * CEffectMgr::EffectMeteorCreate(D3DXVECTOR3 pos)
{
	CEffect_Meteor *pObj = NULL;

	pObj = new CEffect_Meteor;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init();


	return pObj;
}
//========================================
// パーティクル
//========================================
CParticle *CEffectMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos,D3DXVECTOR3 scale,Color col, CParticle::TYPE type ,int nCount, D3DXVECTOR3 rot)
{
	CParticle *pObj = NULL;

	pObj = new CParticle;

	// 初期化処理
	pObj->SetType(type);
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetColor(col);
	

	return pObj;
}
