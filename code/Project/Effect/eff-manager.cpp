//========================================
// 
// エフェクトマネージャーの処理
// Author:RYUKI FUJIWARA
// 
// 
//========================================
#include "eff-manager.h"
#include "../main.h"
#include "../collision.h"
#include "../resource.h"

//========================================
// 静的変数
//========================================

//========================================
// コンストラクタ
//========================================
CEffMgr::CEffMgr(void):CObjectMgr("EffectMgr")
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
// モデルエフェクト
//========================================
CEffect_Model *CEffMgr::ModelEffectCreate(int nIdx, D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR3 scale, Color col, int nCount, D3DXVECTOR3 move)
{
	CEffect_Model *pObj = NULL;

	pObj = new CEffect_Model;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init(nIdx,nCount,move);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetColor(col);

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
CParticle *CEffMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos,D3DXVECTOR3 scale,Color col, CParticle::TYPE type ,int nCount, D3DXVECTOR3 rot,int magni, CDrawState::ALPHA_BLEND_MODE alphamode)
{
	CParticle *pObj = NULL;

	pObj = new CParticle;

	// 初期化処理
	pObj->SetType(type);
	pObj->SetRdmMagni(magni);
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetAlphaMode(alphamode);
	pObj->Init(nTex, nCount);
	pObj->SetColor(col);

	return pObj;
}

//========================================
// 死亡パーティクル
// Author:HIRASAWA SHION
//========================================
CEffect_Death *CEffMgr::DeathParticleCreate(int nIdx, Vector3D pos, Vector3D move, Vector3D rot, Vector3D spin, float size, Color color, CEffect_Death::TYPE type)
{
	//インスタンス生成
	CEffect_Death *pEff = new CEffect_Death;

	//情報設定
	if (pEff != NULL)
		pEff->SetInfo(pos, pos, move, rot, spin, size, color, NONEDATA, nIdx, type);

	//インスタンスを返す
	return pEff;
}

//========================================
// デスインク
// Author:KEISUKE OTONO
//========================================
void CEffMgr::DeathInk(D3DXVECTOR3 pos,int Tex) {

	for (int nInk = 0; nInk < 3; nInk++) {

		bool bSet = false;

		while (1)
		{
			float rot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
			float scale = (float)(rand() % (int)12 + 3);
			float distance = (float)(rand() % (int)16 + 16);

			D3DXVECTOR3 InkPos = pos;
			D3DXVECTOR3 move = INITD3DXVECTOR3;

			move = D3DXVECTOR3(distance * sinf(rot), distance * cosf(rot),0.0f);

			InkPos += move;

			// オブジェクト1つ1つを見ていく
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {

				// 取得したオブジェクトをキャスト
				CStageObject* stageObj = (CStageObject*)obj;

				// 種類取得
				CStageObject::TYPE type = stageObj->GetType();

				if (type == CStageObject::TYPE::BLOCK || type == CStageObject::TYPE::FILLBLOCK) {

					D3DXVECTOR3 ObjPos = stageObj->GetPos();
					D3DXVECTOR3 ObjSize = D3DXVECTOR3(stageObj->GetWidth(), stageObj->GetHeight(), 0.0f);

					if ((InkPos.x + scale/2) > (ObjPos.x - ObjSize.x/2) &&
						(InkPos.x - scale/2) < (ObjPos.x + ObjSize.x/2) &&
						(InkPos.y + scale/2) > (ObjPos.y - ObjSize.y/2) &&
						(InkPos.y - scale/2) < (ObjPos.y + ObjSize.y/2))
					{// インクが判定内にある時、

						bSet = true;
						Manager::EffectMgr()->EffectCreate(Tex, InkPos, D3DXVECTOR3(scale,scale,0.0f), Color{ 255,0,255,255 },-44);
						break;
					}
				}

			}

			if (bSet)	break;
		}

	}
}