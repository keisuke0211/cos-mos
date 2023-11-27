//========================================
// 
// エフェクトマネージャーの処理
// Author:RYUKI FUJIWARA
// 
// 
//========================================
#include "eff-manager.h"

//========================================
// 静的変数
//========================================

//========================================
// コンストラクタ
//========================================
CEffMgr::CEffMgr(void)
{
}

//========================================
// デストラクタ
//========================================
CEffMgr::~CEffMgr()
{
}

//========================================
// 更新
//========================================
void CEffMgr::Update(void)
{

}

//========================================
// エフェクト
//========================================
CEff *CEffMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount, D3DXVECTOR3 spin, D3DXVECTOR3 move,bool bbilborad, D3DXVECTOR3 scalemag)
{
	CEff *pObj = NULL;

	pObj = new CEff;

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
CEffect_Meteor * CEffMgr::EffectMeteorCreate(D3DXVECTOR3 pos)
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
CParticle *CEffMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos,D3DXVECTOR3 scale,Color col, CParticle::TYPE type ,int nCount, D3DXVECTOR3 rot)
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

//========================================
// 死亡パーティクル
// Author:HIRASAWA SHION
//========================================
CEffect_Death *CEffMgr::DeathParticleCreate(int nTex, Vector3D pos, Vector3D move, Vector3D rot, Vector3D spin, Vector2D size, Color color, CEffect_Death::TYPE type)
{
	//インスタンス生成
	CEffect_Death *pEff = new CEffect_Death;

	//情報設定
	if (pEff != NULL)
		pEff->SetInfo(pos, pos, move, rot, spin, size, color, NULL, nTex, type);

	//インスタンスを返す
	return pEff;
}