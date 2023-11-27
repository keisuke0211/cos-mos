//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

//=======================================
// コンストラクタ
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);
}

//=======================================
// デストラクタ
//=======================================
CEffect_Death::~CEffect_Death()
{
	Manager::EffectMgr()->SubList(this);
}

//=======================================
//更新処理
//=======================================
void CEffect_Death::Update(void)
{
	//種類別更新処理
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); break;
		case TYPE::INK: UpdateType_Ink();  break;
	}
	
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetBillboard(true)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	//［寿命］共通更新処理
	Life();
}

//=======================================
//【ボール】更新処理
//=======================================
void CEffect_Death::UpdateType_Ball(void)
{
	Move();
	Spin();
}

//=======================================
//【インク】更新処理
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{

}

//=======================================
//［移動］共通更新処理
//=======================================
void CEffect_Death::Move(void)
{
	m_Info.pos = m_Info.move;
}

//=======================================
//［回転］共通更新処理
//=======================================
void CEffect_Death::Spin(void)
{
	m_Info.rot = m_Info.spin;
}

//=======================================
//［寿命］共通更新処理
//=======================================
void CEffect_Death::Life(void)
{
	//寿命が存在し、尽きた
	if (m_Info.pLife != NULL &&	--*m_Info.pLife <= 0)
		CObject::Delete();
}