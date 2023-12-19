//========================================
// 
// ロケットパーツUIの処理
// Author: RYUKI FUJIWARA
// 
//========================================
#include "coinUI.h"
#include "../main.h"
#include "../Object/Item/coin.h"

#define FRAME_INTER (Scale2D(128.0f,30.0f))				//フレーム間隔
#define SCALE		(4.0f)								//拡縮値

//静的メンバ変数

//========================================
// コンストラクタ
//========================================
CCoinUI::CCoinUI(void) {

	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_scale = Scale2D(SCALE, SCALE);
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\coin.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\parts_frame.png");
	m_num = 0;
	m_bFrame = true;
}

//========================================
// デストラクタ
//========================================
CCoinUI::~CCoinUI(void) {

}

//========================================
// 初期化処理
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot,Scale2D scale,bool bframe) {

	SetPos(pos);
	SetRot(rot);
	SetScale(scale);
	SetbFrame(bframe);
}

//========================================
// 終了処理
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Uninit(void) {

}

//========================================
// 更新処理
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Update(void) {

	if (m_bFrame) {
		//フレーム
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_pos.x + m_scale.x, m_pos.y),0.0f)
			->SetSize(m_scale.x * 5.0f, m_scale.y * 1.5f)
			->SetTex(m_TexIdx[1]);

		//コイン
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_pos.x, m_pos.y),0.0f)
			->SetSize(m_scale.x, m_scale.y)
			->SetTex(m_TexIdx[0]);

		//数
		RNLib::Text2D().Put(PRIORITY_UI, String("%d", CCoin::GetNum()), _RNC_Text::ALIGNMENT::LEFT, 0, Pos2D(m_pos.x + m_scale.x, m_pos.y),0.0f)
			->SetSize(Size2D(m_scale.x * 0.7f, m_scale.y* 0.7f))
			->SetCol(COLOR_WHITE);
	}
	else if (!m_bFrame) {

		//コイン
		RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), m_rot)
			->SetSize(m_scale.x, m_scale.y)
			->SetTex(m_TexIdx[0])
			->SetZTest(false);

		//数
		RNLib::Text3D().Put(PRIORITY_UI, String("%d",CCoin::GetNum()), _RNC_Text::ALIGNMENT::LEFT, 0, Pos3D(m_pos.x + 4.5f, m_pos.y + 0.8f, m_pos.z), m_rot)
			->SetSize(Size2D(3.0f, 3.0f))
			->SetCol(COLOR_WHITE)
			->SetZTest(false);
	}
}

//========================================
// 生成処理
// Author: RYUKI FUJIWARA
//========================================
CCoinUI *CCoinUI::Create(D3DXVECTOR3 pos,Scale2D scale,bool bframe, D3DXVECTOR3 rot)
{
	CCoinUI *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CCoinUI;

	// 初期化処理
	pObj->Init(pos,rot, scale, bframe);

	return pObj;
}