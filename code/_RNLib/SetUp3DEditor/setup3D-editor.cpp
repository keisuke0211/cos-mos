//========================================
// 
// �Z�b�g�A�b�v3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "setup3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�Z�b�g�A�b�v3D�G�f�B�^�[�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CSetUp3DEditor::CSetUp3DEditor() {

	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetBGCol(Color{ 128,128,128,255 });
	m_camera->SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera->SetPosVAndPosR(Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);

	m_doll = new CDoll3D((UShort)RNMode::PRIORITY::OBJECT3D, EDITDATA);
	m_doll->SetPos(Pos3D(0.0f, 0.0f, 0.0f));
	RNLib::Doll3DMgr().SetEditDoll(m_doll);
	RNLib::Doll3DMgr().SetEditCamera(m_camera);
	RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxNum(10);

	m_slither        = INITVECTOR3D;
	m_slitherCounter = 0;
	m_editPath       = NULL;
	m_messageType    = MESSAGE_TYPE::NONE;
	m_messageCounter = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CSetUp3DEditor::~CSetUp3DEditor() {

	CMemory::Release(&m_editPath);
}

//========================================
// ����������
//========================================
void CSetUp3DEditor::Init(void) {

}

//========================================
// �I������
//========================================
void CSetUp3DEditor::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CSetUp3DEditor::Update(void) {

	// [[[ �J�����`�� ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(m_camera)
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ �O���b�h�`�� ]]]
	RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, INITPOS3D, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
		->SetSize(Size2D(90.0f, 90.0f))
		->SetTexUV(
			RNLib::DefaultData().GetTextureIdx(CDefaultData::TEXTURE::GRID),
			Pos2D(0.0f, 0.0f),
			Pos2D(9.0f, 0.0f),
			Pos2D(0.0f, 9.0f),
			Pos2D(9.0f, 9.0f));

	{// [[[ ���� ]]]
		// �V�K�쐬
		if (RNLib::Input().GetKeyTrigger(DIK_1)) {
			CMemory::Release(&m_editPath);
			if (RNLib::File().GetSelectSaveFileName(&m_editPath, "", ".txt")) {
				RNLib::SetUp3D().LoadEditData("RNData\\SetUp3D\\Capsule.txt");
				RNLib::SetUp3D().SaveEditData(m_editPath);
			}
		}

		// �ǂݍ���
		if (RNLib::Input().GetKeyTrigger(DIK_2)) {
			CMemory::Release(&m_editPath);
			if (RNLib::File().GetSelectOpenFileName(&m_editPath, "", ".txt")) {
				RNLib::SetUp3D().LoadEditData(m_editPath);
			}
		}
		
		// �ēǂݍ���
		if (RNLib::Input().GetKeyTrigger(DIK_3)) {
			if (m_editPath != NULL) {
				m_messageType = RNLib::SetUp3D().LoadEditData(m_editPath) ?
					MESSAGE_TYPE::RELOAD_SUCCEEDED:
					MESSAGE_TYPE::RELOAD_FAILED;
			}
			else {
				m_messageType = MESSAGE_TYPE::RELOAD_FAILED;
			}
			m_messageCounter = 30;
		}
		
		// ���_�ԍ��̕`��ON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_4)) {
			RNLib::Doll3DMgr().SetEditDollIsDrawModelVtxIdx(!RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx());
		}

		// ���˂��˒l
		if (RNLib::Input().GetKeyTrigger(DIK_Q))
			m_slither.x -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_W))
			m_slither.x += 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_A))
			m_slither.y -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_S))
			m_slither.y += 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_Z))
			m_slither.z -= 0.1f;
		if (RNLib::Input().GetKeyTrigger(DIK_X))
			m_slither.z += 0.1f;

		if (false) {}
		else if (m_slither.x < 0.0f) m_slither.x = 0.0f;
		else if (m_slither.x > 1.0f) m_slither.x = 1.0f;
		else if (m_slither.y < 0.0f) m_slither.y = 0.0f;
		else if (m_slither.y > 1.0f) m_slither.y = 1.0f;
		else if (m_slither.z < 0.0f) m_slither.z = 0.0f;
		else if (m_slither.z > 1.0f) m_slither.z = 1.0f;

		{// ���_�ԍ��̕`��{�[���ԍ��؂�ւ�
			short boneIdx = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx();

			if (RNLib::Input().GetKeyTrigger(DIK_E)) {
				if (--boneIdx < 0) {
					boneIdx = RNLib::SetUp3D().GetData(EDITDATA).m_boneDataNum - 1;
				}
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_R)) {
				boneIdx = (boneIdx + 1) % RNLib::SetUp3D().GetData(EDITDATA).m_boneDataNum;
			}

			RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxBoneIdx(boneIdx);
		}

		{// ���_�ԍ��̕`�搔
			short drawVtxNum = RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum();

			if (RNLib::Input().GetKeyTrigger(DIK_D)) {
				if (--drawVtxNum < 0)
					drawVtxNum = 0;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_F)) {
				drawVtxNum++;
			}

			RNLib::Doll3DMgr().SetEditDollDrawModelVtxIdxNum(drawVtxNum);
		}
	}

	{
		int countResult = 0;
		const int slitherTimeHalf = SLITHER_TIME * 0.5f;
		if (++m_slitherCounter == SLITHER_TIME) {
			m_slitherCounter = 0;
		}
		else if (m_slitherCounter > slitherTimeHalf) {
			countResult = slitherTimeHalf + (slitherTimeHalf - m_slitherCounter);
		}
		else {
			countResult = m_slitherCounter;
		}

		Rot3D rot = -m_slither + (m_slither * 2.0f * CEase::Easing(CEase::TYPE::INOUT_SINE, countResult, slitherTimeHalf));

		const CSetUp3D::CData& setUp = RNLib::SetUp3D().GetData(EDITDATA);
		if (&setUp != NULL) {
			for (short cntBone = 0; cntBone < setUp.m_boneDataNum; cntBone++) {
				m_doll->GetBoneState(cntBone).SetRot(rot);
			}
		}
	}

	// [[[ ���b�Z�[�W�J�E���g ]]]
	if (m_messageCounter > 0) {
		if (--m_messageCounter == 0) {
			m_messageType = MESSAGE_TYPE::NONE;
		}
	}

	// [[[ ���O�\�� ]]]
	RNLib::Text2D().PutDebugLog(CreateText("-----Information-----"));
	RNLib::Text2D().PutDebugLog(CreateText("EditPath:%s", m_editPath));
	RNLib::Text2D().PutDebugLog(CreateText("-----Control-----"));
	RNLib::Text2D().PutDebugLog(CreateText("CreateNew        [1]"));
	RNLib::Text2D().PutDebugLog(CreateText("Load             [2]"));
	RNLib::Text2D().PutDebugLog(CreateText("ReLoad           [3] %s", m_messageType == MESSAGE_TYPE::RELOAD_SUCCEEDED ? "SUCCEEDED!" : m_messageType == MESSAGE_TYPE::RELOAD_FAILED ? "FAILED!" : ""));
	RNLib::Text2D().PutDebugLog(CreateText("DrawVtxIdx       [4]   :%s", RNLib::Doll3DMgr().GetEditDollIsDrawModelVtxIdx() ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(CreateText("DrawVtxIdxBoneIdx[E][R]:%d", RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxBoneIdx()));
	RNLib::Text2D().PutDebugLog(CreateText("DrawVtxIdxNum    [D][F]:%d", RNLib::Doll3DMgr().GetEditDollDrawModelVtxIdxNum()));
	RNLib::Text2D().PutDebugLog(CreateText("SlitherX         [Q][W]:%.1f", m_slither.x));
	RNLib::Text2D().PutDebugLog(CreateText("SlitherY         [A][S]:%.1f", m_slither.y));
	RNLib::Text2D().PutDebugLog(CreateText("SlitherZ         [Z][X]:%.1f", m_slither.z));
}