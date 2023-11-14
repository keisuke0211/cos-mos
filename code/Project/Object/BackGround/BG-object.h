//========================================
// 
// �w�i�I�u�W�F�N�g�̏���
// Author:FUJIWARA RYUKI
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../../System/BG-Editor.h"

//****************************************
// �N���X��`
//****************************************
class CBGObject : public CObject {
public:

	//========== [[[ �֐��錾 ]]]
	CBGObject();
	virtual ~CBGObject();
	virtual void Update(void);
	D3DXVECTOR3& GetPos(void) { return m_pos; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3& GetRot(void) { return m_rot; }
	void SetInfo(CBGEditor::INFO info) { m_Info = info; m_pos = m_Info.pos; }

protected:
	//========== [[[ �ϐ��錾 ]]]
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	CBGEditor::INFO m_Info;
};