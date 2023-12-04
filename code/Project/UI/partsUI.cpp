//========================================
// 
// ロケットパーツUIの処理
// Author: RYUKI FUJIWARA
// 
//========================================
#include "partsUI.h"
#include "../main.h"
#include "../Character/player.h"

#define FRAME_INTER (Scale2D(96.0f,64.0f))				//フレーム間隔
#define UNACQU_COLA	(150)								//未取得のアルファ値
#define SCALE		(50.0f)								//拡縮値
#define INITPOS		(D3DXVECTOR3(1000.0f,50.0f, 0.0f))	//初期位置


//静的メンバ変数
bool CRocketPartsUI::m_bRocketStg = false;
//========================================
// コンストラクタ
//========================================
CRocketPartsUI::CRocketPartsUI(void) {

	m_pos = INITD3DXVECTOR3;
	m_scale = Scale2D(SCALE, SCALE);
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\rocket_001.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\parts_frame.png");
	m_colorA = INITCOLOR.a;
	m_num = 0;
	m_state = NULL;
	m_FramePos = 0.0f;
}

//========================================
// デストラクタ
//========================================
CRocketPartsUI::~CRocketPartsUI(void) {
	if (m_state != NULL)
		delete[] m_state;
	m_state = NULL;
}

//========================================
// 初期化処理
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Init(void) {

	m_pos = INITPOS;

	m_state = new STATE[CParts::GetNumAll()];

	for (int nUI = 0; nUI < CParts::GetNumAll(); nUI++)
	{
		m_state[nUI] = STATE::NONE;

		m_FramePos += m_pos.x + m_scale.x * nUI;
	}

	m_FramePos /= CParts::GetNumAll();
}

//========================================
// 終了処理
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Uninit(void) {

}

//========================================
// 更新処理
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Update(void) {

	if (m_bRocketStg == true)
	{
		if (CParts::GetNumParts() > m_num)
		{
			m_state[m_num++] = STATE::OBTAIN;
		}

		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_FramePos, m_pos.y), 0.0f)
			->SetSize(FRAME_INTER.x * CParts::GetNumAll(), FRAME_INTER.y)
			->SetTex(m_TexIdx[1]);

		for (int nUI = 0; nUI < CParts::GetNumAll(); nUI++)
		{
			if (m_state[nUI] == STATE::NONE)
			{
				m_colorA = UNACQU_COLA;
			}
			else if (m_state[nUI] == STATE::OBTAIN)
			{
				m_colorA = INITCOLOR.a;
			}

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_pos.x + m_scale.x * nUI, m_pos.y), 0.0f)
				->SetSize(m_scale.x, m_scale.y)
				->SetTex(m_TexIdx[0])
				->SetCol(Color{ INITCOLOR.r,INITCOLOR.g,INITCOLOR.b,(UShort)m_colorA });
		}
	}
}

//========================================
// 生成処理
// Author: RYUKI FUJIWARA
//========================================
CRocketPartsUI *CRocketPartsUI::Create(void)
{
	CRocketPartsUI *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRocketPartsUI;

	// 初期化処理
	pObj->Init();
	pObj->SetRocketstg(true);

	return pObj;
}