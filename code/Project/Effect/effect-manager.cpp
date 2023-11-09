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
CEffect *CEffectMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount)
{
	CEffect *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CEffect;

	// 初期化処理
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);


	CObjectMgr::AddList((CObject*)pObj);

	return pObj;
}
//========================================
// エフェクト
//========================================
CEffect_Meteor * CEffectMgr::EffectMeteorCreate(D3DXVECTOR3 pos)
{
	CEffect_Meteor *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CEffect_Meteor;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init();

	CObjectMgr::AddList((CObject*)pObj);

	return pObj;
}
//========================================
// パーティクル
//========================================
CParticle *CEffectMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount)
{
	CParticle *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CParticle;

	// 初期化処理
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);

	CObjectMgr::AddList((CObject*)pObj);

	return pObj;
}
