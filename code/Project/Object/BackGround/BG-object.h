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
	virtual void Uninit(void);
	virtual void Update(void);
	void SetPos(const D3DXVECTOR3& pos) { m_pos = m_posOld = pos; }
	D3DXVECTOR3& GetPos(void) { return m_pos; }
	D3DXVECTOR3& GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3& GetRot(void) { return m_rot; }
	void SetColor(Color color) { m_color = color; }
	float& GetWidth(void) { return m_width; }
	float& GetHeight(void) { return m_height; }
	void SetInfo(CBGEditor::INFO info) { m_Info = info; }

protected:
	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_rot;
	Color       m_color;
	float       m_width;
	float       m_height;
	CBGEditor::INFO m_Info;
};