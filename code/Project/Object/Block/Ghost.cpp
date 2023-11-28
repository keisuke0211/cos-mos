//========================================
// 
// 幽霊の処理
// Author:KOMURO HIROMU
// 
//========================================
#include"Ghost.h"
#include "../../main.h"
#include "../../collision.h"

#define MAX_MAG_COLOR		(60)
#define MAX_SCALE		(Scale2D(1024.0f * 0.25f,512.0f * 0.25f))
//========================================
// コンストラクタ
//========================================
CGhost::CGhost()
{
	Manager::StageObjectMgr()->AddList(this);

	// タイプの設定
	m_type = CStageObject::TYPE::GHOST;
	m_pos = INITD3DXVECTOR3;
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Ghost0.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Ghost1.png");
	m_nMagCnt = 0;
	m_nPlayerNumber = 0;
}
//========================================
// デストラクタ
//========================================
CGhost::~CGhost()
{

}

//========================================
// 初期化処理
// Author: KOMURO HIROMU
//========================================
void CGhost::Init(void) {


}

//========================================
// 終了処理
// Author: KOMURO HIROMU
//========================================
void CGhost::Uninit(void) {

}

//========================================
// 更新処理
// Author: KOMURO HIROMU
//========================================
void CGhost::Update(void) {
	m_nMagCnt++;	// カウントを増加

	if (m_nMagCnt >= MAX_MAG_COLOR)
	{
		m_nMagCnt = MAX_MAG_COLOR;
	}
	// 割合計算 
	float fCountRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_nMagCnt, MAX_MAG_COLOR);

	
	RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), INITROT3D)
		->SetSize(MAX_SCALE.x, MAX_SCALE.y)
		->SetZTest(false)
		->SetLighting(false)
		->SetBillboard(false)
		->SetTex(m_TexIdx[m_nPlayerNumber]);
		//->SetCol(m_color);

	m_color.a = 255 * fCountRate;

	if (m_nPlayerNumber == 0)
	{
		//m_pos.y += 0.1f;

	}
	else
	{
		//m_pos.y -= 0.1f;
	}
}

//========================================
// 生成処理
// Author: KOMURO HIROMU
//========================================
CGhost *CGhost::Create(D3DXVECTOR3 pos, int nplayernumber)
{
	CGhost *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CGhost;

	// 初期化処理
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetPlayer(nplayernumber);

	return pObj;
}