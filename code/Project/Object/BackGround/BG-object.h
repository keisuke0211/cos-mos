//========================================
// 
// 背景オブジェクトの処理
// Author:FUJIWARA RYUKI
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../../System/BG-Editor.h"

//****************************************
// クラス定義
//****************************************
class CBGObject : public CObject {
public:

	//========== [[[ 関数宣言 ]]]
	CBGObject();
	virtual ~CBGObject();
	virtual void Update(void);
	D3DXVECTOR3& GetPos(void) { return m_pos; }
	void         SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3& GetRot(void) { return m_rot; }
	void         SetInfo(CBGEditor::INFO info) {
		m_Info = info;
		if (m_Info.createTime > 0)
			return;
		m_Info.lifeMax = m_Info.life;
		m_pos  = m_Info.pos;
		m_pos.x += m_Info.posAdd.x * fRand();
		m_pos.y += m_Info.posAdd.y * fRand();
		m_pos.z += m_Info.posAdd.z * fRand();
		m_Info.spin.x += m_Info.spinAdd.x * fRand();
		m_Info.spin.y += m_Info.spinAdd.y * fRand();
		m_Info.spin.z += m_Info.spinAdd.z * fRand();
	}

protected:
	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	CBGEditor::INFO m_Info;
};