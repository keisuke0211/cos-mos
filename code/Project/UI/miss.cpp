//========================================
// 
// ミスの処理
// Author: KOMURO HIROMU
// 
//========================================
#include "miss.h"
#include "../main.h"

#define MAX_MAG_CNT		(60)		// 倍率の最大カウント
#define MAX_SCALE		(Scale2D(512.0f * 0.75f,128.0f * 0.75f))
//========================================
// コンストラクタ
//========================================
CMiss::CMiss()
{
	Manager::StageObjectMgr()->AddList(this);

	// 種類の設定
	m_type = TYPE::MISS;

	m_TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Miss.png");
	m_nMagCnt = 0;
	m_Scale = MAX_SCALE;
}
//========================================
// デストラクタ
//========================================
CMiss::~CMiss()
{

}

//========================================
// 初期化処理
// Author: KOMURO HIROMU
//========================================
void CMiss::Init(void) {


}

//========================================
// 終了処理
// Author: KOMURO HIROMU
//========================================
void CMiss::Uninit(void) {

}

//========================================
// 更新処理
// Author: KOMURO HIROMU
//========================================
void CMiss::Update(void) {
	m_nMagCnt++;	// カウントを増加

	// 割合計算 
	float fCountRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_nMagCnt, MAX_MAG_CNT);

	RNLib::Polygon3D().Put(PRIORITY_UI, Manager::GetMainCamera()->GetPosR(), INITROT3D)
		->SetSize(m_Scale.x * fCountRate, m_Scale.y * fCountRate)
		->SetZTest(false)
		->SetLighting(false)
		->SetTex(m_TexIdx)
		->SetBillboard(true);
}

//========================================
// 生成処理
// Author: KOMURO HIROMU
//========================================
CMiss *CMiss::Create(void)
{
	CMiss *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMiss;

	// 初期化処理
	pObj->Init();

	return pObj;
}