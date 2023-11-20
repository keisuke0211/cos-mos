//========================================
// 
// パーツの処理
// Author:KOMURO HIROMU
// 
//========================================
#include "Parts.h"
#include "../../main.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CPartsクラスのメンバ変数
//----------|---------------------------------------------------------------------
//================================================================================
int					CParts::s_nNumAll = 0;	
const D3DXVECTOR3	CParts::PARTS_ADDROT = { 0.0f,0.02f,0.0f };

//========================================
// コンストラクタ
//========================================
CParts::CParts(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::PARTS;	// 種類の設定
							// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 2;
	m_height = SIZE_OF_1_SQUARE * 2;
	m_bDisp = true;

	// 各情報の初期化
	ModelIdx = RNLib::Model().Load("data\\MODEL\\Rocket_Engine_break.x");
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nEffctAnimCnt = 0;
	s_nNumAll++;
}

//========================================
// デストラクタ
//========================================
CParts::~CParts(void) {

	s_nNumAll--;
}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CParts::Init(void) {

}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CParts::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU (Hirasawa Shion)
//========================================
void CParts::Update(void) {
	
	if (!m_bDisp) return;

	m_rot += PARTS_ADDROT;	// 回転の移動量の増加
	float fBrightness = fabsf(m_rot.y) / (D3DX_PI * 0.5f);

	if (fBrightness >= 1.0f)
	{
		fBrightness = fBrightness - (fBrightness  - 1.0f);
	}

	if (fBrightness < 0)
	{
		fBrightness = fabsf(fBrightness);
	}
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}
	fBrightness = 0.5f + fBrightness * 0.5f;

	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot, false)
		->SetCol(Color{ (UShort)(255 * fBrightness),(UShort)(255 * fBrightness),(UShort)(255 * fBrightness),255 })
		->SetOutLine(true);

	//int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	//m_nEffctAnimCnt++;

	//if (m_nEffctAnimCnt % 6 == 1)
	//{
	//	Manager::EffectMgr()->EffectCreate(ParTex, m_pos, INIT_EFFECT_SCALE, INITCOLOR);
	//}
}

//---------------------------
// 取得したら描画 ON/OFF を切り替え
// Author：Hirasawa Shion
//---------------------------
void CParts::DispSwitch(bool bGet)
{
	//描画切り替え
	m_bDisp = bGet;
}