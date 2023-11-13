//========================================
// 
// 背景オブジェクトの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#include "BG-object.h"
#include "../../../_RNLib/RNlib.h"

//========================================
// コンストラクタ
//========================================
CBGObject::CBGObject() {

	Manager::BGMgr()->AddList(this);
	m_pos = INITD3DXVECTOR3;
	m_posOld = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_color = INITCOLOR;
	m_width = 0.0f;
	m_height = 0.0f;
}

//========================================
// デストラクタ
//========================================
CBGObject::~CBGObject() {

	// リストから削除
	Manager::BGMgr()->SubList(this);
}

//========================================
// 終了処理
//========================================
void CBGObject::Uninit(void) {

}

////======================================
// 終了処理
//========================================
void CBGObject::Update(void) {

	int ModelIdx = RNLib::Model().Load(m_Info.ModelPath);

	m_Info.rot += m_Info.spin;

	if (m_Info.side == CBGEditor::WORLD_SIDE::UP)
	{
		RNLib::Model().Put(m_Info.pos, m_Info.rot, ModelIdx)
			->SetCol(m_Info.col)
			->SetOutLine(true)
			->SetClippingCamera(CMode_Game::GetCameraUp());
	}
	if (m_Info.side == CBGEditor::WORLD_SIDE::DOWN)
	{
		RNLib::Model().Put(m_Info.pos, m_Info.rot, ModelIdx)
			->SetCol(m_Info.col)
			->SetOutLine(true)
			->SetClippingCamera(CMode_Game::GetCameraDown());
	}
	
	Uninit();
}