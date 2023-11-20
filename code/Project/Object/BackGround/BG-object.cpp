//========================================
// 
// �w�i�I�u�W�F�N�g�̏���
// Author:RYUKI FUJIWARA
// 
//========================================
#include "BG-object.h"
#include "../../../_RNLib/RNlib.h"

//========================================
// �R���X�g���N�^
//========================================
CBGObject::CBGObject() {

	Manager::BGMgr()->AddList(this);
	m_pos = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
}

//========================================
// �f�X�g���N�^
//========================================
CBGObject::~CBGObject() {

	// ���X�g����폜
	Manager::BGMgr()->SubList(this);

	// ���
	if (m_Info.createTime != NONEDATA) {
		CMemory::Release(&m_Info.moveInfo);
	}
}

//========================================
// �X�V����
//========================================
void CBGObject::Update(void) {

	// [[[ �������� ]]]
	if (m_Info.createTime > 0) {
		if (RNLib::Count().GetCount() % m_Info.createTime == 0) {
			CBGEditor::INFO info = m_Info;
			CBGObject* pBGObj = new CBGObject;
			info.createTime = NONEDATA;
			pBGObj->SetInfo(info);
		}

		return;
	}

	// [[[ ��]���� ]]]
	m_Info.rot += m_Info.spin;

	// [[[ �ړ����� ]]]
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
		m_pos += m_Info.move;
	}

	// [[[ �������� ]]]
	float lifeRate = 1.0f;
	if (m_Info.life != NONEDATA) {
		if (--m_Info.life == 0) {
			Delete();
			return;
		}
		
		lifeRate = CEase::Easing(CEase::TYPE::LINEAR, m_Info.life, m_Info.lifeMax);
	}

	// [[[ �`�� ]]]
	Color putCol = m_Info.col;
	putCol.a *= lifeRate;
	if (m_Info.side == CBGEditor::WORLD_SIDE::UP) {
		RNLib::Model().Put(PRIORITY_BACKGROUND, m_Info.modelIdx, m_pos, m_Info.rot)
			->SetCol(putCol)
			->SetClippingCamera(CMode_Game::GetCameraUp());
	}
	else if (m_Info.side == CBGEditor::WORLD_SIDE::DOWN) {
		RNLib::Model().Put(PRIORITY_BACKGROUND, m_Info.modelIdx, m_pos, m_Info.rot)
			->SetCol(putCol)
			->SetClippingCamera(CMode_Game::GetCameraDown());
	}
}