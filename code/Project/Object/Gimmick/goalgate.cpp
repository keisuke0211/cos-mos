//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "goalgate.h"
#include "../../main.h"
#include "../../../_RNLib/Basis/input.h"
#include "../../Character/player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CGoalGateクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CGoalGate::CGoalGate(void) {
	Manager::BlockMgr()->AddList(this);

	//初期状態
	m_type = TYPE::GOALGATE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 2.0f;
	m_bGoal = false;
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Star_000.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");
}

//========================================
// デストラクタ
//========================================
CGoalGate::~CGoalGate(void) {

}

//========================================
// 初期化処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Init(void) {

}

//========================================
// 終了処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Uninit(void) {

}

//========================================
// 更新処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Update(void) {

	// ゴールポリゴン
	RNLib::Polygon3D().Put(m_pos, INITD3DXVECTOR3)
		->SetTex(m_TexIdx[0])
		->SetBillboard(true);

	Manager::EffectMgr()->ParticleCreate(m_TexIdx[1],m_pos, INIT_EFFECT_SCALE, INITCOLOR);
}
//========================================
// 描画処理
// Author:RYUKI FUJIWARA
//========================================
void CGoalGate::Draw(void) 
{

}