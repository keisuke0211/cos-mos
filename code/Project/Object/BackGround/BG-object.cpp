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

	// 解放
	CMemory::Release(&m_Info.moveInfo);
}

//========================================
// 終了処理
//========================================
void CBGObject::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CBGObject::Update(void) {

	m_Info.rot += m_Info.spin;

	if (m_Info.moveType == CBGEditor::MOVE_TYPE::AB) {
		CBGEditor::MOVE_AB* moveInfo = (CBGEditor::MOVE_AB*)m_Info.moveInfo;
		if (++moveInfo->counter == moveInfo->time) {
			moveInfo->counter = 0;
			moveInfo->isReturn = !moveInfo->isReturn;
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, moveInfo->counter, moveInfo->time);
		if (moveInfo->isReturn)
			rate = 1.0f - rate;

		m_pos = (m_Info.pos * rate) + (moveInfo->posB * (1.0f - rate));
	}
	else {
		m_pos = m_Info.pos;
	}

	if (m_Info.side == CBGEditor::WORLD_SIDE::UP)
	{
		RNLib::Model().Put(m_pos, m_Info.rot, m_Info.modelIdx)
			->SetCol(m_Info.col)
			->SetOutLine(true)
			->SetClippingCamera(CMode_Game::GetCameraUp());
	}
	if (m_Info.side == CBGEditor::WORLD_SIDE::DOWN)
	{
		RNLib::Model().Put(m_pos, m_Info.rot, m_Info.modelIdx)
			->SetCol(m_Info.col)
			->SetOutLine(true)
			->SetClippingCamera(CMode_Game::GetCameraDown());
	}
	
	Uninit();
}